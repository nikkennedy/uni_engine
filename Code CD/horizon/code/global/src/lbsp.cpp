/*	--------------------------------------------------------------------------------- 
	Filename      - lbsp.cpp
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Implementation of CLBSP (leafy BSP)
					This is probably the most fundamental file as its used by almost
					every other, it contains the functions to read, write and calculate
					leafy bsp trees
	--------------------------------------------------------------------------------- */

#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <lbsp.h>
#include <types.h>
#include <maths.h>
#include <debug.h>
#include <vert3d.h>
#include <p_list.h>

// --------------------------------------------------------------------------------- 
// constructor
// --------------------------------------------------------------------------------- 
CLBSP::CLBSP() 
{
	material_list = NULL;
	vertex_list = NULL;
	tverts_list = NULL;
	normal_list = NULL;
	light_list = NULL;
	divider_indexs = NULL;
	bsp_tree = NULL;
}

// --------------------------------------------------------------------------------- 
// free mem
// --------------------------------------------------------------------------------- 
CLBSP::~CLBSP() 
{

	free(material_list);
		
	if(vertex_list != NULL) {
		for(int i=0;i<num_vertexs;i++) {
			delete vertex_list[i];
		}
		free(vertex_list);
	}

	if(tverts_list != NULL) {
		for(int i=0;i<num_tverts;i++) {
			delete tverts_list[i];
		}
		free(tverts_list);
	}

	if(normal_list != NULL) {
		for(int i=0;i<num_normals;i++) {
			delete normal_list[i];
		}
		free(normal_list);
	}
		
	for(int i=0;i<num_planes;i++) {
		delete plane_list[i];
	}

	if(light_list != NULL) {
		for(int i=0;i<num_lights;i++) {
			delete light_list[i];
		}
		free(light_list);
	}

	if(divider_indexs != NULL) {
		free(divider_indexs);	
		for(int i=0;i<next_divider-1;i++) {
			free(dividers[i]);
		}
	}

	if(bsp_tree != NULL) {
		// recursive delete 
		delete bsp_tree;
	}
}

// --------------------------------------------------------------------------------- 
// build up a list of all the leaves 
// --------------------------------------------------------------------------------- 
void CLBSP::create_node_list(CBSPNode *node, CBSPNode **nodelist, int *count)
{
	if(node->back==NULL && node->front == NULL) {
		nodelist[(*count)++] = node;
		return;
	}

	if(node->back!=NULL)
		create_node_list(node->back, nodelist, count);
	if(node->front!=NULL)
		create_node_list(node->front,nodelist,count);
}

// --------------------------------------------------------------------------------- 
// calculate which leaf the position loc would be in
// --------------------------------------------------------------------------------- 
inline CBSPNode *CLBSP::check_node(CBSPNode *bsp, CVert3D loc, int &node_counter)
{
	if(bsp->front == NULL && bsp->back == NULL) {
		return bsp;
	}

	node_counter++;

	CPlane *pl           = plane_list[bsp->divider->plane_id];
	float classification = pl->classify(loc); 

	if(classification > 0 )  {
		if(bsp->front != NULL) {
			return check_node(bsp->front, loc, node_counter);
		} else if(bsp->back != NULL)
			return check_node(bsp->back, loc,node_counter);
		

	} else if(classification < 0 || classification == 0) {
		if(bsp->back != NULL) {
			return check_node(bsp->back, loc,node_counter);
		} else if(bsp->front != NULL)
			return check_node(bsp->front, loc,node_counter);
	} 

	return NULL;
}

// --------------------------------------------------------------------------------- 
// uses the above function to calculate which leaf loc would be in 
// --------------------------------------------------------------------------------- 
CBSPNode *CLBSP::which_node(CVert3D loc, int &node_counter)
{
	node_counter = 0;
	return check_node(bsp_tree, loc, node_counter);
}

// --------------------------------------------------------------------------------- 
// given a line and a plane, this function calculates the intersection point
// --------------------------------------------------------------------------------- 
inline float CLBSP::chop_line(CVert3D *v1, CVert3D *v2, CPlane pl, CVert3D &out)
{
	CVert3D norm = CVert3D(pl.a, pl.b, pl.c);
	CVert3D vec = *v1 - *v2;

	float start_dist_from_plane = pl.classify(*v1); 
	float projected_line_length = vec*norm;          			
	if(projected_line_length == 0) {
		dbp("Splitting ERROR: division by zero will occur\n");
		exit(0);
	}
	float scale = start_dist_from_plane / projected_line_length;			
	out = *v1 - (vec * scale);
	return scale;
}

// --------------------------------------------------------------------------------- 
// fundamental function needed to create a bsp tree, it takes a plane and a polygon
// splitting the polygon along the plane to create two or more polygons, one set
// thats infront of the plane and one thats behind.
// --------------------------------------------------------------------------------- 
int CLBSP::split_polygon(CPoly *divider, CPoly *in, CPoly *front, CPoly *back)
{
	CVert3D frontv[10], backv[10];   // list of verts	
	CVert3D fronttv[10], backtv[10]; // list of tverts
	int nofv = 0, nobv = 0;         // number of verts
	int noftv = 0, nobtv = 0;       // number of tverts
	float side1, side2;             // classification of point
	int v1 = 0, v2 = 1;             // current and next vert index
	int ret = 0;
	CVert3D tvec;                    // texture vertor

	//CPlane *plane = new CPlane(*normal_list[divider->ni], *vertex_list[divider->vi[0]]);
	CPlane plane = CPlane(*normal_list[divider->ni], *vertex_list[divider->vi[0]]);

	for(int i=0;i<3;i++) { // once for each vertex

		side1 = plane.classify(*vertex_list[in->vi[v1]]);
		side2 = plane.classify(*vertex_list[in->vi[v2]]); //classify_point(plane, vertex_list[in->vi[v2]]);

		if(side1 >= EPSILON && side2 >= EPSILON) { // both infront			
			frontv[nofv++]   = *vertex_list[in->vi[v1]];
			fronttv[noftv++] = *tverts_list[in->ti[v1]];
			v1 = v2;
			v2 = ((v2+1) % 3);

                       // v1 is infront v2 is behind          // v1 is behind and v2 is infront
		} else if( (side1 >= EPSILON && side2 < EPSILON) || (side1 < EPSILON && side2 >= EPSILON) ) {

			if(side1 >= EPSILON && side2 < EPSILON) {
				frontv[nofv++]   = *vertex_list[in->vi[v1]]; // add the first vertex if v1 is infront
				fronttv[noftv++] = *tverts_list[in->ti[v1]]; // add the first vertex if v1 is infront
			}

			// do the vertex -------------------------------------------------------
			float scale = chop_line(vertex_list[in->vi[v1]], vertex_list[in->vi[v2]], plane, frontv[nofv++]);
			
			// do the texture vertex -----------------------------------------------
			tvec = *tverts_list[in->ti[v1]] - *tverts_list[in->ti[v2]];

			fronttv[noftv] = *tverts_list[in->ti[v1]] - (tvec * scale);

			noftv++;

			v1 = v2;
			v2 = ((v2+1) % 3);

		} else if(side1 < EPSILON && side2 < EPSILON) { // both behind
			v1 = v2;
			v2 = ((v2+1) % 3);
		}
	} // front has been created

	dbp("SPLIT occured, front polygon has %d verts\n", nofv);

	if(nofv == 4) {
		dbp("Need to split front quad into 2 tris\n");

		int starti  = num_vertexs;
		int startti = num_tverts;

		vertex_list[num_vertexs++] = new CVert3D(frontv[0]);
		vertex_list[num_vertexs++] = new CVert3D(frontv[1]);
		vertex_list[num_vertexs++] = new CVert3D(frontv[2]);
		vertex_list[num_vertexs++] = new CVert3D(frontv[3]);

		tverts_list[num_tverts++] = new CVert3D(fronttv[0]);
		tverts_list[num_tverts++] = new CVert3D(fronttv[1]);
		tverts_list[num_tverts++] = new CVert3D(fronttv[2]);
		tverts_list[num_tverts++] = new CVert3D(fronttv[3]);

		front[0]             = CPoly(starti,   starti+1, starti+3, startti,   startti+1, startti+3);
		front[1]             = CPoly(starti+1, starti+2, starti+3, startti+1, startti+2, startti+3);
		front[0].ni          = in->ni;
		front[1].ni          = in->ni;
		front[0].material_id = in->material_id;
		front[1].material_id = in->material_id;
		front[0].is_static = in->is_static;
		front[0].lightmap_id = in->lightmap_id;
		front[1].lightmap_id = in->lightmap_id;
		front[1].is_static = in->is_static;

		ret |= FRONT_IS_TWO;
	} else {
		ret |= FRONT_IS_ONE;
		// copy and store polygon
		for(int j=0;j<3;j++) {	
			vertex_list[num_vertexs] = new CVert3D(frontv[j]);
			front[0].vi[j] = num_vertexs++;
			tverts_list[num_tverts] = new CVert3D(fronttv[j]);
			front[0].ti[j] = num_tverts++;			
		}
		front[0].ni = in->ni;
		front[0].material_id = in->material_id;
		front[0].is_static = in->is_static;
		front[0].lightmap_id = in->lightmap_id;

	}

	if(nofv > 4) {
		dbp("Error too many front verts created.\n");
		return 0;
	}

	if(nofv == 0) {
		ret |= FRONT_IS_NONE;
		dbp("Front has no polygons!\n");
		exit(1);
	}
	
	// now grab the behind polygon	
	v1 = 0; v2 = 1;

	for(i=0;i<3;i++) {
		side1 = plane.classify(*vertex_list[in->vi[v1]]);
		side2 = plane.classify(*vertex_list[in->vi[v2]]);

		if(side1 >= EPSILON && side2 >= EPSILON) { // both are infront so ignore
			v1 = v2;
			v2 = ((v2+1) % 3);
		} else if( (side1 >= EPSILON && side2 < EPSILON) || (side1 < EPSILON && side2 >= EPSILON) ) {
			if(side1 < EPSILON && side2 >= EPSILON)  {
				backv[nobv++]   = *vertex_list[in->vi[v1]]; // include this vert 
				backtv[nobtv++] = *tverts_list[in->ti[v1]]; // include this vert 
			}

			float scale = chop_line(vertex_list[in->vi[v1]], vertex_list[in->vi[v2]], plane, backv[nobv++]);

			// do the texture vertex -----------------------------------------------
			tvec            = *tverts_list[in->ti[v1]] - *tverts_list[in->ti[v2]];
			backtv[nobtv++] = *tverts_list[in->ti[v1]] - (tvec * scale);

			v1 = v2;
			v2 = ((v2+1) % 3);
		} else if(side1 < EPSILON && side2 < EPSILON) { // both are behind
			backv[nobv++]   = *vertex_list[in->vi[v1]];
			backtv[nobtv++] = *tverts_list[in->ti[v1]];
			v1 = v2;
			v2 = ((v2+1) % 3);
		}
	} // back was created

	dbp("SPLIT occured, back polygon has %d verts\n", nobv);

	if(nobv == 4) {
		dbp("Need to split back quad into 2 tris\n");
		ret |= BACK_IS_TWO;

		int starti = num_vertexs;
		int startti = num_tverts;

		vertex_list[num_vertexs++] = new CVert3D(backv[0]);
		vertex_list[num_vertexs++] = new CVert3D(backv[1]);
		vertex_list[num_vertexs++] = new CVert3D(backv[2]);
		vertex_list[num_vertexs++] = new CVert3D(backv[3]);

		tverts_list[num_tverts++] = new CVert3D(backtv[0]);
		tverts_list[num_tverts++] = new CVert3D(backtv[1]);
		tverts_list[num_tverts++] = new CVert3D(backtv[2]);
		tverts_list[num_tverts++] = new CVert3D(backtv[3]);

		back[0]             = CPoly(starti,   starti+1, starti+3, startti,   startti+1, startti+3);
		back[1]             = CPoly(starti+1, starti+2, starti+3, startti+1, startti+2, startti+3);
		back[0].ni          = in->ni;
		back[1].ni          = in->ni;
		back[0].material_id = in->material_id;
		back[1].material_id = in->material_id;
		back[0].is_static = in->is_static;
		back[1].is_static = in->is_static;
		back[0].lightmap_id = in->lightmap_id;
		back[1].lightmap_id = in->lightmap_id;

	} else {
		ret |= BACK_IS_ONE;
		// copy and store polygon
		for(int j=0;j<3;j++) {
			vertex_list[num_vertexs] = new CVert3D(backv[j]);
			tverts_list[num_tverts]  = new CVert3D(backtv[j]);
			back[0].vi[j]            = num_vertexs++;
			back[0].ti[j]            = num_tverts++;
		}
		back[0].ni = in->ni;
		back[0].material_id = in->material_id;
		back[0].is_static = in->is_static;
		back[0].lightmap_id = in->lightmap_id;
	}
	if(nobv > 4) {
		dbp("Error too many back verts created\n");
		return 0;
	}
	
	if(nobv == 0) {
		ret |= BACK_IS_NONE;
		dbp("Back has no polygons!\n");
		exit(1);
	}

	return ret;
}

// --------------------------------------------------------------------------------- 
// is poly p1 infront of poly p2
// --------------------------------------------------------------------------------- 
inline int CLBSP::in_front(CPoly *p1, CPoly *p2)
{
	CPlane p1_plane = CPlane(*normal_list[p1->ni], *vertex_list[p1->vi[0]]); 
	int coincedent = 0, infront = 0, behind = 0;

	for(int i=0;i<3;i++) { // check each vertex
		float test = p1_plane.classify(*vertex_list[p2->vi[i]]);

		if(test > -EPSILON && test < EPSILON) coincedent++;
		if(test < -EPSILON) behind++;
		if(test >= EPSILON) infront++;
	}


	if(infront > 0 && behind== 0) return INFRONT;
	if(infront ==0 && behind > 0) return BEHIND;
	if(infront ==0 && behind == 0) return COINCEDENT;	
	
	return SPANNING;
}

// --------------------------------------------------------------------------------- 
// is a list of polygons convex
// --------------------------------------------------------------------------------- 
inline int CLBSP::is_convex(CPolygonList *p_list)
{
	int num_polys = p_list->count();
	int infront;
	
	// make sure that each poly is infront of each other (or coincedent)
	for(int i=0;i<num_polys;i++) {

		if(p_list->get(i)->is_static != 1) { // IGNORE static polys

			for(int j=0;j<num_polys;j++) {
			
				if(i != j && p_list->get(j)->is_static != 1) {

					infront = in_front(p_list->get(i), p_list->get(j));

					if(infront != INFRONT && infront != COINCEDENT && !is_divider(p_list->get(i)) && !is_divider(p_list->get(j))) return 0;
				}

			}
		}
	}
	return 1;
}

// --------------------------------------------------------------------------------- 
// compare two polygons see if they match
// --------------------------------------------------------------------------------- 
inline int CLBSP::compare_polys(CPoly *p1, CPoly *p2)
{
	for(int i=0;i<3;i++) {
		if(*vertex_list[p1->vi[i]] != *vertex_list[p2->vi[i]]) 
			return 0;
	}

	return 1;	
}

// --------------------------------------------------------------------------------- 
// is polygon p1 a divider?
// --------------------------------------------------------------------------------- 
inline int CLBSP::is_divider(CPoly *p1)
{
	for(int i=0;i<next_divider;i++) {
		if(compare_polys(p1, dividers[i]))
			return 1;
	}
	
	return 0;
}


// --------------------------------------------------------------------------------- 
// there are two choices when picking the next dividing plane, the one that causes
// the least splits or the one that produces the most balanced tree.
// --------------------------------------------------------------------------------- 
//#define DIVIDE_CHOICE_MIN_SPLIT 
#define DIVIDE_CHOICE_BALANCED  

// --------------------------------------------------------------------------------- 
// get the next dividing polygon
// --------------------------------------------------------------------------------- 
CPoly *CLBSP::get_dividing_polygon(CPolygonList *p_list)
{
	int best_polygon = -1;
	CPoly *p;

#ifdef DIVIDE_CHOICE_BALANCED
	float best_relation = 0;
	float relation;
	int   least_splits  = 5000000;	
	float min_relation = 1.0f;
	int split;
	int behind;
#endif


	if(is_convex(p_list)) {
		dbp("Convect set\n");
		return NULL;
	}

#ifdef DIVIDE_CHOICE_BALANCED
	while(best_polygon == -1) {

		int num_polys = p_list->count();

		for(int i=0;i<num_polys;i++) {

			if(p_list->get(i)->is_static==1) continue;

			if(!is_divider(p_list->get(i)) && p_list->get(i)->is_static != 1) {
				split = 0;
				behind = 0;
				front = 0;
				for(int j=0;j<p_list->count();j++) {

					if(p_list->get(j)->is_static == 1) continue;

					if(i != j) {
						int side = in_front(p_list->get(i), p_list->get(j));					
						if(side == SPANNING)
							split++;
						if(side == INFRONT)
							front++;
						if(side == BEHIND  || side == COINCEDENT)
							behind++;
					}
				}

				if(front > behind)
					relation = ((float)behind/(float)front);
				else
					relation = ((float)front/(float)behind);

				if( (relation >= min_relation && split < least_splits) || (split == least_splits && relation > best_relation)) {
					best_polygon = i;
					//min_relation = relation;
					best_relation = relation;
					least_splits = split;
				}
			}

		}
		min_relation /= 2;
	}
#endif

#ifdef DIVIDE_CHOICE_MIN_SPLIT 

	int min_split = 1000000;
	int split;
	int front;
	int behind;

	while(best_polygon == -1) {

		int num_polys = p_list->count();

		for(int i=0;i<num_polys;i++) {

			int is_divid = is_divider(p_list->get(i));
			if(!is_divider(p_list->get(i)) && p_list->get(i)->is_static != 1) {
				split = 0;
				behind = 0;
				front = 0;
				for(int j=0;j<p_list->count();j++) {

					if(i != j) {
						int side = in_front(p_list->get(i), p_list->get(j));					
						if(side == SPANNING)
							split++;
						if(side == INFRONT)
							front++;
						if(side == BEHIND)
							behind++;
					}
				}

				if(split < min_split) {
					min_split = split;
					best_polygon = i;
				}
			}
		}
	}
#endif

	p = new CPoly(p_list->get(best_polygon));

	return p;
}

// --------------------------------------------------------------------------------- 
// give this function a list of polygons and out pops a bsp tree
// --------------------------------------------------------------------------------- 
int CLBSP::create_bsp_tree(int num_polygons, CPoly **polygons)
{
	bsp_p_list = new CPolygonList(num_polygons, polygons);	
	
	skipping_static_poly_count = 0;

	if((divider_indexs = (int *) malloc(sizeof(int) * num_polygons)) == NULL) {
		printf("Can not allocate memory for divider indexs\n");
		return 0;
	}

	memset(divider_indexs, 0, sizeof(int) * num_polygons);
	next_divider = 0;

	num_planes = 0; // these are generated when bsp is saved

	bsp_tree = new CBSPNode();

	CPolygonList *p_list = new CPolygonList(num_polygons, polygons);
	
	generate_bsp_tree(bsp_tree, p_list);

	// now need to push down the static polygons

	for(int i=0;i<bsp_p_list->count();i++) {

		CPoly *p = bsp_p_list->get(i);

		if(p->is_static == 1) {
			CBSPNode *n1 = poly_which_node(bsp_tree, p, 1);
			CBSPNode *n2 = poly_which_node(bsp_tree, p, 0);
			//printf("adding static poly\n");
			if(n1 == n2) {
				n1->polygons->add(p);
			} else {
				n1->polygons->add(p);
				n2->polygons->add(p);
			}
		}
	}

	printf("BSP Tree Created\n");

	return 1;
}

// --------------------------------------------------------------------------------- 
// debug function to write polygon to debug file
// --------------------------------------------------------------------------------- 
void CLBSP::debug_dump_poly(CPoly *p)
{
	for(int i=0;i<3;i++) {
		dbp("V%d: %f %f %f\n", i, vertex_list[p->vi[i]]->x, vertex_list[p->vi[i]]->y, vertex_list[p->vi[i]]->z);
		dbp("T%d: %f %f %f\n", i, tverts_list[p->ti[i]]->x, tverts_list[p->ti[i]]->y, tverts_list[p->ti[i]]->z);
	}
}

// --------------------------------------------------------------------------------- 
// searches through each node, if a polygon spans a divider and frontorback 
// is 1 then the path continues down the front path, otherwise it continues
// down the back path. Used when adding static polygons to the tree
// --------------------------------------------------------------------------------- 
CBSPNode * CLBSP::poly_which_node(CBSPNode *bsp, CPoly *p, int frontorback)
{
	if(bsp->front == NULL && bsp->back == NULL) {
		// return the leaf node the polygon is in
		return bsp;
	}

	int value = in_front(bsp->divider, p);

	if(value == INFRONT || value == COINCEDENT) {
		if(bsp->front != NULL)
			return poly_which_node(bsp->front, p, frontorback);		
		else if(bsp->back != NULL)
			return poly_which_node(bsp->back, p, frontorback);		
	} 
	if(value == BEHIND) {
		if(bsp->back != NULL)
			return poly_which_node(bsp->back, p, frontorback);
		else if(bsp->front != NULL)
			return poly_which_node(bsp->front, p, frontorback);

	}

	if(value == SPANNING) {
		if(frontorback == 1) {
			if(bsp->front != NULL)
				return poly_which_node(bsp->front, p, frontorback);
			else if(bsp->back != NULL)
				return poly_which_node(bsp->back, p, frontorback);

		} else {
			if(bsp->back != NULL)
				return poly_which_node(bsp->back, p, frontorback);
			else if(bsp->front != NULL)
				return poly_which_node(bsp->front, p, frontorback);
		}
	}

	return NULL;	
}

// --------------------------------------------------------------------------------- 
// generates the bsp tree, goes through each list of polygons starting with every
// polygon being considered, then chooses divider. splits the list of polygons into
// those that are in front of the divider and those behind, any polygons spanning 
// the divider are split. The resulting front and back lists are then recursively 
// processed until the polygons within the list create a convex polyhedra
// --------------------------------------------------------------------------------- 
int CLBSP::generate_bsp_tree(CBSPNode *bsp_node, CPolygonList *p_list)
{
	CPolygonList *front_p_list = new CPolygonList();
	CPolygonList *back_p_list = new CPolygonList();

	bsp_node->polygons = p_list;
	bsp_node->divider  = get_dividing_polygon(p_list);
	bsp_node->front    = NULL;
	bsp_node->back     = NULL;

	if(bsp_node->divider == NULL) return 1;

	front = back = spanning = 0;

	for(int i=0;i<p_list->count();i++) {

		int value = in_front(bsp_node->divider, p_list->get(i));

		if(p_list->get(i)->is_static == 1) {
			skipping_static_poly_count++;
			continue;
		}

		if(value == INFRONT || value == COINCEDENT) {
			front++;
			front_p_list->add(p_list->get(i));
		}
		if(value == BEHIND) {
			back++;
			back_p_list->add(p_list->get(i));
		}
		if(value == SPANNING) {
			spanning++;
			
			CPoly poly_front[2], poly_back[2];

			int result = split_polygon(bsp_node->divider, p_list->get(i), poly_front, poly_back);
			if(!(result & FRONT_IS_NONE)) front_p_list->add(&poly_front[0]);
			if(!(result & BACK_IS_NONE))back_p_list->add(&poly_back[0]);
			front++;
			back++;

#ifdef DEBUG
			dbp("Front 1:\n");
			debug_dump_poly(&poly_front[0]);
			dbp("Back 1:\n");
			debug_dump_poly(&poly_back[0]);
#endif

			if(result & FRONT_IS_TWO) {
#ifdef DEBUG
				dbp("Front 2:\n");
				debug_dump_poly(&poly_front[1]);
#endif
				front_p_list->add(&poly_front[1]);
				front++;
			}
			if(result & BACK_IS_TWO) {
#ifdef DEBUG
				dbp("Back 2:\n");
				debug_dump_poly(&poly_back[1]);
#endif
				back_p_list->add(&poly_back[1]);
				back++;
			}
			
			
		}
	}
#ifdef DEBUG
	dbp("Number spanning = %d\n", spanning);
	dbp("Number front    = %d\n", front);
	dbp("Number back     = %d\n", back);
#endif

	dividers[next_divider++] = bsp_node->divider;


	if(front_p_list->count() > 0) {
		bsp_node->front = new CBSPNode();
		generate_bsp_tree(bsp_node->front, front_p_list);
	}

	if(back_p_list->count() > 0) {
		bsp_node->back = new CBSPNode();
		generate_bsp_tree(bsp_node->back, back_p_list);
	}

	return 1;	
}

// --------------------------------------------------------------------------------- 
// set the vertex list to vl and copy all the vertices 
// --------------------------------------------------------------------------------- 
void CLBSP::set_vertex_list(CVert3D **vl)
{
	if(num_vertexs <= 0) {
		printf("Make sure you set vertex count first\n");
		exit(0);
	}

	if((vertex_list = (CVert3D**) malloc(sizeof(CVert3D*) * MAX_VERTS_PER_LIST)) == NULL) {
		printf("Can not allocate memory to hold vertex list\n");
		exit(0);
	}

	for(int i=0;i<num_vertexs;i++) {
		vertex_list[i] = new CVert3D(*vl[i]);
	}
}

// --------------------------------------------------------------------------------- 
// set the texture vertex list to tl making a copy of all the verts
// --------------------------------------------------------------------------------- 
void CLBSP::set_tverts_list(CVert3D **tl)
{
	if(num_tverts <= 0) {
		printf("Make sure you set vertex count first\n");
		exit(0);
	}

	if((tverts_list = (CVert3D**) malloc(sizeof(CVert3D*) * MAX_VERTS_PER_LIST)) == NULL) {
		printf("Can not allocate memory to hold vertex list\n");
		exit(0);
	}

	for(int i=0;i<num_tverts;i++) {
		tverts_list[i] = new CVert3D(*tl[i]);
	}
}

// --------------------------------------------------------------------------------- 
// write out a polygon to the file
// --------------------------------------------------------------------------------- 
void CLBSP::write_polygon(CPoly *p)
{
	write_printf("%d %d\n", p->material_id, p->ni);
	write_printf("%d %d %d\n", p->vi[0], p->vi[1], p->vi[2]);
	write_printf("%d %d %d\n", p->ti[0], p->ti[1], p->ti[2]);
	// generate and save a plane for this polygon
	
	CPlane *pl = new CPlane(*normal_list[p->ni], *vertex_list[p->vi[0]]);
	write_printf("%d\n", num_planes);
	plane_list[num_planes] = pl;
	num_planes++;

	write_printf("%d %d\n", p->is_static, p->lightmap_id);

	for(int i=0;i<3;i++) {
		write_printf("%.20f %.20f\n", p->lm_u[i], p->lm_v[i]);
	}

	write_printf("%d %d\n", p->lm_h, p->lm_w);
}

// --------------------------------------------------------------------------------- 
// write out a BSP node, i.e. all polygons in the node
// --------------------------------------------------------------------------------- 
void CLBSP::write_bsp_node(CBSPNode *bsp)
{	
	write_printf(NODE_START);

	if(bsp->bb_max.x == -100000) { // only calculate bounding box if it wasn;t created before

		for(int i=0;i<bsp->polygons->count();i++) {
			CPoly p = *bsp->polygons->get(i);

			if(p.is_static == 1) continue;

			if(vertex_list[p.vi[0]]->x < bsp->bb_min.x)
				bsp->bb_min.x = vertex_list[p.vi[0]]->x;
			if(vertex_list[p.vi[0]]->y < bsp->bb_min.y)
				bsp->bb_min.y = vertex_list[p.vi[0]]->y;
			if(vertex_list[p.vi[0]]->z < bsp->bb_min.z)
				bsp->bb_min.z = vertex_list[p.vi[0]]->z;

			if(vertex_list[p.vi[0]]->x > bsp->bb_max.x)
				bsp->bb_max.x = vertex_list[p.vi[0]]->x;
			if(vertex_list[p.vi[0]]->y > bsp->bb_max.y)
				bsp->bb_max.y = vertex_list[p.vi[0]]->y;
			if(vertex_list[p.vi[0]]->z > bsp->bb_max.z)
				bsp->bb_max.z = vertex_list[p.vi[0]]->z;
	
			if(vertex_list[p.vi[1]]->x < bsp->bb_min.x)
				bsp->bb_min.x = vertex_list[p.vi[1]]->x;
			if(vertex_list[p.vi[1]]->y < bsp->bb_min.y)
				bsp->bb_min.y = vertex_list[p.vi[1]]->y;
			if(vertex_list[p.vi[1]]->z < bsp->bb_min.z)
				bsp->bb_min.z = vertex_list[p.vi[1]]->z;

			if(vertex_list[p.vi[1]]->x > bsp->bb_max.x)
				bsp->bb_max.x = vertex_list[p.vi[1]]->x;
			if(vertex_list[p.vi[1]]->y > bsp->bb_max.y)
				bsp->bb_max.y = vertex_list[p.vi[1]]->y;
			if(vertex_list[p.vi[1]]->z > bsp->bb_max.z)
				bsp->bb_max.z = vertex_list[p.vi[1]]->z;

			if(vertex_list[p.vi[2]]->x < bsp->bb_min.x)
				bsp->bb_min.x = vertex_list[p.vi[2]]->x;
			if(vertex_list[p.vi[2]]->y < bsp->bb_min.y)
				bsp->bb_min.y = vertex_list[p.vi[2]]->y;
			if(vertex_list[p.vi[2]]->z < bsp->bb_min.z)
				bsp->bb_min.z = vertex_list[p.vi[2]]->z;

			if(vertex_list[p.vi[2]]->x > bsp->bb_max.x)
				bsp->bb_max.x = vertex_list[p.vi[2]]->x;
			if(vertex_list[p.vi[2]]->y > bsp->bb_max.y)
				bsp->bb_max.y = vertex_list[p.vi[2]]->y;
			if(vertex_list[p.vi[2]]->z > bsp->bb_max.z)
				bsp->bb_max.z = vertex_list[p.vi[2]]->z;
		}
	}

	// write the bounding box data
	write_printf("%f %f %f %f %f %f\n", bsp->bb_min.x, bsp->bb_min.y, bsp->bb_min.z, bsp->bb_max.x, bsp->bb_max.y, bsp->bb_max.z); 

	// write visible node info
	write_printf("%d\n", bsp->num_vis_nodes);
	for(int i=0;i<bsp->num_vis_nodes;i++) {
		write_printf("%d\n", bsp->vis_node_list[i]);
	}

	if(bsp->divider != NULL) {		
		write_polygon(bsp->divider);                    // write the divider for the node
	} else  {
		write_printf("-1");

		write_printf("\n%d\n", bsp->polygons->count()); // write the num polygons for node

		// output each polygon in node -------------------------------------------------
		for(i=0;i<bsp->polygons->count();i++) {
			write_polygon(bsp->polygons->get(i));
		}
	}
}

// --------------------------------------------------------------------------------- 
// write out a whole branch of the tree
// --------------------------------------------------------------------------------- 
void CLBSP::write_branch(CBSPNode *bsp)
{
	if(bsp->front != NULL) {
		write_bsp_node(bsp->front);
		write_branch(bsp->front);
	} else {
		write_printf(FRONT_EMPTY);
	}

	if(bsp->back != NULL) {
		write_bsp_node(bsp->back);
		write_branch(bsp->back);
	} else {
		write_printf(BACK_EMPTY);
	}
}

// --------------------------------------------------------------------------------- 
// write the material list
// --------------------------------------------------------------------------------- 
void CLBSP::write_material_list(void)
{
	write_printf("NumberMaterials %d\n", num_materials);
	
	for(int i=0;i<num_materials;i++) {
		write_printf("MaterialNum %d\nMaterialName ", material_list[i].num);
		for(int j=0;j<(int)strlen(material_list[i].name);j++) {
			write(fd, &material_list[i].name[j], 1);
		}
		write_printf("\nMaterialFilename ");
		for(j=0;j<(int)strlen(material_list[i].filename);j++) {
			write(fd, &material_list[i].filename[j], 1);
		}
		write(fd, "\n", 1);
	}
}

// --------------------------------------------------------------------------------- 
// write the vertex list
// --------------------------------------------------------------------------------- 
void CLBSP::write_vertex_list(void)
{
	write_printf("\nVertexList %d\n", num_vertexs);

	for(int i=0;i<num_vertexs;i++) {
		write_printf("%f %f %f\n", vertex_list[i]->x, vertex_list[i]->y, vertex_list[i]->z);
	}	
}

// --------------------------------------------------------------------------------- 
// write the texture list
// --------------------------------------------------------------------------------- 
void CLBSP::write_tverts_list(void)
{
	write_printf("\nTextureVertexList %d\n", num_tverts);

	for(int i=0;i<num_tverts;i++) {
		write_printf("%f %f %f\n", tverts_list[i]->x, tverts_list[i]->y, tverts_list[i]->z);
	}	
}

// --------------------------------------------------------------------------------- 
// write the normal list
// --------------------------------------------------------------------------------- 
void CLBSP::write_normal_list(void)
{
	write_printf("\nNormalList %d\n", num_normals);

	for(int i=0;i<num_normals;i++) {
		write_printf("%f %f %f\n", normal_list[i]->x, normal_list[i]->y, normal_list[i]->z);
	}	
}

// --------------------------------------------------------------------------------- 
// write all the planes
// --------------------------------------------------------------------------------- 
void CLBSP::write_plane_list(void)
{
	write_printf("\nPlaneList %d\n", num_planes);

	for(int i=0;i<num_planes;i++) {
		write_printf("%f %f %f %f\n", plane_list[i]->a, plane_list[i]->b, plane_list[i]->c, plane_list[i]->d);
	}	
}

// --------------------------------------------------------------------------------- 
// write all the lights
// --------------------------------------------------------------------------------- 
void CLBSP::write_light_list()
{
	write_printf("\nLightList %d\n", num_lights);

	for(int i=0;i<num_lights;i++) {
		write_printf("%f %f %f %d %f %f %f %f %f\n", light_list[i]->pos.x, light_list[i]->pos.y, light_list[i]->pos.z, light_list[i]->type, light_list[i]->color[0], light_list[i]->color[1], light_list[i]->color[2], light_list[i]->intesity, light_list[i]->attenuation); 
	}
}

// --------------------------------------------------------------------------------- 
// function that writes a complete BSP tree to disk
// --------------------------------------------------------------------------------- 
int CLBSP::write_bsp_tree(char *filename)
{
	fd = open(filename, _O_CREAT | _O_TRUNC | _O_WRONLY, _S_IWRITE);

	if(fd < 0) {
		printf("can not open output file %s\n", filename);
		return 0;
	}
	num_planes = 0;
	write(fd, BSP_FILE_HEADER, strlen(BSP_FILE_HEADER));
	write(fd, "\n", 1);

	write_material_list();	
	write_vertex_list();
	write_tverts_list();
	write_normal_list();
	
	write_bsp_node(bsp_tree); // write the root
	write_branch(bsp_tree);   // write its branches

	write_plane_list();
	write_light_list();
	return 1;
}

// --------------------------------------------------------------------------------- 
// Read the materials
// --------------------------------------------------------------------------------- 
void CLBSP::read_material_list(void)
{	
	num_materials = get_int();
	if((material_list = (Material *) malloc(sizeof(Material) * num_materials)) == NULL) {
		dbp("Can not allocate memory for materials %d, %s.\n", num_materials, line);
		printf("Can not allocate memory for materials %d, %s.\n", num_materials, line);
		exit(0);
	}

	for(int i=0;i<num_materials;i++) {
		material_list[i].num = get_int();
		get_string(material_list[i].name);
		get_string(material_list[i].filename);	
	}
}

// --------------------------------------------------------------------------------- 
// read vertex list
// --------------------------------------------------------------------------------- 
void CLBSP::read_vertex_list(void)
{
	char strbuf[30];

	read_line(); sscanf(line, "%s %d", strbuf, &num_vertexs);

	if(!cmp("VertexList")) {
		dbp("Expected VertexList got %s instead\n", line);
		exit(0);
	}

	// allocate memory ---------------------------------------------------------------
	if((vertex_list = (CVert3D**) malloc(sizeof(CVert3D*) * num_vertexs)) == NULL) {
		dbp("Can not allocate memory for the vertex list\n");
		exit(0);
	}

	// read the vertices -------------------------------------------------------------
	for(int i=0;i<num_vertexs;i++) {
		read_line();
		vertex_list[i] = new CVert3D();
		sscanf(line, "%f %f %f", &vertex_list[i]->x, &vertex_list[i]->y, &vertex_list[i]->z);
	}
}

// --------------------------------------------------------------------------------- 
// read texture verts list
// --------------------------------------------------------------------------------- 
void CLBSP::read_tverts_list(void)
{
	char strbuf[30];

	read_line(); sscanf(line, "%s %d", strbuf, &num_tverts);

	if(!cmp("TextureVertexList")) {
		dbp("Expected TextureVertexList got %s instead\n", line);
		exit(0);
	}

	// allocate memory ---------------------------------------------------------------
	if((tverts_list = (CVert3D**) malloc(sizeof(CVert3D*) * num_tverts)) == NULL) {
		dbp("Can not allocate memory for the texture vertex list\n");
		exit(0);
	}

	// read the vertices -------------------------------------------------------------
	for(int i=0;i<num_tverts;i++) {
		read_line();
		tverts_list[i] = new CVert3D();
		sscanf(line, "%f %f %f", &tverts_list[i]->x, &tverts_list[i]->y, &tverts_list[i]->z);
	}
}

// --------------------------------------------------------------------------------- 
// read normal list
// --------------------------------------------------------------------------------- 
void CLBSP::read_normal_list(void)
{
	char strbuf[30];

	read_line(); sscanf(line, "%s %d", strbuf, &num_normals);

	if(!cmp("NormalList")) {
		printf("Expected NormalList got %s instead\n", line);
		exit(0);
	}

	// allocate memory ---------------------------------------------------------------
	if((normal_list = (CVert3D**) malloc(sizeof(CVert3D*) * num_normals)) == NULL) {
		printf("Can not allocate memory for the normal list\n");
		exit(0);
	}

	// read the vertices -------------------------------------------------------------
	for(int i=0;i<num_normals;i++) {
		read_line();
		normal_list[i] = new CVert3D();
		sscanf(line, "%f %f %f", &normal_list[i]->x, &normal_list[i]->y, &normal_list[i]->z);
	}
}

// --------------------------------------------------------------------------------- 
// read plane list
// --------------------------------------------------------------------------------- 
void CLBSP::read_plane_list(void)
{
	char strbuf[30];

	read_line(); sscanf(line, "%s %d", strbuf, &num_planes);

	if(!cmp("PlaneList")) {
		dbp("Expected PlaneList got %s instead\n", line);
		exit(0);
	}

	// read the planes -------------------------------------------------------------
	for(int i=0;i<num_planes;i++) {
		read_line();
		plane_list[i] = new CPlane();
		sscanf(line, "%f %f %f %f", &plane_list[i]->a, &plane_list[i]->b, &plane_list[i]->c, &plane_list[i]->d);
	}
}

// --------------------------------------------------------------------------------- 
// read light list
// --------------------------------------------------------------------------------- 
void CLBSP::read_light_list(void)
{
	char strbuf[30];

	read_line(); sscanf(line, "%s %d", strbuf, &num_lights);

	if(!cmp("LightList")) {
		dbp("Expected LightList got %s instead\n", line);
		exit(0);
	}

	// allocate memory ---------------------------------------------------------------
	if((light_list = (CLight**) malloc(sizeof(CLight*) * num_lights)) == NULL) {
		dbp("Can not allocate memory for the light list\n");
		exit(0);
	}

	// read the planes -------------------------------------------------------------
	for(int i=0;i<num_lights;i++) {
		read_line();
		light_list[i] = new CLight();
		sscanf(line, "%f %f %f %d %f %f %f %f %f", &light_list[i]->pos.x, &light_list[i]->pos.y, &light_list[i]->pos.z, &light_list[i]->type, &light_list[i]->color[0], &light_list[i]->color[1], &light_list[i]->color[2], &light_list[i]->intesity, &light_list[i]->attenuation);
	}

}

// --------------------------------------------------------------------------------- 
// read the complete BSP tree
// --------------------------------------------------------------------------------- 
int CLBSP::read_bsp_tree(char *filename)
{
	if(!open_file(filename)) return 0;


	read_line();
	if(!cmp(BSP_FILE_HEADER)) {
		dbp("Not a Horizon BSP file\n");
		return 0;
	}

	read_material_list();	
	read_vertex_list();
	read_tverts_list();
	read_normal_list();

	bsp_tree = new CBSPNode();

	read_line(); // need to read the first line
	read_bsp_node(bsp_tree);	


	read_plane_list();
	read_light_list();

	strcpy(bsp_name, filename);

	return 1;	
}
 
// --------------------------------------------------------------------------------- 
// read a divider (same as reading polygon just assume first line has been read)
// --------------------------------------------------------------------------------- 
void CLBSP::read_divider(CPoly *p)
{
	// has no readline
	sscanf(line, "%d %d", &p->material_id, &p->ni);
	read_line();
	sscanf(line, "%d %d %d", &p->vi[0],&p->vi[1],&p->vi[2]);
	read_line();
	sscanf(line, "%d %d %d", &p->ti[0],&p->ti[1],&p->ti[2]);
	read_line();
	sscanf(line, "%d", &p->plane_id);
	read_line();
	sscanf(line, "%d %d", &p->is_static, &p->lightmap_id);
	for(int i=0;i<3;i++) {
		read_line();
		sscanf(line, "%f %f", &p->lm_u[i], &p->lm_v[i]);
	}
	read_line();
	sscanf(line, "%d %d", &p->lm_h, &p->lm_w);


}

// --------------------------------------------------------------------------------- 
// read a polygon
// --------------------------------------------------------------------------------- 
void CLBSP::read_polygon(CPoly *p)
{
	read_line();
	sscanf(line, "%d %d", &p->material_id, &p->ni);
	read_line();
	sscanf(line, "%d %d %d", &p->vi[0],&p->vi[1],&p->vi[2]);
	read_line();
	sscanf(line, "%d %d %d", &p->ti[0],&p->ti[1],&p->ti[2]);
	read_line();
	sscanf(line, "%d", &p->plane_id);
	read_line();
	sscanf(line, "%d %d", &p->is_static, &p->lightmap_id);
	for(int i=0;i<3;i++) {
		read_line();
		sscanf(line, "%f %f", &p->lm_u[i], &p->lm_v[i]);
	}
	read_line();
	sscanf(line, "%d %d", &p->lm_h, &p->lm_w);

}

// --------------------------------------------------------------------------------- 
// read a node
// --------------------------------------------------------------------------------- 
void CLBSP::read_bsp_node(CBSPNode *bsp)
{
	CPoly *p = new CPoly(); //(Poly*) malloc(sizeof(Poly));
	int number_polys;
	int test_divider;

	dbp("Reading a node...");
	
	if(cmp("BSP_NODE")) { 
		dbp("Found node\n");

		// Read the bounding box info
		read_line();
		sscanf(line, "%f %f %f %f %f %f", &bsp->bb_min.x, &bsp->bb_min.y, &bsp->bb_min.z, &bsp->bb_max.x, &bsp->bb_max.y, &bsp->bb_max.z); 

		read_line();
		sscanf(line, "%d", &bsp->num_vis_nodes);
		
		for(int i=0;i<bsp->num_vis_nodes;i++) {
			read_line();
			sscanf(line, "%d", &bsp->vis_node_list[i]);
		}

		// read the dividing polygon
		read_line(); sscanf(line, "%d", &test_divider);
		if(test_divider == -1) {
			bsp->divider = NULL;

			// read the number of polygons
			read_line();	sscanf(line, "%d", &number_polys);

			// read each polygon
			for(int i=0;i<number_polys;i++) {
				read_polygon(p);
				
				int v0 = p->vi[0];
				int v1 = p->vi[1];
				int v2 = p->vi[2];

				if(*vertex_list[v0] == *vertex_list[v1] ||
					*vertex_list[v1] == *vertex_list[v2] ||
					*vertex_list[v0] == *vertex_list[v2] )
					continue; // no line polygons!

				int countx, county, countz;

				countx = county = countz = 0;
				if(vertex_list[v0]->x == vertex_list[v1]->x)
					countx++;
				if(vertex_list[v1]->x == vertex_list[v2]->x)
					countx++;
				if(vertex_list[v0]->x == vertex_list[v2]->x)
					countx++;

				if(vertex_list[v0]->y == vertex_list[v1]->y)
					county++;
				if(vertex_list[v1]->y == vertex_list[v2]->y)
					county++;
				if(vertex_list[v0]->y == vertex_list[v2]->y)
					county++;

				if(vertex_list[v0]->z == vertex_list[v1]->z)
					countz++;
				if(vertex_list[v1]->z == vertex_list[v2]->z)
					countz++;
				if(vertex_list[v0]->z == vertex_list[v2]->z)
					countz++;

				if( (countx == 3 && county == 3) || 
					(countx == 3 && countz == 3) ||
					(county == 3 && countz == 3)) {
					continue;
				}

				bsp->polygons->add(p);
				dbp("   - read polygon\n");
			}

		}

		else {
			read_divider(bsp->divider);
		}

	} 

	// check front node
	read_line();
	dbp("Now checking front [%s]\n", line);
	if(!cmp("front_empty")) {
		if(!cmp("BSP_NODE")) {
			dbp("should be front node\n");
			return;
		}
		bsp->front = new CBSPNode();
		read_bsp_node(bsp->front);
	} 

	read_line();
	dbp("Now checking back [%s]\n", line);
	if(!cmp("back_empty")) {
		if(!cmp("BSP_NODE")) {
			dbp("should be back node\n");
			return;
		}
		bsp->back = new CBSPNode();
		read_bsp_node(bsp->back);
	}
}

// --------------------------------------------------------------------------------- 
// count the number of polygons in node
// --------------------------------------------------------------------------------- 
int CLBSP::recusive_count(CBSPNode *bsp, int total_so_far)
{
	if(bsp->front == NULL && bsp->back == NULL) {
		return total_so_far + bsp->polygons->count();
	}

	if(bsp->front != NULL) {
		total_so_far = recusive_count(bsp->front, total_so_far);
	}
	if(bsp->back != NULL) {
		total_so_far = recusive_count(bsp->back, total_so_far);
	}

	return total_so_far;
	
}

// --------------------------------------------------------------------------------- 
// count all polygons in bsp tree
// --------------------------------------------------------------------------------- 
int CLBSP::count_polys()
{
	return recusive_count(bsp_tree, 0);
}

// END OF FILE ----------------------------------------------------------------------