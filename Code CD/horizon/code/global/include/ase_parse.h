/*	--------------------------------------------------------------------------------- 
	Filename      - ase_parse.h 
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - CAseParser - parse an ASE file extracts info and creates an HRZ
	                file.
	--------------------------------------------------------------------------------- */

#ifndef _ASE_PARSE_H_
#define _ASE_PARSE_H_

#include <parse.h>
#include <types.h>
#include <light.h>


// --------------------------------------------------------------------------------- 
// predefined limits
// --------------------------------------------------------------------------------- 
#define MAX_OBJECTS      2000
#define MAX_VERTS        10000
#define MAX_LIGHTS       100

// --------------------------------------------------------------------------------- 
// CAseParser
// --------------------------------------------------------------------------------- 
class CAseParser : public CParse {

public:
	int        num_materials;
	int        num_submaterials;
	int        num_objects;
	int        num_lightobjects;
	Material   *materials;	
	GeomObject *objs;
	CVert3D    *new_vert_list[MAX_VERTS];
	CLight     **lights;
	// --------------------------------------------------------------------------------- 
	// ignored now
	// --------------------------------------------------------------------------------- 
	int        oldversion;

public:

private:
	int get_material(int i); // gets a single material
	int get_materials(); // gets all the materials
	int get_mesh(Mesh *mesh);
	int get_textures(Mesh *mesh); // gets all the texture info for a mesh
	int get_vert(CVert3D *vert);
	int get_face(Face *face);
	int get_normal(CVert3D *norm);
	int get_color(Color *col);
	int get_pos(CVert3D *pos);


	int add_geomobject();
	int add_lightobject();
	int get_type();

public:
#ifdef DEBUG
	CAseParser() : CParse("Ase2Hrz.dat") {}
#else
	CAseParser() : CParse() { oldversion = 0;}
#endif

	// --------------------------------------------------------------------------------- 
	// parse ASE file filename into list of objects objs
	// --------------------------------------------------------------------------------- 
	int parse_file(char *filename, GeomObject *objs);

	// --------------------------------------------------------------------------------- 
	// output the created HRZ file
	// --------------------------------------------------------------------------------- 
	int output_hrz(char *filename);

};

#endif

// END OF FILE -------------------------------------------------------------------