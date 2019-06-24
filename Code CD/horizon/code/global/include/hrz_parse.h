/*	--------------------------------------------------------------------------------- 
	Filename      - hrz_parse.h
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - CHrzParser definition
	                Parses a HRZ file into list of verts, materials, polygons etc
					provides methods to access these lists
	--------------------------------------------------------------------------------- */

#ifndef _HRZ_PARSE_H_
#define _HRZ_PARSE_H_

#include <parse.h>
#include <types.h>
#include <light.h>

// --------------------------------------------------------------------------------- 
// header
// --------------------------------------------------------------------------------- 
#define HRZ_HEADER       "HRZ File Version 1.0\n"

// --------------------------------------------------------------------------------- 
// CHrzParser
// --------------------------------------------------------------------------------- 
class CHrzParser: public CParse {
private:
	// --------------------------------------------------------------------------------- 
	// the containers for vertices, polgons, lights etc
	// --------------------------------------------------------------------------------- 
	int num_materials;
	int num_verts;
	int num_tverts;
	int num_faces;
	int num_lights;
	CVert3D **verts;
	CVert3D **tverts;
	CVert3D **normals;
	Face *faces;
	CPoly **polys;
	Material *materials;
	CLight **lights;	
	
private:
	// --------------------------------------------------------------------------------- 
	// functions used to read .HRZ file
	// --------------------------------------------------------------------------------- 
	int get_vert(CVert3D *vert);
	int get_face(Face *face);
	int get_material(Material *mat);
	int get_light(CLight *l);


public:
	// --------------------------------------------------------------------------------- 
	// constructor/destructor
	// --------------------------------------------------------------------------------- 
	CHrzParser() : CParse() {}
	~CHrzParser();

	// --------------------------------------------------------------------------------- 
	// parse file filename
	// --------------------------------------------------------------------------------- 
	int parse_file(char *filename);

	// --------------------------------------------------------------------------------- 
	// acessor methods
	// --------------------------------------------------------------------------------- 
	CPoly **get_polygons() { return polys; }
	int get_num_polygons() { return num_faces; }

	Material *get_materials() { return materials; }
	int get_num_materials() { return num_materials; }
	Material *CHrzParser::get_material(int i);


	CVert3D **get_vertexlist() { return verts; }
	int get_num_verts() { return num_verts; }

	CVert3D **get_texture_vertexlist() { return tverts; }
	int get_num_tverts() { return num_tverts; }

	CVert3D **get_normal_list() { return normals; }
	int get_num_normals() { return num_faces; }

	CLight  **get_light_list() { return lights; }
	int get_num_lights() { return num_lights; }
};

#endif

// END OF FILE ------------------------------------------------------------------------