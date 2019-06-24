/*	--------------------------------------------------------------------------------- 
	Filename      - md2.cpp
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Class implementation of  structures used to load a .MD2 model
	--------------------------------------------------------------------------------- */

#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>

#include <gl/gl.h>
#include <gl/glu.h>

#include <md2.h>
#include <ogl_texturemanager.h>

// --------------------------------------------------------------------------------- 
// free mem
// --------------------------------------------------------------------------------- 
CMD2::~CMD2()
{
	free(command_verts_type);
	free(command_verts);
}

// --------------------------------------------------------------------------------- 
// actually load the .MD2 file
// --------------------------------------------------------------------------------- 
int CMD2::load_md2(char *path, char *filename)
{
	char strbuffer[500];

	strcpy(strbuffer, path);
	strcat(strbuffer, filename);

	fp = fopen(strbuffer, "rb");

	fread(&header, sizeof(header), 1, fp);
	fseek(fp, header.offsetFrames, SEEK_SET);
	
	// read in each frame
	for(int i=0;i< header.numFrames; i++) {		
		fread(&frames[i], header.frameSize, 1, fp);			
		for(int j=0;j<header.numVertices;j++) {
			frames[i].verts[j] = CVert3D(frames[i].vertices[j].vertex[0] * frames[i].scale[0], 
									     frames[i].vertices[j].vertex[1] * frames[i].scale[1],
										 frames[i].vertices[j].vertex[2] * frames[i].scale[2]);
			// translate vertices according to specification
			frames[i].verts[j] = frames[i].verts[j] + CVert3D(frames[i].translate[0], frames[i].translate[1], frames[i].translate[2]);
		}
	}

	// read in all OpenGL helper commands
	fseek(fp, header.offsetGlCommands, SEEK_SET);

	command_verts_type = (int*) malloc(sizeof(int) * header.numGlCommands);
	command_verts = (glCommandVertex_t**) malloc(sizeof(glCommandVertex_t*) * header.numGlCommands);

	for(i=0;i<header.numGlCommands;i++) {
		int amount;
		fread(&amount, sizeof(int), 1, fp);			
		command_verts[i] = (glCommandVertex_t*) malloc(sizeof(glCommandVertex_t) * abs(amount));
		if(amount == 0) {
			command_verts_type[i] = -1;
			break;
		}
		if(amount > 0) { // Triangle Strip
			command_verts_type[i] = amount;
			
			for(int j=0;j< amount;j++) {
				fread(&command_verts[i][j], sizeof(glCommandVertex_t), 1, fp);
			}			
		} 
		if(amount < 0) { // Triangle fan
			command_verts_type[i] = amount;
			
			for(int j=0;j< abs(amount);j++) {
				fread(&command_verts[i][j], sizeof(glCommandVertex_t), 1, fp);
			}
		}		
	}

	fclose(fp);
	return 1;
}

// --------------------------------------------------------------------------------- 
// constructor
// --------------------------------------------------------------------------------- 
CMD2::CMD2() 
{ 
	cur_frame = 0;
}

// --------------------------------------------------------------------------------- 
// searches all the frames, for the one which has a name equal to frame_name
// --------------------------------------------------------------------------------- 
int CMD2::find_frame_num(char *frame_name)
{
	for(int i=0;i<header.numFrames;i++) {
		if(strncmp(frames[i].name, frame_name,strlen(frame_name))==0)
			return i;
	}

	return -1;
}

// END OF FILE ---------------------------------------------------------------------