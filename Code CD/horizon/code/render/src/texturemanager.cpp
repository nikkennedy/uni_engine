/*	--------------------------------------------------------------------------------- 
	Filename      - texturemanager.cpp
	Last Modified - 10/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Class definition of CTextureManager 
					This is the super class of any texture management implementation
					COGLTextureManager is a sub class of this and implements an OpenGL
					representation of the texture management functionality.
	--------------------------------------------------------------------------------- */

#include <texturemanager.h>
#include <basictexture.h>
#include <console.h>

// --------------------------------------------------------------------------------- 
// constuctor
// --------------------------------------------------------------------------------- 
CTextureManager::CTextureManager()
{
	total_textures = 0; 
	total_lm = 0; 
	total_sprite_textures = 0;
}

// --------------------------------------------------------------------------------- 
// removes all textures from the container's, do not remove sprite textures
// --------------------------------------------------------------------------------- 
void CTextureManager::clean_all()
{
	// vector::clear()
	for(CTextureIterator itor = textures.begin(); itor != textures.end(); itor++) {
		delete *itor;
	}
	textures.clear();
	for(CLightMapIterator lmitor = lm_textures.begin(); lmitor != lm_textures.end(); lmitor++) {
		delete *lmitor;
	}
	lm_textures.clear();
	total_lm = 0;
	total_textures = 0;
}

// --------------------------------------------------------------------------------- 
// removes the sprite textures from the container
// --------------------------------------------------------------------------------- 
void CTextureManager::clear_sprites()
{
	sprite_textures.clear();
	total_sprite_textures = 0;
}

// --------------------------------------------------------------------------------- 
// add a new sprite texture 
// --------------------------------------------------------------------------------- 
int CTextureManager::add_sprite_texture(CInvisTexture *tex)
{
	if(total_sprite_textures+1 >= MAX_TEXTURES) {
		cprt("error, reached the sprite texture limit, remove some textures\n");
		return -1; // too many textures
	} else {
		if(bind_texture(tex->get_name()) != -1) {
			return total_sprite_textures; // already added
		} else {
			sprite_textures.push_back(tex);
			total_sprite_textures++;
			return total_sprite_textures-1; // new texture added
		}
	}
}

// --------------------------------------------------------------------------------- 
// add texture to container
// --------------------------------------------------------------------------------- 
int CTextureManager::add_texture(CTexture *tex)
{
	if(total_textures+1 >= MAX_TEXTURES) {
		cprt("Reached the texture limit, remove some textures\n");
		return -1; // too many textures
	} else {

		// check if texture already in system
		if(bind_texture(tex->get_name()) != -1) {
			return total_textures; // return if it is
		} else {
			// create the new texture, and add it to the container
			textures.push_back(tex);
			total_textures++;
			return tex->get_id();
		}
	}
}

// --------------------------------------------------------------------------------- 
// creates a new texture from the filename (filename) assigning the identification
// string (text_name) to allow access.
// --------------------------------------------------------------------------------- 
int CTextureManager::add_texture(char *filename, char *tex_name)
{
	if(total_textures+1 >= MAX_TEXTURES) {
		cprt("Reached the texture limit, remove some textures\n");
		return -1; // too many textures
	} else {

		// check if texture already in system
		if(bind_texture(tex_name) != -1) {
			return total_textures; // return if it is
		} else {
			// create the new texture, and add it to the container
			textures.push_back(new CBasicTexture(filename, tex_name));
			total_textures++;
			return total_textures;
		}
	}
}

// --------------------------------------------------------------------------------- 
// create and add a lightmap to the lightmap container
// --------------------------------------------------------------------------------- 
int CTextureManager::add_lightmap(char *filename, int height, int width)
{
	if(total_lm+1 >= MAX_TEXTURES) {
		cprt("Reached the lm limit, remove some lm's\n");
		return -1; // too many lightmaps
	} else {
		lm_textures.push_back(new CLightMapTexture(filename, width, height));
		total_lm++;
		return total_lm;
	}
}

// --------------------------------------------------------------------------------- 
// the texture at container index texture_id is bound, i.e. is made ready to use
// --------------------------------------------------------------------------------- 
int CTextureManager::bind_texture(int texture_id)
{	
	CTexture *tex = textures[texture_id]; 
	tex->bind_texture();
	return texture_id;
}

// --------------------------------------------------------------------------------- 
// bind a texture based on its texture_name
// --------------------------------------------------------------------------------- 
int CTextureManager::bind_texture(char *texture_name)
{	
	for(CTextureIterator itor = textures.begin();itor!=textures.end();itor++) {
		CTexture *tex = (*itor); // search the texture container
		if(!strcmp(texture_name, tex->get_name())) { // if the texture names match
			tex->bind_texture(); // bind the texture
			return 1; 
		}
	}	
//	cprt("Can not find the texture %s\n", texture_name); // report error 
	return -1;
}

// --------------------------------------------------------------------------------- 
// bind a lightmap texture, based on its lightmap ID
// --------------------------------------------------------------------------------- 
void CTextureManager::bind_lightmap(int lm_id)
{
	if(lm_id>=total_lm) 
		return; // simple bounds check
	CLightMapTexture *lmtex = lm_textures[lm_id];
	lmtex->bind_texture(); // bind the texture
}

// --------------------------------------------------------------------------------- 
// bind a sprite texture, based on its lightmap ID
// --------------------------------------------------------------------------------- 
void CTextureManager::bind_sprite_texture(int spr_tex_id)
{
	sprite_textures[spr_tex_id]->bind_texture();
}

// END OF FILE ---------------------------------------------------------------------