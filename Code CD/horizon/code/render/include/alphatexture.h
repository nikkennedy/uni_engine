/*	--------------------------------------------------------------------------------- 
	Filename      - alphatexture.h
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Class definition of CAlphaTexture
					provides for textures with alpha values set
	--------------------------------------------------------------------------------- */

#ifndef _ALPHA_TEXTURE_H_
#define _ALPHA_TEXTURE_H_

#include <types.h>
#include <texture.h>

class CAlphaTexture : public CTexture {
public:
	CAlphaTexture(char *filename, char *texture_name, CVert3D color);
	~CAlphaTexture();
	void process_vertex(CVert3D *v);
	void bind_texture();
};

#endif

// END OF FILE ----------------------------------------------------------------------
