/*	--------------------------------------------------------------------------------- 
	Filename      - lbsp.h
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Definition of CLBSP (leafy BSP)
					This is probably the most fundamental file as its used by almost
					every other, it contains the functions to read, write and calculate
					leafy bsp trees
	--------------------------------------------------------------------------------- */

#ifndef _L_BSP_H_
#define _L_BSP_H_

#include <types.h>
#include <parse.h>
#include <light.h>
#include <p_list.h>

#define INFRONT         1
#define BEHIND          2
#define COINCEDENT      3
#define SPANNING        4
#define FRONT_IS_ONE    1
#define FRONT_IS_TWO    2
#define BACK_IS_ONE     4
#define BACK_IS_TWO     8
#define BACK_IS_NONE   16
#define FRONT_IS_NONE  32
#define EPSILON       1

#define BSP_FILE_HEADER "Horizon_BSP_File_v1.00"
#define FRONT_EMPTY     "front_empty\n"
#define BACK_EMPTY      "back_empty\n"
#define NODE_START      "BSP_NODE\n"

// --------------------------------------------------------------------------------- 
// bsp tree consists of these nodes
// --------------------------------------------------------------------------------- 
class CBSPNode 
{
public:
	CPoly        *divider;	// the polygon used to create splitting plane
	CBSPNode     *back;		// the back (left) sub tree
	CBSPNode     *front;	// the front (right) sub tree
	CPolygonList *polygons;	// the polygons in this node
	CVert3D      bb_max;    // bounding box max
	CVert3D      bb_min;    // bounding box min
	CPointSet    *pointset;      // for visibility calculations
	int          *vis_node_list; // visible nodes
	int          num_vis_nodes;  // num of visible nodes

	// --------------------------------------------------------------------------------- 
	// constructor
	// --------------------------------------------------------------------------------- 
	CBSPNode() 
	{
		divider  = new CPoly(); 
		back     = NULL;
		front    = NULL;
		polygons = new CPolygonList();
		vis_node_list = (int*) malloc(sizeof(int) * 1000);
		num_vis_nodes = 0;
		pointset = NULL;		

		bb_max = CVert3D(-100000,-100000,-100000);
		bb_min = CVert3D( 100000, 100000, 100000);

		if(vis_node_list == NULL) {
			printf("Gone over max node list\n");
			exit(1);
		}
	}

	// --------------------------------------------------------------------------------- 
	// destructor
	// --------------------------------------------------------------------------------- 
	~CBSPNode() 
	{
		if(divider != NULL)
			delete divider;
		if(polygons != NULL)
			delete polygons;
		if(pointset != NULL)
			delete pointset;
		
		free(vis_node_list);

		if(back != NULL)
			delete back;
		if(front != NULL)
			delete front;
	}
};


// --------------------------------------------------------------------------------- 
// CLSBP 
// --------------------------------------------------------------------------------- 
class CLBSP : public CParse {
public:
	Material  *material_list; // materials used
	CVert3D   **vertex_list;   // the vertex list used
	CVert3D   **tverts_list;   // the texture vertex list 
	CVert3D   **normal_list;   // the normal list
	CPlane    *plane_list[4000];    // the generated plane list
	CLight    **light_list; 
	int num_materials;
	int num_vertexs;
	int num_tverts;
	int num_normals;
	int num_planes;
	int num_lights;
	int skipping_static_poly_count;
	CPolygonList *bsp_p_list; // polygons used in the tree
	int *divider_indexs; // if 1 then used as divider
	CPoly *dividers[2000];
	int next_divider;
	CBSPNode *bsp_tree;
	int front, back, spanning;

	CLBSP();
	~CLBSP();

public:
	char bsp_name[255];

private:
	int is_convex(CPolygonList *p_list); // is the set of polygons convex?
	int in_front(CPoly *p1, CPoly *p2);    // is p2 infront of p1?
	CPoly *get_dividing_polygon(CPolygonList *p_list); // best dividing polygon
	int compare_polys(CPoly *p1, CPoly *p2); // returns 1 if match 0 otherwise
	int is_divider(CPoly *p1); // checks if poly used as divider before
	int generate_bsp_tree(CBSPNode *bsp_node, CPolygonList *p_list); // generate the bsp tree
	float chop_line(CVert3D *v1, CVert3D *v2, CPlane pl, CVert3D &out);
	int split_polygon(CPoly *divider, CPoly *in, CPoly *front, CPoly *back);
	void write_material_list(void);
	void write_vertex_list(void);
	void write_tverts_list(void);
	void write_normal_list(void);
	void write_plane_list(void);
	void write_light_list();
	void write_polygon(CPoly *p);
	void write_branch(CBSPNode *bsp);
	void write_bsp_node(CBSPNode *bsp);
	void read_material_list(void);
	void read_vertex_list(void);
	void read_tverts_list(void);
	void read_normal_list(void);
	void read_plane_list(void);
	void read_light_list(void);
	void read_divider(CPoly *p);
	void read_polygon(CPoly *p);
	void read_bsp_node(CBSPNode *bsp);

	void debug_dump_poly(CPoly *p);
public:
	void set_material_list(Material *ml) { material_list = ml; }
	void set_vertex_list(CVert3D **vl);
	void set_tverts_list(CVert3D **tl);
	void set_normal_list(CVert3D **nl)   { normal_list   = nl; }
	void set_light_list(CLight **ll)     { light_list    = ll; }
	void set_num_materials(int nm)       { num_materials = nm; }
	void set_num_vertexs(int nv)         { num_vertexs   = nv; }
	void set_num_tverts(int nt)          { num_tverts    = nt; }
	void set_num_normals(int nn)         { num_normals   = nn; }
	void set_num_lights(int nl)          { num_lights    = nl; }
	int get_num_materials(void)          { return num_materials; }
	int get_num_vertexs(void)            { return num_vertexs; }
	int get_num_tverts(void)             { return num_tverts; }
	int get_num_normals(void)            { return num_normals; }
	int get_num_planes(void)             { return num_planes; }
	int get_num_lights(void)             { return num_lights; }
	Material *get_material(int i)        { return &material_list[i]; }
	CVert3D **get_vertex_list(void)        { return vertex_list; }
	CVert3D **get_tverts_list(void)        { return tverts_list; }
	CVert3D **get_normal_list(void)        { return normal_list; }
	CPlane  **get_plane_list(void)         { return plane_list;  }
	CLight  **get_light_list(void)         { return light_list;  }
	CBSPNode *poly_which_node(CBSPNode *bsp, CPoly *p, int frontorback);
	CBSPNode *get_bsp(void)            { return bsp_tree;    }
	int create_bsp_tree(int num_polygons, CPoly **polygons);
	int write_bsp_tree(char *filename);
	int read_bsp_tree(char *filename);
	int recusive_count(CBSPNode *bsp, int total_so_far);
	int count_polys();
	CBSPNode *which_node(CVert3D loc, int &node_counter);
	CBSPNode *check_node(CBSPNode *bsp, CVert3D loc, int &node_counter);
	void create_node_list(CBSPNode *node, CBSPNode **nodelist, int *count);
};

#endif

// END OF FILE -----------------------------------------------------------