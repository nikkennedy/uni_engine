/*	--------------------------------------------------------------------------------- 
	Filename      - lightmap_gen.cpp
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Impementation of CLightMapGenerator
	--------------------------------------------------------------------------------- */

#include <stdio.h>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <types.h>
#include <debug.h>
#include <parse.h>
#include <maths.h>
#include <lbsp.h>
#include <lightmap.h>
#include <lightmap_gen.h>

// --------------------------------------------------------------------------------- 
// constructor
// --------------------------------------------------------------------------------- 
CLightMapGenerator::CLightMapGenerator()
{ 
	bsp_parser = new CLBSP();
	create_lookups(); 
	next_free_light_map_id = 0;	
	low_resolution = 1;
	total_bytes = 0;
};

// --------------------------------------------------------------------------------- 
// descructor
// --------------------------------------------------------------------------------- 
CLightMapGenerator::~CLightMapGenerator() 
{
	delete bsp_parser;
}

// --------------------------------------------------------------------------------- 
// create a complete list of all the polygons in the BSP tree
// --------------------------------------------------------------------------------- 
void CLightMapGenerator::create_polygon_list(CPolygonList *p_list, CBSPNode *bsp)
{
	if(bsp->front == NULL && bsp->back == NULL) {
		int count = bsp->polygons->count();

		for(int i=0;i<count;i++) {
			CPoly *p = bsp->polygons->get(i);
			p->create_per_pen_plane(vertex_list, normal_list);
			p_list->add(p);
		}
		return;
	}

	if(bsp->front != NULL)
		create_polygon_list(p_list, bsp->front);
	if(bsp->back != NULL)
		create_polygon_list(p_list, bsp->back);
}

// --------------------------------------------------------------------------------- 
// does the vector from start to end hit intersect polygon p
// --------------------------------------------------------------------------------- 
inline int CLightMapGenerator::hit_poly(CPoly *p, CVert3D start, CVert3D end)
{
	CPlane pl = CPlane(*normal_list[p->ni], *vertex_list[p->vi[0]]);
	
	// if both positive or both negative then didn't cross plane of polygon
	if(pl.classify(start) * pl.classify(end) >= -0.5)
		return 0; 
	
	CVert3D vec = end-start;
	float sect = - pl.classify(start) / (pl.normal() * vec);

	CVert3D intersect = start + (vec * sect);

	if(p->p1.classify(intersect) >= 0 && p->p2.classify(intersect) >= 0 && p->p3.classify(intersect) >= 0)
		return 1; // yes

	return 0; // no
}

// --------------------------------------------------------------------------------- 
// does ray from start to end hit anyother polygon excluding p
// --------------------------------------------------------------------------------- 
inline int CLightMapGenerator::hit_other_poly(CPoly p, CVert3D start, CVert3D end, CBSPNode *cur_node)
{
	int count = cur_node->polygons->count();

	// check current leaf
	for(int i=0;i<count;i++) {
		CPoly *p2 = cur_node->polygons->get(i);

		if(*p2 != p) {

			CPlane pl1 = *plane_list[p.plane_id];
			CPlane pl2 = *plane_list[p2->plane_id];

			if(pl1 != pl2 || pl2.classify(*vertex_list[p.vi[0]]) < 0)  {
				
				if(hit_poly(p2, start, end))
					return 1;
			}						
		}
	}

	// check all visible leaves
	for(int k=0;k<cur_node->num_vis_nodes;k++) {
		CPolygonList *plist = node_list[cur_node->vis_node_list[k]]->polygons;

		int count = plist->count();

		for(int i=0;i<count;i++) {
			CPoly *p2 = plist->get(i);

			if(*p2 != p) {

				CPlane pl1 = *plane_list[p.plane_id];
				CPlane pl2 = *plane_list[p2->plane_id];

				if(pl1 != pl2 || pl2.classify(*vertex_list[p.vi[0]]) < 0)  {
					if(hit_poly(p2, start, end))
						return 1;
				}						
			}
		}
	}

	return 0; // no it does not
}

// --------------------------------------------------------------------------------- 
// calculate the size required for each lightmap
// --------------------------------------------------------------------------------- 
void CLightMapGenerator::scan_poly_first_pass(CPoly *p)
{
	CVert3D norm = *normal_list[p->ni];
	CVert3D anorm;
	CVert3D vs[3];
	CVert3D light_out;
	CPlane pplanes[3];
	int planemap = -1;
	int lmid = next_free_light_map_id++;

	p->lightmap_id = lmid;

	// calculate prominent axes using normal of polygon
	anorm.x = (float) fabs(norm.x);	anorm.y = (float) fabs(norm.y);	anorm.z = (float) fabs(norm.z);
	CPlane plane = CPlane(norm, *vertex_list[p->vi[0]]);
	if(anorm.x >= anorm.y && anorm.x >= anorm.z) {
		dbp("Mapping to YZ plane\n");
		planemap = 1;
	} else if(anorm.y > anorm.x && anorm.y > anorm.z) {
		dbp("Mapping to XZ plane\n");
		planemap = 2;
	} else if(anorm.z > anorm.x && anorm.z > anorm.y) {
		dbp("Mapping to XY plane\n");
		planemap = 3;
	} else {
		dbp("Problem with plane\n");
		printf("Problem with Plane: %f, %f, %f\n", norm.x, norm.y, norm.z);
		exit(1);
	}

	for(int i=0;i<3;i++) {
		vs[i] = *vertex_list[p->vi[i]];
	}

	// map prominent axis to u,v
	for(i=0;i<3;i++) {
		if(planemap == 1) {
			light_maps[lmid]->u[i] = vs[i].z;
			light_maps[lmid]->v[i] = vs[i].y;
		}
		if(planemap == 2) {
			light_maps[lmid]->u[i] = vs[i].x;
			light_maps[lmid]->v[i] = vs[i].z;
		}
		if(planemap == 3) {
			light_maps[lmid]->u[i] = vs[i].x;
			light_maps[lmid]->v[i] = vs[i].y;
		}
	}

	// calculate max and min u,v
	float umin = light_maps[lmid]->u[0];
	float vmin = light_maps[lmid]->v[0];
	float umax = light_maps[lmid]->u[0];
	float vmax = light_maps[lmid]->v[0];

	for(i=0;i<3;i++) {
		if(light_maps[lmid]->u[i] < umin)
			umin = light_maps[lmid]->u[i];
		if(light_maps[lmid]->v[i] < vmin)
			vmin = light_maps[lmid]->v[i];

		if(light_maps[lmid]->u[i] > umax)
			umax = light_maps[lmid]->u[i];
		if(light_maps[lmid]->v[i] > vmax)
			vmax = light_maps[lmid]->v[i];

	}

	float du = umax - umin;
	float dv = vmax - vmin;

	for(i=0;i<3;i++) {
		light_maps[lmid]->u[i] -= umin;
		light_maps[lmid]->v[i] -= vmin;
		light_maps[lmid]->u[i] /= du;
		light_maps[lmid]->v[i] /= dv;	

		// these are the polygons texture coordiates used to map the 
		// lightmap to the polygon correctly
		p->lm_u[i] = light_maps[lmid]->u[i];
		p->lm_v[i] = light_maps[lmid]->v[i];
	}


	CVert3D uv_vector;
	CVert3D vect1;
	CVert3D vect2;
	CVert3D edge1;
	CVert3D edge2;

	// calculate the vectors used to speed up calculation of each
	// light map pixel coord
	if(planemap == 1) {
		float X;
		X = -(plane.c * umin + plane.b * vmin + plane.d) / plane.a;
		uv_vector = CVert3D(X, vmin, umin);
		X = -(plane.c * umax + plane.b * vmin + plane.d) / plane.a;
		vect1 = CVert3D(X, vmin, umax);
		X = -(plane.c * umin + plane.b * vmax + plane.d) / plane.a;
		vect2 = CVert3D(X, vmax, umin);
	}
	if(planemap == 2) {
		float Y;
		Y = -(plane.a * umin + plane.c * vmin + plane.d) / plane.b;
		uv_vector = CVert3D(umin, Y, vmin);
		Y = -(plane.a * umax + plane.c * vmin + plane.d) / plane.b;
		vect1 = CVert3D(umax, Y, vmin);
		Y = -(plane.a * umin + plane.c * vmax + plane.d) / plane.b;
		vect2 = CVert3D(umin, Y, vmax);
	}
	if(planemap == 3) {
		float Z;
		Z = -(plane.a * umin + plane.b * vmin + plane.d) / plane.c;
		uv_vector = CVert3D(umin, vmin, Z);
		Z = -(plane.a * umax + plane.b * vmin + plane.d) / plane.c;
		vect1 = CVert3D(umax, vmin, Z);
		Z = -(plane.a * umin + plane.b * vmax + plane.d) / plane.c;
		vect2 = CVert3D(umin, vmax, Z);
		
	}

	edge1 = vect1 - uv_vector;
	edge2 = vect2 - uv_vector;

	float length1 = edge1.length();
	float length2 = edge2.length();

	// calculate width and height of lightmap based on width and height
	// of each polygon

	float length = length1;

	if(length > 0 && length <= 4)
		light_maps[lmid]->width = 16;
	if(length > 4 && length <= 8)
		light_maps[lmid]->width = 32;
	if(length > 8 && length <= 16)
		light_maps[lmid]->width = 64;
	if(length > 16 && length <= 32)
		light_maps[lmid]->width = 128;
	if(length > 32 && length <= 64)
		light_maps[lmid]->width = 512;
	if(length > 64)
		light_maps[lmid]->width = 512;

	length = length2;

	if(length > 0 && length <= 4)
		light_maps[lmid]->height = 16;
	if(length > 4 && length <= 8)
		light_maps[lmid]->height = 32;
	if(length > 8 && length <= 16)
		light_maps[lmid]->height = 64;
	if(length > 16 && length <= 32)
		light_maps[lmid]->height = 128;
	if(length > 32 && length <= 64)
		light_maps[lmid]->height = 512;
	if(length > 64)
		light_maps[lmid]->height = 512;

	p->lm_h = light_maps[lmid]->height;
	p->lm_w = light_maps[lmid]->width;

	light_maps[lmid]->uv_vector = uv_vector;
	light_maps[lmid]->edge1 = edge1;
	light_maps[lmid]->edge2 = edge2;

	total_bytes += (light_maps[lmid]->width * light_maps[lmid]->height * 3);
}

// --------------------------------------------------------------------------------- 
// perform the lightmap generation process
// --------------------------------------------------------------------------------- 
void CLightMapGenerator::scan_poly(CPoly *p)
{
	int lmid = p->lightmap_id;
	CVert3D light_out;
	CLight *light;
	CVert3D norm = *normal_list[p->ni];

	CPlane plane      = CPlane(norm, *vertex_list[p->vi[0]]);
	CVert3D edge1     = light_maps[lmid]->edge1;
	CVert3D edge2     = light_maps[lmid]->edge2;
	CVert3D uv_vector = light_maps[lmid]->uv_vector;

	if((light_maps[lmid]->rgb_data = (u_char*) malloc(sizeof(u_char) * (light_maps[lmid]->height) * (light_maps[lmid]->width) * 3)) == NULL) {
		printf("[ :( ] Can not allocate memory for lightmap\n");
		exit(1);
	}

	// add ambient lighting
	CVert3D ambient_light = CVert3D(0,0,0);
	int ambient_r, ambient_g, ambient_b;
	for(int lighti=0;lighti<num_lights;lighti++) {
		light = light_list[lighti];
		if(light->type == LIGHT_TYPE_AMBIENT) {
			ambient_light = CVert3D(light->color[0], light->color[1], light->color[2]);
			ambient_light = ambient_light * 255.0f;
		}

	}

	ambient_r = ambient_light.x;
	ambient_g = ambient_light.y;
	ambient_b = ambient_light.z;

	// This is to set the ambient light
	for(int x = 0; x< light_maps[lmid]->width;x++) {
		for(int y=0;y<light_maps[lmid]->height;y++) {
				int index = (y*light_maps[lmid]->width*3) + x*3;
				
				light_maps[lmid]->rgb_data[index]   = (int)ambient_r;
				light_maps[lmid]->rgb_data[index+1] = (int)ambient_g;
				light_maps[lmid]->rgb_data[index+2] = (int)ambient_b;			
		}
	}

	printf("%% complete is %.2f\r", (float)lmid/(float)num_polys * 100);
	
	float ir, ig, ib;

	// process each light
	for(lighti = 0; lighti < num_lights; lighti++) {

		light_out = light_list[lighti]->pos;
		light = light_list[lighti];

		if(light->type == LIGHT_TYPE_AMBIENT)
			continue;

		if(plane.classify(light->pos) < 0){
			// Light is behind this polygons plane, so ignore it
			continue;
		}

		int I_plusR;
		int I_plusG;
		int I_plusB;
		int index;
		float attenuation;
		float i_only;
		
		// do this here to save on divisions in the loop
		float inverse_multiply = 1.0f / light->attenuation; 
		
		float div_width = 1.0f / (float)light_maps[lmid]->width;
		float div_height = 1.0f / (float)light_maps[lmid]->height;

		CVert3D new_edge1;
		CVert3D new_edge2;
		CVert3D point;
		CVert3D lumel;
		
		int node_count;
		CBSPNode *cur_node = bsp_parser->which_node(light_out, node_count);

		// create low resolution textures
		if(low_resolution) {
			for(int xl = 0; xl < light_maps[lmid]->width; xl+=4) {
				for(int yl = 0; yl < light_maps[lmid]->height; yl+=4) {
					float ufactor = (float)(xl) * div_width;
					float vfactor = (float)(yl) * div_height;

					new_edge1 = edge1 * ufactor;	
					new_edge2 = edge2 * vfactor;

					lumel = uv_vector + new_edge1 + new_edge2;

					// if ray hits other poly then in shadow so ignore lights
					// contribution
					if(hit_other_poly(*p, lumel, light_out, cur_node)) 
						continue;
						
					point = light_out - lumel;

					float length = point.length(); // only calculate the length once (sqrt)
					float distsq = length * length * inverse_multiply;
					point = point / length; // don't call normalise as it will perform sqrt again

					// light value calculation
					attenuation = 1 / distsq;
					i_only = (light->intesity * (norm * point)) * 255 * attenuation;

					ir = (i_only * light->color[0]);
					ig = (i_only * light->color[1]);
					ib = (i_only * light->color[2]);

					for(int x2=xl;x2<xl+4;x2++) {
						for(int y2=yl;y2<yl+4;y2++) {
							index = (y2*light_maps[lmid]->width*3) + x2*3;
				
							I_plusR = (int)(light_maps[lmid]->rgb_data[index]  ) + (int)ir;
							I_plusG = (int)(light_maps[lmid]->rgb_data[index+1]) + (int)ig;
							I_plusB = (int)(light_maps[lmid]->rgb_data[index+2]) + (int)ib;
			
							// clamp values to 255
							if(I_plusR > 255) I_plusR = 255;
							if(I_plusG > 255) I_plusG = 255;
							if(I_plusB > 255) I_plusB = 255;				

							light_maps[lmid]->rgb_data[index]   = I_plusR;
							light_maps[lmid]->rgb_data[index+1] = I_plusG;
							light_maps[lmid]->rgb_data[index+2] = I_plusB;
						}
					}				
				}			
			}
		} else { // higher resolution textures
			for(int xl = 0; xl < light_maps[lmid]->width; xl++) {
				for(int yl = 0; yl < light_maps[lmid]->height; yl++) {
					float ufactor = (float)(xl) * div_width;//light_maps[lmid]->width;
					float vfactor = (float)(yl) * div_height;//light_maps[lmid]->height;

					new_edge1 = edge1 * ufactor;	
					new_edge2 = edge2 * vfactor;

					lumel = uv_vector + new_edge1 + new_edge2;

					if(hit_other_poly(*p, lumel, light_out, cur_node)) 
						continue;
						
					point = light_out - lumel;

					float length = point.length(); // only calculate the length once (sqrt)
					float distsq = length * length * inverse_multiply;
					point = point / length; // don't call normalise as it will perform sqrt again

					attenuation = 1 / distsq;

					i_only = (light->intesity * (norm * point)) * 255 * attenuation;

					ir = (i_only * light->color[0]);
					ig = (i_only * light->color[1]);
					ib = (i_only * light->color[2]);

					index = (yl*light_maps[lmid]->width*3) + xl*3;
				
					I_plusR = (int)(light_maps[lmid]->rgb_data[index]  ) + (int)ir;
					I_plusG = (int)(light_maps[lmid]->rgb_data[index+1]) + (int)ig;
					I_plusB = (int)(light_maps[lmid]->rgb_data[index+2]) + (int)ib;
			
					if(I_plusR > 255) I_plusR = 255;
					if(I_plusG > 255) I_plusG = 255;
					if(I_plusB > 255) I_plusB = 255;				

					light_maps[lmid]->rgb_data[index]   = I_plusR;
					light_maps[lmid]->rgb_data[index+1] = I_plusG;
					light_maps[lmid]->rgb_data[index+2] = I_plusB;
				}			
			}
		}
	}	
}

// --------------------------------------------------------------------------------- 
// process each leaf of the BSP tree recursively
// --------------------------------------------------------------------------------- 
void CLightMapGenerator::process_lights_recursive(CBSPNode *bsp, int pass)
{
	if(bsp->front == NULL && bsp->back == NULL) {		

		CPolygonList *p_list = bsp->polygons;
		float start_time;
		if(pass != FIRST_PASS) start_time = GetTickCount();
		for(int i=0;i<p_list->count();i++) {
			CPoly *p = p_list->get(i);	
			if(pass == FIRST_PASS)
				scan_poly_first_pass(p);
			else {
				scan_poly(p);
			}
		}

		if(pass != FIRST_PASS)  {
			float end_time = GetTickCount();
			printf("Leaf processing time was %.2f secs.\n", (end_time - start_time) / 1000.0f);
		}
		return;
	}

	if(bsp->front != NULL) {
		process_lights_recursive(bsp->front, pass);
	}
	if(bsp->back != NULL) {
		process_lights_recursive(bsp->back, pass);
	}
}


// --------------------------------------------------------------------------------- 
// output the lightmaps to file
// --------------------------------------------------------------------------------- 
void CLightMapGenerator::save_light_maps(char *filenameroot)
{
	char filename[255];

	for(int i=0;i<num_polys;i++) {
		sprintf(filename, "%s\\lightmap%d.raw", filenameroot, i);
		int fd = open(filename, _O_BINARY | _O_CREAT | _O_RDWR | _O_TRUNC,  _S_IREAD | _S_IWRITE);

		if(fd <= 0) {
			printf("Unable to save lightmap %s, terminating.\n", filename);
			return;
		}

		int index = 0;
		for(int x = 0;x<light_maps[i]->width;x++) {
			for(int y=0;y<light_maps[i]->height;y++) {
				write(fd, (char*)&light_maps[i]->rgb_data[((y)*light_maps[i]->width*3) + ((x)*3)], 3); 
			}
		}
		close(fd);
	}	
}

// --------------------------------------------------------------------------------- 
// main function call provided by CLightMapGenerator, this does all the hard work
// --------------------------------------------------------------------------------- 
void CLightMapGenerator::generate_light_maps(char *in_fname, char *out_fname, char *lightmap_path)
{
	float start_time = GetTickCount();

	printf("Loading the BSP tree %s...\n", in_fname);

	if(!bsp_parser->read_bsp_tree(in_fname)) {
		printf("Can not parse input BSP tree file %s\n", in_fname);
		return;
	}

	set_num_normals(bsp_parser->get_num_normals());
	set_num_vertices(bsp_parser->get_num_vertexs());
	set_num_lights(bsp_parser->get_num_lights());

	set_normal_list(bsp_parser->get_normal_list());
	set_vertex_list(bsp_parser->get_vertex_list());
	set_light_list(bsp_parser->get_light_list());
	set_plane_list(bsp_parser->get_plane_list());

	num_polys = bsp_parser->count_polys();
	printf("Number polys in BSP = %d\n", num_polys);

	bsp_root = bsp_parser->get_bsp();

	node_list = (CBSPNode**) malloc(sizeof(CBSPNode*) * 1000);

	int number_nodes = 0;
	bsp_parser->create_node_list(bsp_root, node_list, &number_nodes);
	printf("There are %d leaves\n", number_nodes);


	if((light_maps = (CLightMap **) malloc(sizeof(CLightMap*) * num_polys)) == NULL) {
		printf("Can not allocate memory to generate %d lightmaps\n", num_polys);
		return;
	}

	for(int i=0;i<num_polys;i++) {
		light_maps[i] = new CLightMap();
	}

	if(low_resolution)
		printf("Using v.low resolution textures.\n");
	else 
		printf("Using v.high resolution textures.\n");

	total_poly_list = new CPolygonList();
	create_polygon_list(total_poly_list, bsp_root);

	printf("Processing first pass...\n");
	process_lights_recursive(bsp_root, FIRST_PASS);
	
	printf("Memory required for all the lightmaps is %d bytes\n", total_bytes);

	printf("Processing second pass...\n");
	process_lights_recursive(bsp_root, SECOND_PASS);

	printf("\nFinsihed so re-saving the updated BSP tree to %s...\n", out_fname);
	bsp_parser->write_bsp_tree(out_fname);

	printf("Now writing the lightmaps to %s...\n", lightmap_path);
	save_light_maps(lightmap_path);

	float end_time = GetTickCount();

	printf("Total Time to Complete is %.1f Minutes.\n", ((end_time - start_time) / 1000.0f)/60);	
}


// END OF FILE -----------------------------------------------------------------------