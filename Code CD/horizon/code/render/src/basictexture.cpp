/*	--------------------------------------------------------------------------------- 
	Filename      - basictexture.cpp
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Class implementation of CBasicTexture
					provides for basic red,green,blue textures
	--------------------------------------------------------------------------------- */

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glext.h>

#include <basictexture.h>
#include <image.h>
#include <types.h>
#include <debug.h>

// --------------------------------------------------------------------------------- 
// constructor
// --------------------------------------------------------------------------------- 
CBasicTexture::CBasicTexture(char *filename, char *texture_name) : CTexture(filename, texture_name)
{
	if((data = (u_char *) malloc(cur_texture->get_width() * cur_texture->get_height() * 3)) == NULL) {
		cprt("Can not allocate memory for the texture\n");
		return;
	}

	memcpy(data, cur_texture->get_RGB_data(),(cur_texture->get_width() * cur_texture->get_height() * 3));

	height = cur_texture->get_height();
	width  = cur_texture->get_width();
	
	memset(name, '\0', 30);     // clear and set the texture name
	strcpy(name, texture_name); 

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

//	cprt("Texture ID %d, Loaded texture %s\n", id, texture_name);

	delete cur_texture;
}

// --------------------------------------------------------------------------------- 
// OpenGL binding
// --------------------------------------------------------------------------------- 
void CBasicTexture::bind_texture()
{
	glBindTexture(GL_TEXTURE_2D, id); // standard OpenGL binding
}

// --------------------------------------------------------------------------------- 
// OpenGL vertex
// --------------------------------------------------------------------------------- 
void CBasicTexture::process_vertex(CVert3D *v)
{
	glVertex3fv((float*)v); // standard OpenGL vertex drawing
}

// --------------------------------------------------------------------------------- 
// destructor
// --------------------------------------------------------------------------------- 
CBasicTexture::~CBasicTexture()
{
	unsigned int intarray[1] = { id };
	glDeleteTextures(1, intarray);
}

// END OF FILE ----------------------------------------------------------------------