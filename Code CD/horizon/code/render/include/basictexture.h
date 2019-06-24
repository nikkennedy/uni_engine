/*	--------------------------------------------------------------------------------- 
	Filename      - basictexture.h
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Class definition of CBasicTexture
					provides for basic red,green,blue textures
	--------------------------------------------------------------------------------- */

#ifndef _BASIC_TEXTURE_H_
#define _BASIC_TEXTURE_H_

#include <types.h>
#include <texture.h>

class CBasicTexture : public CTexture {
public:
	CBasicTexture(char *filename, char *texture_name);
	~CBasicTexture();
	void process_vertex(CVert3D *v);
	void bind_texture();
};

#endif

// END OF FILE ----------------------------------------------------------------------
