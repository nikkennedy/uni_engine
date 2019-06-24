/*	--------------------------------------------------------------------------------- 
	Filename      - lightmap_gen.h
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Class defintion of CLightMapGenerator
	--------------------------------------------------------------------------------- */
/*
	Only two public function

	void generate_light_maps(char *in_fname, char *out_fname, char *lightmap_path);
	void set_high_res();

 */

#ifndef _LIGHTMAP_GEN_H_
#define _LIGHTMAP_GEN_H_

#include <types.h>
#include <lbsp.h>

// --------------------------------------------------------------------------------- 
// process lights parameter
// --------------------------------------------------------------------------------- 
#define FIRST_PASS  0
#define SECOND_PASS 1

// --------------------------------------------------------------------------------- 
// CLightMapGenerator
// --------------------------------------------------------------------------------- 
class CLightMapGenerator
{
	// --------------------------------------------------------------------------------- 
	// Constructor/Destructor
	// --------------------------------------------------------------------------------- 

public:
	CLightMapGenerator();
	~CLightMapGenerator();

	// --------------------------------------------------------------------------------- 
	// BSP Specific
	// --------------------------------------------------------------------------------- 

private:
	int num_normals;
	int num_vertices;
	int num_lights;
	int num_polys;
	CLBSP     *bsp_parser;
	CVert3D   **vertex_list;   
	CVert3D   **normal_list;   
	CLight    **light_list;     
	CPlane    **plane_list;
	CBSPNode  *bsp_root;
	CBSPNode  **node_list;
	void set_bsp(CBSPNode *bsp)        { bsp_root = bsp; }
	void set_num_normals(int nn)       { num_normals = nn; }
	void set_num_vertices(int nv)      { num_vertices = nv; }
	void set_num_lights(int nl)        { num_lights = nl; }
	void set_normal_list(CVert3D **nl) { normal_list = nl; }
	void set_vertex_list(CVert3D **vl) { vertex_list = vl; }
	void set_light_list(CLight **ll)   { light_list = ll; }
	void set_plane_list(CPlane **pl)   { plane_list = pl; }

	CPolygonList *total_poly_list;
	inline int hit_other_poly(CPoly p, CVert3D start, CVert3D end, CBSPNode *cur_node);
	void create_polygon_list(CPolygonList *p_list, CBSPNode *bsp);	

	// --------------------------------------------------------------------------------- 
	// Lightmap specific
	// --------------------------------------------------------------------------------- 

private:
	CLightMap **light_maps;    // one per polygon
	int next_free_light_map_id;
	int total_bytes;
	int low_resolution;
	void process_lights_recursive(CBSPNode *bsp, int pass);
	void scan_poly(CPoly *p);
	void scan_poly_first_pass(CPoly *p);
	void save_light_maps(char *filenameroot);
public:
	void set_high_res() { low_resolution = 0; }
	void generate_light_maps(char *in_fname, char *out_fname, char *lightmap_path);

	// Ray Tracing (for shadows)
private:
	inline int hit_poly(CPoly *p, CVert3D start, CVert3D end);

};

#endif

// END OF FILE --------------------------------------------------------------------------