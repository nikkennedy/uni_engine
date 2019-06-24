/*	--------------------------------------------------------------------------------- 
	Filename      - texturemanager.h 
	Last Modified - 09/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Class definition of CTextureManager 
					This is the super class of any texture management implementation
					COGLTextureManager is a sub class of this and implements an OpenGL
					representation of the texture management functionality.
	--------------------------------------------------------------------------------- */

#ifndef _TEXTUREMANAGER_H_
#define _TEXTUREMANAGER_H_

#include <types.h>
#include <texture.h>
#include <lightmap.h>
#include <invistexture.h>

// maximum allowed textures 
#define MAX_TEXTURES 5000

class CTextureManager {
protected:
	// containers <vector>
	CTextureVector         textures;
	CTextureVector         sprite_textures;
	CLightMapTextureVector lm_textures;

	// counters
	int total_textures; 
	int total_lm;
	int total_sprite_textures;

public:
	// constructor
	CTextureManager();

	// sprite textures
	int add_sprite_texture(CInvisTexture *tex);
	void bind_sprite_texture(int spr_tex_id);

	// all textures
	int add_texture(CTexture *tex);
	int bind_texture(char *texture_name);
	int bind_texture(int texture_id);
	
	// add basic texture by filename
	virtual int add_texture(char *filename, char *tex_name);

	// lightmap adding/binding functions
	int  add_lightmap(char *filename, int height, int width);
	void bind_lightmap(int lm_id);
	
	// clean(empty) the containers
	void clean_all();	
	void clear_sprites();
};

#endif

// END OF FILE -----------------------------------------------------------------------------