/*	--------------------------------------------------------------------------------- 
	Filename      - main.cpp (hvis)
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Code to create the PVS of each leaf in a BSP tree
	--------------------------------------------------------------------------------- */

#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <types.h>
#include <lbsp.h>
#include <debug.h>

// --------------------------------------------------------------------------------- 
// CRay, used to hold two verts that signify a ray to trace for visibility between
// --------------------------------------------------------------------------------- 
class CRay 
{
public:
	CVert3D start;
	CVert3D end;

	// --------------------------------------------------------------------------------- 
	// constructor
	// --------------------------------------------------------------------------------- 
	CRay(CVert3D s, CVert3D e) {
		start = s;
		end   = e;
	}
	void dump() {
		printf("*** RAY s=%f,%f,%f e=%f,%f,%f ***\n", start.x, start.y, start.z, end.x, end.y, end.z);
	}
};


// --------------------------------------------------------------------------------- 
// The class containing the functionality to create the PVS's
// --------------------------------------------------------------------------------- 
class CVis {

private:
	int fd;
	int verbose;

	int vis_count;
public:
	int amount_points;

	int accuracy;

	// output the list of points, rays etc. into this path
	char path_to_save_nodes[255];

	int first_time;
	int no_calc;
	CLBSP    *bsp_parser;
	char     in_filename[255];
	CBSPNode *root_node;

	CPolygonList *root_p_list;

	CBSPNode **node_list;
	int      node_count;
	
	CVert3D  **normal_list;
	CVert3D  **vertex_list;
	CPlane   **plane_list;

	CVis(char *filename);

	void save_again(char *filename);

	void calculate_vis();
	void calculate_vis_r(CBSPNode *node, CPointSet *p_set);
	CPointSet *distribute_points(CBSPNode *node);
	void clean_up_points(CBSPNode *node, CPointSet *p_set);

	void build_polylist_r(CBSPNode *node, CPolygonList *p_list);
	void build_polylist(CBSPNode *node);

	void create_perpen_planes(CBSPNode *bsp);
	int  check_inside_poly(CPoly *poly, CVert3D point);
	int  check_inside_poly2(CPoly *poly, CVert3D point);

	void calculate_neighbours();
	void calculate_rays();

	void get_node_list(CBSPNode *node, CBSPNode **nodelist, int *count);

	int check_visibility(CBSPNode *node1, CBSPNode *node2);
	int ray_intersects_tree(CRay r);

	float chop_line(CVert3D *v1, CVert3D *v2, CPlane *pl, CVert3D &out);

	int get_vis_count() { return vis_count; }

};

// --------------------------------------------------------------------------------- 
// go through each leaf and check what is visible from it
// --------------------------------------------------------------------------------- 
void CVis::calculate_rays()
{
	// debug output of rays
	if(path_to_save_nodes[0] != '\0') {
		char strbuffer[255];
		sprintf(strbuffer, "%s\\raylist.pts", path_to_save_nodes);
		fd = open(strbuffer, _O_CREAT | _O_TRUNC | _O_WRONLY, _S_IWRITE);
	}

	vis_count = 0;

	for(int q=0;q<node_count;q++) 
		for(int i=0;i<node_count;i++)  {
		
			if(i<=q) continue; 
			
			for(int j=0;j<node_list[q]->num_vis_nodes;j++) {
				if(node_list[q]->vis_node_list[j] == i) goto skip;
			}

			if(verbose)
				printf("[%d,%d] ",q,i);
			else
				printf("\r              \r%d -> %d\r", q, i);
			if(check_visibility(node_list[q], node_list[i])) { // perform the ray tracing
				node_list[q]->vis_node_list[node_list[q]->num_vis_nodes++] = i;
				node_list[i]->vis_node_list[node_list[i]->num_vis_nodes++] = q;
			}
skip:;
	}

	if(verbose)
		printf("\n");
	else 
		printf("Complete.\n");

	if(path_to_save_nodes != '\0') 
		close(fd);
}


// --------------------------------------------------------------------------------- 
// split a line at the point of intersection with a plane
// --------------------------------------------------------------------------------- 
inline float CVis::chop_line(CVert3D *v1, CVert3D *v2, CPlane *pl, CVert3D &out)
{
	CVert3D norm = CVert3D(pl->a, pl->b, pl->c);
	CVert3D vec = *v1 - *v2;

	float start_dist_from_plane = pl->classify(*v1); 
	float projected_line_length = vec*norm;          			

	if(projected_line_length == 0) {
		printf("*** Splitting ERROR: division by zero will occur ***\n");
		return -1;
	}

	float scale = start_dist_from_plane / projected_line_length;			
	out = *v1 - (vec * scale);
	return scale;
}

// --------------------------------------------------------------------------------- 
// check the ray to see if it intersects any polygons within the BSP tree
// --------------------------------------------------------------------------------- 
int CVis::ray_intersects_tree(CRay ray)
{
	int num_polys = root_p_list->count();

	for(int i=0;i<num_polys;i++) {		
		CPoly p   = *root_p_list->get(i);
		if(p.is_static == 1) continue; // ignore static polygons		

		CPlane pl = CPlane(*normal_list[p.ni], *vertex_list[p.vi[0]]);

		if((pl.classify(ray.start) * pl.classify(ray.end)) < 0) {
			// okay the ray crosses the polygon, now have to find intersection point
			CVert3D out;			
			if(chop_line(&ray.start, &ray.end, &pl, out) == -1) return 1;
			if(check_inside_poly(&p, out)) {
				return 1; // ray hit the polygon
			}
		}
	}	
	return 0; // ray hit no polygons
}

// --------------------------------------------------------------------------------- 
// check to see if node2 is visible from node1
// --------------------------------------------------------------------------------- 
int CVis::check_visibility(CBSPNode *node1, CBSPNode *node2)
{
	Vert3DList sp1 = node1->pointset->get_set();
	Vert3DList sp2 = node2->pointset->get_set();

	char strbuffer[500];

	for(Vert3DIterator itor1 = sp1.begin(); itor1 != sp1.end(); itor1++) {
		for(Vert3DIterator itor2 = sp2.begin(); itor2 != sp2.end(); itor2++) {

			// create a ray for each point in node1 to each point in node2
			CRay r = CRay(*itor1, *itor2);			

			if(ray_intersects_tree(r) == 0) {

				if(verbose)
					printf("********************* Visible!!!! *********************\n");

				vis_count++;

				if(fd > 0) {
					sprintf(strbuffer, "%f %f %f %f %f %f\n", r.start.x, r.start.y, r.start.z, r.end.x, r.end.y, r.end.z);
					write(fd, strbuffer, strlen(strbuffer));
				}

				if(verbose)
					r.dump();

				return 1;
			}		
		}
	}	

	return 0;	
}

// --------------------------------------------------------------------------------- 
// create the list of leaves of a BSP tree
// --------------------------------------------------------------------------------- 
void CVis::get_node_list(CBSPNode *node, CBSPNode **nodelist, int *count)
{
	if(node->back==NULL && node->front==NULL) {
		nodelist[(*count)++] = node; // found leaf so add it to list
		return;
	}

	if(node->back != NULL)
		get_node_list(node->back, nodelist, count);
	if(node->front != NULL)
		get_node_list(node->front, nodelist, count);

}

// --------------------------------------------------------------------------------- 
// save the finished BSP tree with vis_node_list's created
// --------------------------------------------------------------------------------- 
void CVis::save_again(char *filename)
{
	// reload the original BSP tree file as computing the PVS's results in
	// unwanted values in the already loaded BSP tree.
	
	CLBSP *bsp_correct = new CLBSP();
	bsp_correct->read_bsp_tree(in_filename);	

	CBSPNode **out_node_list = (CBSPNode **) malloc(sizeof(CBSPNode*) * 1000);
	if(out_node_list == NULL) {
		printf("ERROR: can not allocate memory for node list.\n");
		exit(1);
	}

	int index =0;
	get_node_list(bsp_correct->get_bsp(), out_node_list, &index);
	printf("Number of leaves in BSP tree: %d\n", index);

	for(int i=0;i<index;i++)  {
		out_node_list[i]->num_vis_nodes = node_list[i]->num_vis_nodes;
		out_node_list[i]->vis_node_list = node_list[i]->vis_node_list;
	}

	printf("Saving the BSP tree...\n");
	bsp_correct->write_bsp_tree(filename);
}

// --------------------------------------------------------------------------------- 
// reduce the ray tracing calculations required by computing what leaves are 
// neighbours (i.e. share a point in their pointlists)
// --------------------------------------------------------------------------------- 
void CVis::calculate_neighbours()
{
	printf("Leaf count: %d\n", node_count);

	for(int i=0;i<node_count;i++) {

		CBSPNode *n1          = node_list[i];
		Vert3DList vect_list1 = n1->pointset->get_set();

		for(int j=0;j<node_count;j++) {
			if(i == j) continue;
			CBSPNode *n2          = node_list[j];
			Vert3DList vect_list2 = n2->pointset->get_set();
			for(Vert3DIterator itor1 = vect_list1.begin(); itor1 != vect_list1.end(); itor1++) {
				CVert3D v1 = CVert3D(*itor1);
				for(Vert3DIterator itor2 = vect_list2.begin(); itor2 != vect_list2.end(); itor2++) {
					CVert3D v2 = CVert3D(*itor2);
					if(v1.x == v2.x && v1.y == v2.y && v1.z == v2.z) {
						// node i and node j share a point so they are neighbours and visible to 
						// each other
						n1->vis_node_list[n1->num_vis_nodes++] = j; 
						printf("             \r%d -> %d\r", i,j);
						goto skip;					
					}

				}
			}
			skip:;
		}		
	}
	printf("\n");
}

// --------------------------------------------------------------------------------- 
// recursive function to create the perpendicular planes for every polygon
// --------------------------------------------------------------------------------- 
void CVis::create_perpen_planes(CBSPNode *bsp)
{	
	if(bsp->front == NULL && bsp->back == NULL) {

		for(int i=0;i<bsp->polygons->count();i++) {
			CPoly *poly = bsp->polygons->get(i);
			poly->create_per_pen_plane(vertex_list, normal_list);
		}		

		return;
	}

	if(bsp->front != NULL)
		create_perpen_planes(bsp->front);
	if(bsp->back != NULL)
		create_perpen_planes(bsp->back);		
}


// --------------------------------------------------------------------------------- 
// constructor
// --------------------------------------------------------------------------------- 
CVis::CVis(char *filename)
{
	verbose = 0;
	no_calc = 0;
	bsp_parser = new CLBSP();
	accuracy = 1; //default is medium quality

	path_to_save_nodes[0] = '\0';

	first_time = 1;

	// read the BSP tree into memory
	strcpy(in_filename,filename);
	printf("Reading the BSP tree <%s>...",filename);
	if(!bsp_parser->read_bsp_tree(filename)) {
		printf("Can not parse input file %s\n", filename);
		exit(1);
	}
	printf("Complete.\n");

	normal_list = bsp_parser->get_normal_list();
	vertex_list = bsp_parser->get_vertex_list();
	plane_list  = bsp_parser->get_plane_list();

	node_list = (CBSPNode **) malloc(sizeof(CBSPNode*) * 1000);
	node_count = 0;

	if(node_list == NULL) {
		printf("ERROR: Node list malloc error.\n");
		exit(1);
	}

	root_node = bsp_parser->get_bsp();

	create_perpen_planes(root_node);

	build_polylist(root_node);
	root_p_list = new CPolygonList(root_node->polygons);

#ifdef DEBUG
	c_debug = new CDebug();
	c_debug->debug_init("hvisdebug.dat");
#endif

}

// --------------------------------------------------------------------------------- 
// calculate the visibilty points for each leave of the BSP tree
// --------------------------------------------------------------------------------- 
void CVis::calculate_vis_r(CBSPNode *node, CPointSet *p_set)
{
	if(p_set != NULL) {
		if(verbose) {
			printf("Cleaning up points ---------------------------------\n");
			printf(" Before cleanup there are %d points.\n", p_set->list.size());
		}

		clean_up_points(node, p_set);

		if(verbose)
			printf(" After cleanup there are %d points.\n", p_set->list.size());
	}

	if(node->back == NULL && node->front == NULL) {	
		if(verbose) {
			printf("*** Node Complete and contains %d points. ***\n", p_set->list.size());
		} else {
			printf("*");
		}
		node_list[node_count++] = node; // create node list at the same time!
		node->pointset = new CPointSet(p_set);
		delete p_set;
		return;
	}

	CPointSet *new_points = distribute_points(node);
	CPointSet *right = new CPointSet(new_points);
	CPointSet *left  = new CPointSet(new_points);

	if(p_set != NULL) {
		Vert3DList point_list = p_set->list;

		CPlane pl = CPlane(*normal_list[node->divider->ni], *vertex_list[node->divider->vi[0]]);

		for(Vert3DIterator itor = point_list.begin(); itor != point_list.end(); itor++) {

			CVert3D v = CVert3D(*itor);

			float test = pl.classify(v);
		
			if(test >= -1 && test <= 1) {
				right->add_point(*itor);
				left->add_point(*itor);
			} else if(test > 1) {
				right->add_point(*itor);
			} else if(test < -1) {
				left->add_point(*itor);
			}
			
		}
	
	}

	
	if(node->back != NULL) {
		calculate_vis_r(node->back, left);
	} 
	if(node->front != NULL) {
		calculate_vis_r(node->front, right);
	}	
}

// --------------------------------------------------------------------------------- 
// calls the above function with the root of the BSP tree
// --------------------------------------------------------------------------------- 
void CVis::calculate_vis()
{

	calculate_vis_r(root_node, NULL);

	if(verbose)
		printf("All the points are distributed now...we got to find which nodes are next to each other.\n");	

}

// --------------------------------------------------------------------------------- 
// build a list of all the polygons in the BSP tree, it speeds up ray-plane 
// collision checking
// --------------------------------------------------------------------------------- 
void CVis::build_polylist_r(CBSPNode *node, CPolygonList *p_list)
{
	if(node->back == NULL && node->front == NULL) {
		for(int i=0;i<node->polygons->count();i++) {			
			CPoly *p = node->polygons->get(i);
			if(p->is_static == 1) continue;

			p_list->add(p);
		}
		return;
	}

	if(node->back != NULL)
		build_polylist_r(node->back, p_list);
	if(node->front != NULL)
		build_polylist_r(node->front, p_list);
}

// --------------------------------------------------------------------------------- 
// simply function which calls the above
// --------------------------------------------------------------------------------- 
void CVis::build_polylist(CBSPNode *node)
{
	if(node->polygons->count() != 0)
		return;

	build_polylist_r(node, node->polygons);

}

// --------------------------------------------------------------------------------- 
// if point is within perpendicular bounds of poly reutrn 1 else 0
// --------------------------------------------------------------------------------- 
inline int CVis::check_inside_poly(CPoly *poly, CVert3D point)
{
	// check against per-pen-planes

	// was <= 1 when everyhting nearly working
	float a = poly->p1.classify(point);// + c_radius;
	if(a < 0) return 0;
	float b = poly->p2.classify(point);// + c_radius;
	if(b < 0) return 0;
	float c = poly->p3.classify(point);// + c_radius;
	if(c < 0) return 0;

	return 1; // polygon was hit

}

// --------------------------------------------------------------------------------- 
// removes points from the pointset that are outside the bounding box of the node,
// or coplanar and within the bounds of any polygon in the leaves below the node
// being processed.
// --------------------------------------------------------------------------------- 
void CVis::clean_up_points(CBSPNode *node, CPointSet *p_set)
{	
	if(p_set == NULL)         return;

	Vert3DList point_list = p_set->get_set();

	for(Vert3DIterator itor = point_list.begin(); itor != point_list.end();) {
		Vert3D p = (*itor);

		// remove points outside of bounding box
		if( (p.x > node->bb_max.x+0.1) || // +
			(p.y > node->bb_max.y+0.1) ||
			(p.z > node->bb_max.z+0.1) ||
			(p.x < node->bb_min.x-0.1) ||
			(p.y < node->bb_min.y-0.1) ||
			(p.z < node->bb_min.z-0.1) ) {
			point_list.erase(itor++);
		} else {
			itor++;
		}
	}

	node->pointset = p_set;

	if(node->front == NULL && node->back == NULL) {
		CPolygonList *p_list = node->polygons;

		int num_polys = p_list->count();

		if(verbose)
			printf("*** Clean up points is checking %d polygons. ***\n", num_polys);

		for(int i=0;i<num_polys;i++) {
	
			CPoly p   = p_list->get(i);			
			if(p.is_static == 1) continue;
			for(itor = point_list.begin(); itor != point_list.end();) {
				Vert3D v = (*itor);		
			
				float test = plane_list[p.plane_id]->classify(CVert3D(v));

				if( (test >= 0.01) ) {			
					itor++;
					continue; // removes all verts outside of node
				}

				if(check_inside_poly( &p, CVert3D(v) ) ) {
					point_list.erase(itor++); // if point is within bounds of polygon remove it
				} else {
					itor++;				
				}
			}	
		}
	}

	p_set->list = point_list;
	node->pointset = p_set;

}

// --------------------------------------------------------------------------------- 
// distribute points along the dividing plane at a set interval
// --------------------------------------------------------------------------------- 
CPointSet *CVis::distribute_points(CBSPNode *node)
{
	CPointSet *p_set = new CPointSet();	

	CVert3D norm = *normal_list[node->divider->ni];
	CVert3D anorm;
	int     planemap = -1;
	float   width;
	float   height;
	float   umin, umax;
	float   vmin, vmax;

	// compute the prominent axes of the dividing plane
	anorm.x = (float) fabs(norm.x);
	anorm.y = (float) fabs(norm.y);
	anorm.z = (float) fabs(norm.z);

	if(verbose)
		printf("Normal is %f,%f,%f\n", norm.x, norm.y, norm.z);

	CPlane plane = CPlane(norm, *vertex_list[node->divider->vi[0]]);

	if(anorm.x >= anorm.y && anorm.x >= anorm.z) {
		if(verbose)
			printf("Distributing on YZ plane\n");
		planemap = 1;
	} else if(anorm.y > anorm.x && anorm.y > anorm.z) {
		if(verbose)
			printf("Distributing on XZ plane\n");
		planemap = 2;
	} else if(anorm.z > anorm.x && anorm.z > anorm.y) {
		if(verbose)	
			printf("Distributing on XY plane\n");
		planemap = 3;
	} else {
		printf("Problem with plane\n");
		printf("Problem with Plane: %f, %f, %f\n", norm.x, norm.y, norm.z);
		exit(1);
	}

	float du,dv;

	// set maximum/minimum u and v values
	switch(planemap) {
	case 1:
		umin   = node->bb_min.y;
		umax   = node->bb_max.y;
		vmin   = node->bb_min.z;
		vmax   = node->bb_max.z;
		break;
	case 2:
		umin   = node->bb_min.x;
		umax   = node->bb_max.x;
		vmin   = node->bb_min.z;
		vmax   = node->bb_max.z;
		break;
	case 3:
		umin   = node->bb_min.x;
		umax   = node->bb_max.x;
		vmin   = node->bb_min.y;
		vmax   = node->bb_max.y;
		break;
	}	

	// calculate gap between points
	width  = umax - umin;
	height = vmax - vmin;
	du = width / (float)amount_points;
	dv = height / (float)amount_points;

	CVert3D point;	
	int point_count = 0;
	
	float savedv = vmin;

	// distrbute the points 
	for(int u = 0; u < amount_points; u++) {
		vmin = savedv;
		for(int v = 0; v< amount_points; v++) {
			
			if(planemap == 1) {
				float X = -(plane.c * vmin + plane.b * umin + plane.d) / plane.a;
				point = CVert3D(X, umin, vmin);
			} else
			if(planemap == 2) {
				float Y = -(plane.a * umin + plane.c * vmin + plane.d) / plane.b;
				point = CVert3D(umin, Y, vmin);
			} else
			if(planemap == 3) {
				float Z = -(plane.a * umin + plane.b * vmin + plane.d) / plane.c;
				point = CVert3D(umin, vmin, Z);
			}
			vmin+=dv;

			Vert3D p;
			point.get_floats(&p);

			p_set->add_point(p);

			point_count++;
		}
		umin+=du;
	}

	if(verbose)
		printf("\n\nDistributed %d points for this plane.\n", point_count);
	return p_set;
}

// --------------------------------------------------------------------------------- 
// main, starting point for executable
// --------------------------------------------------------------------------------- 
__cdecl main(int argc, char **argv)
{
	printf("\nHorizon visibility generator, (C) Nicholas Kennedy 2003\n\n");
	if(argc <= 2) {
		printf("Usage: inputfile.bsp outfile.vis <quality> <pathfordebuginfo>\n\n");
		printf("       quality can be a value from 1 to 3 which indicates the\n");
		printf("       level of accuracy of the visible node lists created.\n\n");
		printf("       0 = fast processing/low accuracy, 1 = medium/medium 2 = long/high\n");
		printf("\n\n");
		return 0;
	}

	printf("Updated BSP file will be stored in: %s\n", argv[2]);

	CVis *c_vis = new CVis(argv[1]);

	if(argc >= 4) {
		c_vis->accuracy = atoi(argv[3]);
	}

	if(argc >= 5) {
		strcpy(c_vis->path_to_save_nodes, argv[4]);
	}

	switch(c_vis->accuracy) {
	case 0:
		printf("Using low accuracy calculations.\n");
		break;
	case 1:
		printf("Using medium accuracy calculations.\n");
		break;
	case 2:
		printf("Using high accuracy calculations.\n");
		break;
	case 3:
		printf("Using ultra-high accuracy calculations.\n");
		break;
	default:
		printf("Error with accuracy so defaulting to medium accuracy calculations.\n");
		c_vis->accuracy = 1;
	}

	printf("Starting the visibility calculations now...\n");
	// save start time 
	float complete_start = GetTickCount();

	c_vis->amount_points = 10;
	
	// go around the while loop increase the number of points distributed along the dividing planes
	// until the maximum value of 40 is reached or the quality wanted is reached.
	while(1) {

		c_vis->node_count = 0;

		printf("- Distributing %dx%d points now...", c_vis->amount_points, c_vis->amount_points);
		float start_time = GetTickCount();
		c_vis->calculate_vis();		
		float end_time = GetTickCount();
		printf(" %.3f mins.\n", ((end_time - start_time) / 1000.0f)/60.0f);

		printf("Computing neighbour nodes...\n");
		start_time = GetTickCount();
		c_vis->calculate_neighbours();
		end_time = GetTickCount();
		printf("Computing neighbour nodes took %.3f mins.\n", ((end_time - start_time) / 1000.0f)/60.0f);

		printf("Visibility ray-tracing now...\n");
		start_time = GetTickCount();
		c_vis->calculate_rays();
		end_time = GetTickCount();
		printf("Found %d visible leaves.\n",c_vis->get_vis_count());
		printf("Ray tracing took %.3f mins\n", ((end_time - start_time) / 1000.0f)/60.0f);

		if(c_vis->amount_points == 10 && c_vis->accuracy == 0)
			break;

		if(c_vis->amount_points == 20 && c_vis->accuracy == 1)			
			break;

		if(c_vis->amount_points == 40 && c_vis->accuracy == 2)
			break;
		
		if(c_vis->amount_points == 50 && c_vis->accuracy == 3)
			break;
		
		if(c_vis->amount_points == 40 && c_vis->accuracy > 2)
			c_vis->amount_points = 50;

		if(c_vis->amount_points == 20 && c_vis->accuracy > 1)
			c_vis->amount_points = 40;

		if(c_vis->amount_points == 10 && c_vis->accuracy > 0)
			c_vis->amount_points = 20;			

	}

	if(c_vis->path_to_save_nodes[0] != '\0') {
		// write out the node points
		char strbuffer[255];

		for(int i=0;i<c_vis->node_count;i++) {
			sprintf(strbuffer, "%s\\node%d.pts", c_vis->path_to_save_nodes, i);
			int fd = open(strbuffer, _O_CREAT | _O_TRUNC | _O_WRONLY, _S_IWRITE);
			if(fd<=0) continue;

			CBSPNode *node = c_vis->node_list[i];
			Vert3DList point_list = node->pointset->get_set();

			sprintf(strbuffer, "%d\n", point_list.size());
			write(fd, strbuffer, strlen(strbuffer));
			for(Vert3DIterator itor = point_list.begin(); itor != point_list.end();) {
				Vert3D v = (*itor++);			
				sprintf(strbuffer, "%f %f %f\n", v.x, v.y, v.z);
				write(fd, strbuffer, strlen(strbuffer));
			}	
			close(fd);		
		}
	}


	c_vis->save_again(argv[2]);

	float complete_end = GetTickCount();
	printf("\n\n*** Total time %.1f mins ***\n", ((complete_end - complete_start) / 1000.0f)/60.0f);

	return 0;
}