/*	--------------------------------------------------------------------------------- 
	Filename      - alphatexture.cpp
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Class implementation of CAlphaTexture
					provides for textures with alpha values set
	--------------------------------------------------------------------------------- */

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glext.h>

#include <alphatexture.h>
#include <image.h>
#include <types.h>
#include <debug.h>
#include <hrz_config.h>

// --------------------------------------------------------------------------------- 
// constructor
// --------------------------------------------------------------------------------- 
CAlphaTexture::CAlphaTexture(char *filename, char *texture_name, CVert3D color) : CTexture(filename, texture_name)
{
	if((data = (u_char *) malloc(cur_texture->get_width() * cur_texture->get_height() * 4)) == NULL) {
		dbp("Can not allocate memory for the texture\n");
		return;
	}

	memcpy(data, cur_texture->get_alpha_data(color),(cur_texture->get_width() * cur_texture->get_height() * 4));

	height = cur_texture->get_height();
	width  = cur_texture->get_width();
	
	memset(name, '\0', 100);     // clear and set the texture name
	strcpy(name, texture_name); 

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

//	cprt("Texture ID %d, Loaded Alpha texture %s\n", id, name);

	delete cur_texture;

}

// --------------------------------------------------------------------------------- 
// OpenGL binding
// --------------------------------------------------------------------------------- 
void CAlphaTexture::bind_texture()
{
	glBindTexture(GL_TEXTURE_2D, id);
}

// --------------------------------------------------------------------------------- 
// OpenGL processing
// --------------------------------------------------------------------------------- 
void CAlphaTexture::process_vertex(CVert3D *v)
{
	glVertex3fv((float*)v); // standard OpenGL vertex drawing
}

// --------------------------------------------------------------------------------- 
// descructor
// --------------------------------------------------------------------------------- 
CAlphaTexture::~CAlphaTexture()
{
	unsigned int intarray[1] = { id };
	glDeleteTextures(1, intarray);
}

// END OF FILE -----------------------------------------------------------------------