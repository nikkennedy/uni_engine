/*	--------------------------------------------------------------------------------- 
	Filename      - invistexture.h
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Class definition of CInvisTexture
					provides for basic red,green,blue textures but with added holes in
	--------------------------------------------------------------------------------- */

#ifndef _INVIS_TEXTURE_H_
#define _INVIS_TEXTURE_H_

#include <types.h>
#include <texture.h>

class CInvisTexture : public CTexture {
public:
	CInvisTexture(char *filename, char *texture_name, CVert3D invis_color);
	~CInvisTexture();
	void process_vertex(CVert3D *v);
	void bind_texture();
};

#endif

// END OF FILE ----------------------------------------------------------------------
