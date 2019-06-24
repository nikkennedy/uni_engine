/*	--------------------------------------------------------------------------------- 
	Filename      - hrz_parse.cpp
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Implementation of CHrzParser -> subclass of CParse
					Code to parse HRZ files, which are the model format for Horizon 
	--------------------------------------------------------------------------------- */

#include <parse.h>
#include <types.h>
#include <hrz_parse.h>
#include <debug.h>

// --------------------------------------------------------------------------------- 
// grabs vertex_id, x, y, z (a vertex)
// --------------------------------------------------------------------------------- 
int CHrzParser::get_vert(CVert3D *vert)
{
	int vert_num;
	read_line();
	sscanf(line, "%d %f %f %f", &vert_num, &vert->x, &vert->y, &vert->z);
	return vert_num;
}

// --------------------------------------------------------------------------------- 
// grabs light_id, x, y, z, light_type, r, g, b, intensity, attenuation (a light)
// --------------------------------------------------------------------------------- 
int CHrzParser::get_light(CLight *l)
{
	int lnum;
	read_line();
	sscanf(line, "%d %f %f %f %d %f %f %f %f %f", &lnum, &l->pos.x, &l->pos.y, &l->pos.z, &l->type, &l->color[0], &l->color[1], &l->color[2], &l->intesity, &l->attenuation);
	return lnum;
}

// --------------------------------------------------------------------------------- 
// grabs face_id, 
//		 vertex_index0, 
//		 vertex_index1, 
//		 vertex_index2, 
//		 texture_index0,
//		 texture_index1,
//		 texture_index2,
//       material_id,
//	     lightmap_id,
//       is_poly_static (used by BSP tree compiler)
// (A face/polygon)
// --------------------------------------------------------------------------------- 
int CHrzParser::get_face(Face *face)
{
	int face_num;
	read_line();
	sscanf(line, "%d %d %d %d %d %d %d %d %d %d", &face_num, 
		&face->v0,  &face->v1,  &face->v2,
		&face->tv0, &face->tv1, &face->tv2,
		&face->material_id, 
		&face->lightmap_id,
		&face->is_static);
	return face_num;	
}

// --------------------------------------------------------------------------------- 
// grabs material_name, corresponding filename (material/texture)
// --------------------------------------------------------------------------------- 
int CHrzParser::get_material(Material *mat)
{
	mat->num = get_int();
	get_string(mat->name);
	get_string(mat->filename);	
	return 1;
}

// --------------------------------------------------------------------------------- 
// returns the material stored at index i
// --------------------------------------------------------------------------------- 
Material *CHrzParser::get_material(int i)
{
	return &materials[i];
}

// --------------------------------------------------------------------------------- 
// This function parses a whole .HRZ file, storing all the important data in the
// correct variables.
// --------------------------------------------------------------------------------- 
int CHrzParser::parse_file(char *filename)
{
	dbp("HRZ Parser: START ---------------------------------------\n");
	
	// open file return 0 on error
	if(!open_file(filename)) {
		dbp("HRZ parser: can not open file %s\n", filename);
		printf("[ :( ]: Can not open %s\n", filename);
		return 0;
	}

	// check header to make sure this is a HRZ file
	read_line();
	if(!cmp("HRZ")) {
		dbp("File is not a HRZ file\n");
		printf("[ :( ] Not a HRZ file\n");
		return 0;
	}

	// Read Materials ------------------------------------------------
	num_materials = get_int();
	if((materials = (Material *) malloc(sizeof(Material) * num_materials)) == NULL) {
		dbp("Can not allocate memory for materials.\n");
		return 0;
	}
	for(int i=0;i<num_materials;i++) {
		get_material(&materials[i]);
	}

	// Read Vertices -------------------------------------------------
	num_verts = get_int();
	dbp("HRZ Parser: No. of verts %d\n", num_verts); // * 2 to add enough space for verts created when polys spliy
	if((verts = (CVert3D **) malloc(sizeof(CVert3D*) * num_verts * 2)) == NULL) {
		dbp("can not allocate memory for verts %d\n", num_verts);
		return 0;
	}
	for(i=0;i<num_verts;i++) {
		verts[i] = new CVert3D();
		get_vert(verts[i]);
	}

	// Read Texture Vertices -----------------------------------------
	num_tverts = get_int();
	dbp("HRZ Parser: No. tverts %d\n", num_tverts);
	if((tverts = (CVert3D **) malloc(sizeof(CVert3D*) * num_tverts)) == NULL) {
		dbp("can not allocate memory for texture verts\n");
		return 0;
	}
	for(i=0;i<num_tverts;i++) {
		tverts[i] = new CVert3D();
		get_vert(tverts[i]);
	}

	// Read Faces ----------------------------------------------------
	num_faces = get_int();
	dbp("HRZ Parser: No. polygons %d\n", num_faces);
	if((faces = (Face *) malloc(sizeof(Face) * num_faces)) == NULL) {
		dbp("can not allocate memory for faces\n");
		return 0;
	}	
	for(i=0;i<num_faces;i++) {
		get_face(&faces[i]);
	}

	if((normals = (CVert3D **) malloc(sizeof(CVert3D*) * num_faces)) == NULL) {
		dbp("can not allocate memory for normals\n");
		return 0;
	}

	// Create a list of polygons -------------------------------------
	if((polys = (CPoly **) malloc(sizeof(CPoly*) * num_faces)) == NULL) {
		dbp("can not allocate memory for polygons\n");
		return 0;
	}	

	for(i=0;i<num_faces;i++) {
		// copy verts
		polys[i] = new CPoly(faces[i].v0, faces[i].v1, faces[i].v2, faces[i].tv0, faces[i].tv1, faces[i].tv2);
		polys[i]->is_static = faces[i].is_static;
		polys[i]->lightmap_id = faces[i].lightmap_id;
		// calculate the Normal
		CVert3D v, u;
		
		u = *verts[polys[i]->vi[1]] - *verts[polys[i]->vi[0]];
		v = *verts[polys[i]->vi[2]] - *verts[polys[i]->vi[0]];

		CVert3D norm = u ^ v; // the cross product
		norm.normalise();
#ifdef FLIP_NORMALS
		norm = -norm;
#endif
		// rounding error
		if(norm.x < 0.000001 && norm.x > -0.000001) norm.x = 0;
		if(norm.y < 0.000001 && norm.y > -0.000001) norm.y = 0;
		if(norm.z < 0.000001 && norm.z > -0.000001) norm.z = 0;

//		dbp("Normal for poly %i is", i); norm.dump(); //dump_vertex(norm);
		
		normals[i] = new CVert3D(norm);
		polys[i]->ni = i;
		polys[i]->material_id = faces[i].material_id;
	}


	// Lights -------------------------------------------
	num_lights = get_int();
	dbp("HRZ Parser: No. Lights %d\n", num_lights);
	if((lights = (CLight **) malloc(sizeof(CLight*) * num_lights)) == NULL) {
		dbp("can not allocate memory for lights\n");
		return 0;
	}	

	for(i=0;i<num_lights;i++) {		
		lights[i] = new CLight();
		get_light(lights[i]);
	}

	printf("þ Verts     %5d\n", num_verts);
	printf("þ Materials %5d\n", num_materials);
	printf("þ Polys     %5d\n", num_faces);
	printf("þ Lights    %5d\n", num_lights);

	close_file();
	
	dbp("HRZ Parser: END -----------------------------------------\n");
	return 1;
}

// --------------------------------------------------------------------------------- 
// destructor, makes sure memory allocated is freed after use
// --------------------------------------------------------------------------------- 
CHrzParser::~CHrzParser()
{
	free(materials);

	for(int i=0;i<num_verts;i++) {
		delete verts[i];
	}

	free(verts);
	
	for(i=0;i<num_tverts;i++) {
		delete tverts[i];
	}

	free(tverts);

	for(i=0;i<num_faces;i++) {
		delete polys[i];
		delete normals[i];
	}

	free(faces);

	for(i=0;i<num_lights;i++) {		
		delete lights[i];		
	}

	free(lights);

}

// END OF FILE ----------------------------------------------------