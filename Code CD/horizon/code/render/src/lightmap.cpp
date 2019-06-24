/*	--------------------------------------------------------------------------------- 
	Filename      - lightmap.cpp
	Last Modified - 09/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Implementation of CLightMapTexture
					this is a basic lightmap class, it is just another textureformat
					OpenGL specific
	--------------------------------------------------------------------------------- */

#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glext.h>

#include <types.h>
#include <texture.h>
#include <lightmap.h>
#include <console.h>

// --------------------------------------------------------------------------------- 
// constructor, opens and loads the lightmap file, creates the OpenGL texture object
// --------------------------------------------------------------------------------- 
CLightMapTexture::CLightMapTexture(char *filename, int width, int height)
{
	// open lightmap file (filename)
	int fd = open(filename, _O_RDONLY | _O_BINARY);

	// report error
	if(fd <= 0) {
//		cprt("error, can not open lightmap file %s\n", filename);
		return;
	}

	this->width = width;
	this->height = height;

	// allocate memory for texture data
	if((data = (u_char *) malloc(width * height * 3)) == NULL) {
		cprt("error, unable to allocate memory for lightmap %s\n", filename);
		return;
	}

	// speed up lightmap reading by loading whole file first
	u_char *whole_chunk = (u_char *) malloc(width * height * 3);
	if(whole_chunk == NULL) {
		cprt("error, unable to allocate memory for lightmap %s\n", filename);
		return;
	}

	if(read(fd, whole_chunk, width * height * 3) != width * height * 3) {
		cprt("error, lightmap file seems corrupted %s\n", filename);
		return;
	}

	int index = 0;

	// rearrange the data into the correct order
	for(int x =0;x<width;x++) {
		for(int y=0;y<height;y++) {
			int mul_y = y * width * 3;
			int mul_x = x * 3;

			data[mul_y+mul_x]   = whole_chunk[index++];
			data[mul_y+mul_x+1] = whole_chunk[index++];
			data[mul_y+mul_x+2] = whole_chunk[index++];
		}
	}

	// free up unneeded memory
	free(whole_chunk);
	// close file handle
	close(fd);

	// create the OpenGL texture object
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	
	glGenTextures(1, &id);

	if(id <= 0) {
		cprt("Error loading lightmap.\n");
	}

	// OpenGL renderer specific code
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
}

// --------------------------------------------------------------------------------- 
// Basic OpenGL binding code
// --------------------------------------------------------------------------------- 
void CLightMapTexture::bind_texture(void)
{
	glBindTexture(GL_TEXTURE_2D, id);	
}

// END OF FILE ---------------------------------------------------------------------
