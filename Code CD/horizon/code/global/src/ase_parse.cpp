/*	--------------------------------------------------------------------------------- 
	Filename      - ase_parse.cpp
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Implementation of CAseParser -> subclass of CParse
					Code to parse ASE files, which are the model format exported by
					3D Studio Max. 
	--------------------------------------------------------------------------------- */

#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <debug.h>
#include <types.h>
#include <parse.h>
#include <ase_parse.h>
#include <hrz_parse.h>

// --------------------------------------------------------------------------------- 
// grabs vertex
// --------------------------------------------------------------------------------- 
int CAseParser::get_vert(CVert3D *vert)
{
	char entity_name[255];
	int vert_num = -1;

	read_line();
	sscanf(line, "%s %d %f %f %f", entity_name, &vert_num, &vert->x, &vert->y, &vert->z);

	return vert_num;
}

// --------------------------------------------------------------------------------- 
// grabs face/polygon
// --------------------------------------------------------------------------------- 
int CAseParser::get_face(Face *face)
{
	char entity_name[255];
	char temp[255];
	int tempi;
	
	read_line();
	sscanf(line, "%s %s %s %d %s %d %s %d %s %d %s %d %s %d %s %d %s %d\n", 
		entity_name, temp, temp, &face->v0, temp, &face->v1, temp, &face->v2,
		temp, &tempi, temp, &tempi, temp, &tempi, temp, &tempi, temp, &face->material_id);
	return 1;
}

// --------------------------------------------------------------------------------- 
// grabs normal
// --------------------------------------------------------------------------------- 
int CAseParser::get_normal(CVert3D *norm)
{
	char entity_name[255];
	int norm_num = -1;

	read_line();
	sscanf(line, "%s %d %f %f %f", entity_name, &norm_num, &norm->x, &norm->y, &norm->z);

	return norm_num;
}

// --------------------------------------------------------------------------------- 
// grabs postion
// --------------------------------------------------------------------------------- 
int CAseParser::get_pos(CVert3D *pos)
{
	char entity_name[255];

	read_line();
	sscanf(line, "%s %f %f %f", entity_name, &pos->x, &pos->y, &pos->z);

	return 1;

}


// --------------------------------------------------------------------------------- 
// grabs colour
// --------------------------------------------------------------------------------- 
int CAseParser::get_color(Color *col)
{
	char entity_name[255];
	read_line();
	sscanf(line, "%s %f %f %f", entity_name, &col->r, &col->g, &col->b);
	if(!cmp("*WIREFRAME_COLOR")) {
		printf("Expected wireframe color got instead %s\n", line);
		return 0;
	}
	return 1;
}


// --------------------------------------------------------------------------------- 
// gets a material 
// --------------------------------------------------------------------------------- 
int CAseParser::get_material(int i)
{
	if(num_submaterials > 0) {
		skip_until("*SUBMATERIAL");	
	} else {
		skip_until("*MATERIAL");
	}
	get_string(materials[i].name);
	skip_until("*MAP_AMOUNT"); // skip everthing until u find bitmap info (which is line after this)
	get_string(materials[i].filename);
	skip_until("}");
	read_line(); // } end of material
	materials[i].num = i;
	return 1;
}

// --------------------------------------------------------------------------------- 
// gets all materials
// --------------------------------------------------------------------------------- 
int CAseParser::get_materials(void)
{
	num_materials = 0;
	num_submaterials = 0;
	read_line(); // *MATERIAL_LIST {
	if(!cmp("*MATERIAL_LIST {")) {
		printf("Could not file Material List token found %s instead.\n", line);
		return -1;
	}	
	
	save_point();
	if(skip_until("*NUMSUBMTLS") != -1) {
		char strbuf[255];
		sscanf(line, "%s %d", strbuf, &num_submaterials);
		printf("There are %d sub materials (%s)\n", num_submaterials,line);
		num_materials = num_submaterials;
	} else {
		restore_point();
		// get the number of materials in the list
		num_materials = get_int();
		printf("There are %d materials listed (%s)\n", num_materials, line);
	}

	if((materials = (Material *) malloc(sizeof(Material) * num_materials)) == NULL) {
		printf("[ :( ] Can not allocate memory to hold all the materials.\n");
		return -1;
	}
	
	// get each material structure
	for(int i=0;i<num_materials;i++) {
		if(get_material(i) == -1) return -1;
	}

	read_line(); // }

	if(num_submaterials != 0) {
		read_line(); // }
	}

	return num_materials;
}

// --------------------------------------------------------------------------------- 
// gets each texture
// --------------------------------------------------------------------------------- 
int CAseParser::get_textures(Mesh *mesh)
{
	char buf[255];
	int skip;

	// read number of vertices
	// get the number of t-verts
	mesh->num_tverts = get_int();
	
	if((mesh->tverts = (CVert3D **) malloc(sizeof(CVert3D) * mesh->num_tverts)) == NULL) {
		printf("unable to allocate memory for texture verts\n");
		return -1;
	}

	// read number of texture coordinates
	read_line(); // *MESH_TVERTLIST {
	if(!cmp("*MESH_TVERTLIST {")) {
		printf("ERROR: expected tvert list instead %s\n", line);
		return -1;
	}	
	for(int i=0;i<mesh->num_tverts;i++) {
		mesh->tverts[i] = new CVert3D();
		get_vert(mesh->tverts[i]);
	}
	read_line(); // }

	// read in each face
	if(get_int() != mesh->num_faces) { // get the number of t_faces check if = to faces
		printf("ERROR: Texture faces do not equal number of faces.\n");
		return -1;
	}

	// read in all the texture coordinate faces
	read_line();
	if(!cmp("*MESH_TFACELIST {")) {
		printf("ERROR: expected tface list instead %s\n", line);
		return -1;
	}

	// read in each mesg
	for(i=0;i<mesh->num_faces;i++) {
		read_line();
		sscanf(line, "%s %d %d %d %d\n", buf, &skip, &mesh->faces[i].tv0, &mesh->faces[i].tv1, &mesh->faces[i].tv2);
	}

	read_line(); // }

	return 1;
}	

// --------------------------------------------------------------------------------- 
// parses an ASE mesh
// --------------------------------------------------------------------------------- 
int CAseParser::get_mesh(Mesh *mesh)
{
	dbp("MESH\n");
	read_line();
	if(!cmp("*MESH")) {
		printf("expected *MESH got %s\n", line);
		return -1;
	}

	read_line(); // ignore timevalue
	mesh->num_verts = get_int();
	mesh->num_faces = get_int();
	if(verbose_output) {
		printf("- num verts %d\n", mesh->num_verts);
		printf("- num faces %d\n", mesh->num_faces);
	}

	if((mesh->verts = (CVert3D **) malloc(sizeof(CVert3D *) * mesh->num_verts)) == NULL) {
		printf("unable to allocate memory for verts %d\n", mesh->num_verts);
		return -1;
	}
	if((mesh->vert_normals = (CVert3D **) malloc(sizeof(CVert3D *) * mesh->num_verts)) == NULL) {
		printf("unable to allocate memory for vert normals\n");
		return -1;
	}
	if((mesh->faces = (Face *) malloc(sizeof(Face) * mesh->num_faces)) == NULL) {
		printf("unable to allocate memory for faces\n");
		return -1;
	}

	// read in the verts
	read_line();
	if(!cmp("*MESH_VERTEX_LIST")) {
		printf("expected vertex list got %s\n", line);
		return 0;
	}
	for(int i=0;i<mesh->num_verts;i++) {
		mesh->verts[i] = new CVert3D();
		get_vert(mesh->verts[i]);	
		if(verbose_output)
			printf("Vert %d (%f,%f, %f)\n", i, mesh->verts[i]->x, mesh->verts[i]->y, mesh->verts[i]->z);
	}
	read_line();

	// read in the faces
	read_line(); // { //
	if(!cmp("*MESH_FACE_LIST")) {
		printf("expect face list got %s\n", line);
		return 0;
	}
	for(int j=0;j<mesh->num_faces;j++) {
		get_face(&mesh->faces[j]);
	}	

	
	read_line(); // } //

	if(get_textures(mesh) == -1) return -1; // get the texture information for this mesh

	read_line(); // *MESH_NUMCVERTEX
	read_line(); // } end of mesh //	

	return 1;
}	

// --------------------------------------------------------------------------------- 
// only need to read GEOMOBJECTS and LIGHTOBJECT from ASE
// --------------------------------------------------------------------------------- 
#define ASE_GEOMOBJECT  1
#define ASE_LIGHTOBJECT 2
#define ASE_EOF         3

// --------------------------------------------------------------------------------- 
// which type of information is next
// --------------------------------------------------------------------------------- 
int CAseParser::get_type(void)
{
	if(read_line() == -1) return ASE_EOF;

	if(cmp("*GEOMOBJECT")) {
		return ASE_GEOMOBJECT;
	}

	if(cmp("*LIGHTOBJECT")) {
		return ASE_LIGHTOBJECT;
	}

	return -1;
}

// --------------------------------------------------------------------------------- 
// add a new polygon based object
// --------------------------------------------------------------------------------- 
int CAseParser::add_geomobject()
{
		if(verbose_output)
			printf("Geomobject number %d being read\n", num_objects);
	
		get_string(objs[num_objects].name);
		if(verbose_output)
			printf("object name is %s\n", objs[num_objects].name);
		// need to check if object can be split by bsp processes
		if(strncmp("static", objs[num_objects].name, 6) == 0) {
			if(verbose_output)
				printf("Static Object Found\n");
			objs[num_objects].is_static = 1;
		} else {
			objs[num_objects].is_static = 0;
		}
	
		int skip_amount;
		read_line();
		if(cmp("*NODE_PARENT")) skip_amount = 16;
			else skip_amount = 15;

		// skip *NODE_TM stuff
		for(int i=0;i<skip_amount;i++) 
			read_line();

		// read mesh 
		if(get_mesh(&objs[num_objects].mesh) == -1)
			return -1;
	
		// ignore next 3 entities
		read_line(); read_line(); read_line();

		// material reference for object
		objs[num_objects].material_id = get_int();

		// set all the faces material id to this one
		for(i=0;i<objs[num_objects].mesh.num_faces;i++) {
			objs[num_objects].mesh.faces[i].material_id = objs[num_objects].material_id;
		}

		read_line(); // } end of object //

		num_objects++;

	return 1;
}

// --------------------------------------------------------------------------------- 
// add a new light to the environment
// --------------------------------------------------------------------------------- 
int CAseParser::add_lightobject()
{
	lights[num_lightobjects] = new CLight();

	get_string(lights[num_lightobjects]->name);
	if(verbose_output)
		printf("LightObjects name is %s\n", lights[num_lightobjects]->name);

	char type_str[30];
	get_string(type_str);
	if(strncmp(type_str, "Omni", 4) == 0) 
		lights[num_lightobjects]->type = LIGHT_TYPE_OMNI;

	// Check to see if light is the definition of ambient light
	if(strncmp(lights[num_lightobjects]->name, "ambientlight", 12) == 0) {
		lights[num_lightobjects]->type = LIGHT_TYPE_AMBIENT;
	}

	skip_until("*TM_ROW3");
	get_pos(&lights[num_lightobjects]->pos);	
	if(verbose_output)
		printf("Light Position is %f,%f,%f\n", lights[num_lightobjects]->pos.x, lights[num_lightobjects]->pos.y, lights[num_lightobjects]->pos.z);

	skip_until("*TIMEVALUE");

	// get the lights colour
	get_pos((CVert3D*)&lights[num_lightobjects]->color);
	if(verbose_output)
		printf("Light Color is %f %f %f\n", lights[num_lightobjects]->color[0], lights[num_lightobjects]->color[1], lights[num_lightobjects]->color[2]); 

	// get the lights intensity
	lights[num_lightobjects]->intesity = get_float();	

	read_line();
	read_line();

	lights[num_lightobjects]->attenuation = get_float();
	
	skip_until("*LIGHT_RAYBIAS");
	read_line();
	read_line();

	num_lightobjects++;
	return 1;
}

// --------------------------------------------------------------------------------- 
// parse a ASE file
// --------------------------------------------------------------------------------- 
int CAseParser::parse_file(char *filename, GeomObject *objects)
{
	if(!open_file(filename)) 
		return 0;

	// check if ASE file
	read_line();		
	if(!cmp("*3DSMAX_ASCIIEXPORT")) {
		printf("Input file does not seem to be an ASE file.\n");
		return 0;
	}

	// ignore ASE header info
	for(int i=0;i<10;i++) {
		read_line();		
	}

	if((objects = (GeomObject*) malloc(sizeof(GeomObject) * MAX_OBJECTS)) == NULL) {
		printf("[ :( ] Can not allocate memory for %d GeomObject's\n");
		return 0;
	}
	if((lights = (CLight**) malloc(sizeof(CLight*) * MAX_LIGHTS)) == NULL) {
		printf("[ :( ] Can not allocate memory for %d LightObject's\n");
		return 0;
	}

	this->objs       = objects;
	num_objects      = 0;
	num_lightobjects = 0;

	// materials always first!
	if(get_materials() == -1) 
		return 0;

	int object_type;
	while((object_type = get_type()) != ASE_EOF) {
		switch(object_type) {
		case ASE_LIGHTOBJECT:
			add_lightobject();
			break;
		case ASE_GEOMOBJECT:
			add_geomobject();
			break;
		default:
			printf("[ :( ] Unknown Type in ase file :%s:\n", line);
			return -1;
			exit(0);
		}
	}
	
//	if(get_objects() == -1) return 0;

	return 1;
}

// --------------------------------------------------------------------------------- 
// output the parsed HRZ file as an ASE file
// --------------------------------------------------------------------------------- 
int CAseParser::output_hrz(char *filename)
{
	int total_verts = 0, total_faces = 0, total_tverts = 0, count = 0;
	char strbuf[100];

	o_fd = open(filename, _O_WRONLY | _O_CREAT | _O_TRUNC, _S_IWRITE);

	if(o_fd < 0) {
		printf("Can not open output file %s\n", filename);
		return 0;
	}

	for(int i=0;i<num_objects;i++) {
		objs[i].mesh.vert_offset = total_verts; // set the offset
		objs[i].mesh.face_offset = total_faces; // set the offset
		objs[i].mesh.tvert_offset = total_tverts; // set the offset;
		total_verts += objs[i].mesh.num_verts;
		total_faces += objs[i].mesh.num_faces;
		total_tverts += objs[i].mesh.num_tverts;
	}

	// header
	write(o_fd, HRZ_HEADER, strlen(HRZ_HEADER));
	
	// Materials
	sprintf(strbuf, "\nNumberMaterials %d\n", num_materials);
	write(o_fd, strbuf, strlen(strbuf));
	
	for(i=0;i<num_materials;i++) {
		sprintf(strbuf, "MaterialNum %d \nMaterialName ", materials[i].num);
		write(o_fd, strbuf, strlen(strbuf));			
		for(int j=0;j<(int)strlen(materials[i].name);j++) {
			write(o_fd, &materials[i].name[j], 1);
		}
		sprintf(strbuf, "\nMaterialFilename ");
		write(o_fd, strbuf, strlen(strbuf));
		for(j=0;j<(int)strlen(materials[i].filename);j++) {
			write(o_fd, &materials[i].filename[j], 1);
		}
		write(o_fd, "\n", 1);
	}

	// Vertices --------------------------------------------------
	sprintf(strbuf, "\nNumberVerts %d\n", total_verts);
	write(o_fd, strbuf, strlen(strbuf));


	for(i=0;i<num_objects;i++) {
		for(int j=0;j<objs[i].mesh.num_verts;j++) {
			if(!oldversion) {
				float temp = objs[i].mesh.verts[j]->y;
				float y = objs[i].mesh.verts[j]->z;
				float z = -temp;
				float x = objs[i].mesh.verts[j]->x;
				sprintf(strbuf, "%05d %f %f %f \n", count++, x,y,z);
			} else {
				sprintf(strbuf, "%05d %f %f %f \n", count++, objs[i].mesh.verts[j]->x, 
								   							         objs[i].mesh.verts[j]->y, 
																	 objs[i].mesh.verts[j]->z);
			}
			write(o_fd, strbuf, strlen(strbuf));
		}
	}


	
	// Texture Vertices ------------------------------------------
	count = 0;
	sprintf(strbuf, "\nNumberTextureVerts %d\n", total_tverts);
	write(o_fd, strbuf, strlen(strbuf));

	for(i=0;i<num_objects;i++) {
		for(int j=0;j<objs[i].mesh.num_tverts;j++) {
			if(!oldversion) {
				sprintf(strbuf, "%05d %f %f %f \n", count++, objs[i].mesh.tverts[j]->x, 
								   							   objs[i].mesh.tverts[j]->y * -1, 
															   objs[i].mesh.tverts[j]->z);
			} else {
				sprintf(strbuf, "%05d %f %f %f \n", count++, objs[i].mesh.tverts[j]->x, 
								   							   objs[i].mesh.tverts[j]->y, 
															   objs[i].mesh.tverts[j]->z);
			}
			write(o_fd, strbuf, strlen(strbuf));
		}
	}

	// faces
	sprintf(strbuf, "\nNumberFaces %d\n", total_faces);
	write(o_fd, strbuf, strlen(strbuf));
	
	count = 0;
	for(i=0;i<num_objects;i++) {
		for(int j=0;j<objs[i].mesh.num_faces;j++) {

			// format for FACE is: num v1 v2 v3 tv1 tv2 tv3 material_ref
			sprintf(strbuf, "%05d %5d %5d %5d %5d %5d %5d %3d %5d %8d\n", count, 
							objs[i].mesh.faces[j].v0  + objs[i].mesh.vert_offset,
							objs[i].mesh.faces[j].v1  + objs[i].mesh.vert_offset,
							objs[i].mesh.faces[j].v2  + objs[i].mesh.vert_offset,							
							objs[i].mesh.faces[j].tv0 + objs[i].mesh.tvert_offset,
							objs[i].mesh.faces[j].tv1 + objs[i].mesh.tvert_offset,
							objs[i].mesh.faces[j].tv2 + objs[i].mesh.tvert_offset,
							objs[i].mesh.faces[j].material_id, // sub material							
							count, // lightmap id
							objs[i].is_static);
							//objs[i].material_id);			// main material		
									
			count++;
																 
			write(o_fd, strbuf, strlen(strbuf));
		}
	}
	
	// Lights ------------------------------------------------
	sprintf(strbuf, "\nLights %d\n", num_lightobjects);
	write(o_fd, strbuf, strlen(strbuf));

	for(i=0;i<num_lightobjects;i++) {
		if(oldversion) {
			sprintf(strbuf, "%d %.4f %.4f %.4f %d %f %f %f\n", i, lights[i]->pos.x, lights[i]->pos.y, lights[i]->pos.z, lights[i]->type, lights[i]->color[0], lights[i]->color[1], lights[i]->color[2]);
		} else {
			float temp = lights[i]->pos.y;
			float y = lights[i]->pos.z;
			float z = -temp;
			float x = lights[i]->pos.x;
			sprintf(strbuf, "%d %.4f %.4f %.4f %d %f %f %f %f %f\n", i, x,y,z, lights[i]->type, lights[i]->color[0], lights[i]->color[1], lights[i]->color[2], lights[i]->intesity, lights[i]->attenuation);			
		}
		write(o_fd, strbuf, strlen(strbuf));

	}
	close(o_fd);
	return 1;
}

// END OF FILE -----------------------------------------------------------------

