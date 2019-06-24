/*	--------------------------------------------------------------------------------- 
	Filename      - main.cpp
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - frontend to the BSP tree creation process
	--------------------------------------------------------------------------------- */

#include <windows.h>
#include <stdio.h>

#include <lbsp.h>
#include <debug.h>
#include <hrz_parse.h>

// --------------------------------------------------------------------------------- 
// main
// --------------------------------------------------------------------------------- 
int main(int argc, char **argv)
{
	// --------------------------------------------------------------------------------- 
	// need to be able to parse the input file
	// --------------------------------------------------------------------------------- 
	CHrzParser *hrz_parser = new CHrzParser();
	
#ifdef DEBUG
	c_debug = new CDebug();
	c_debug->debug_init("leafybsp.dat");
#endif

	printf("Leafy BSP Tree Compiler by Nicholas Kennedy (C) 2002\n\n");

	if(argc < 2) {
		printf("Usage: <infile.hrz> <outfile.bsp>\n\n");
		return 0;
	}

	printf("in: %s\n", argv[1]);
	printf("out: %s\n", argv[2]);

	dbp("LBSP Debug Started\n");
		
	float start_time = GetTickCount();

	// --------------------------------------------------------------------------------- 
	// parse HRZ
	// --------------------------------------------------------------------------------- 
	if(!hrz_parser->parse_file(argv[1])) {
		printf("Can not parse input file %s\n", argv[1]);
		return 0;
	}

	// --------------------------------------------------------------------------------- 
	// create new BSP tree 
	// --------------------------------------------------------------------------------- 
	CLBSP *c_lbsp_creator = new CLBSP();	

	// --------------------------------------------------------------------------------- 
	// set relevant pointers
	// --------------------------------------------------------------------------------- 
	c_lbsp_creator->set_num_vertexs  (hrz_parser->get_num_verts()         );
	c_lbsp_creator->set_num_tverts   (hrz_parser->get_num_tverts()        );
	c_lbsp_creator->set_num_normals  (hrz_parser->get_num_normals()       );
	c_lbsp_creator->set_num_materials(hrz_parser->get_num_materials()     );
	c_lbsp_creator->set_num_lights   (hrz_parser->get_num_lights()        );
	c_lbsp_creator->set_vertex_list  (hrz_parser->get_vertexlist()        );
	c_lbsp_creator->set_tverts_list  (hrz_parser->get_texture_vertexlist());
	c_lbsp_creator->set_normal_list  (hrz_parser->get_normal_list()       );
	c_lbsp_creator->set_material_list(hrz_parser->get_materials()         );
	c_lbsp_creator->set_light_list   (hrz_parser->get_light_list()        );

	// --------------------------------------------------------------------------------- 
	// create the BSP tree
	// --------------------------------------------------------------------------------- 
	c_lbsp_creator->create_bsp_tree(hrz_parser->get_num_polygons(), hrz_parser->get_polygons());
	                       
	// --------------------------------------------------------------------------------- 
	// save the BSP tree to file
	// --------------------------------------------------------------------------------- 
	printf("There were %d static polygons processed.\n", c_lbsp_creator->skipping_static_poly_count);
	printf("Saving BSP.\n");
	c_lbsp_creator->write_bsp_tree(argv[2]);

	float end_time = GetTickCount();
	printf("BSP Tree completed in %.1f mins (%.2f secs).\n", ((end_time - start_time) / 1000.0f)/60,((end_time - start_time) / 1000.0f));	
	
	return 1;
}

// END OF FILE ----------------------------------------------------------------------