/*	--------------------------------------------------------------------------------- 
	Filename      - ogl_texturemanager.h 
	Last Modified - 09/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Class definition of COGLTextureManager (sub-class of CTextureManager)
	                a container for textures and lightmaps.
	--------------------------------------------------------------------------------- */

#ifndef _OGL_TEXTURE_MANAGER_H_
#define _OGL_TEXTURE_MANAGER_H_

#include <types.h>
#include <texturemanager.h>

class COGLTextureManager : public CTextureManager {
private:
public:
	// constructor
	COGLTextureManager() : CTextureManager() {}
	
	// vertex processing
	void process_vertex(CVert3D *v);	
};

#endif

// END OF FILE ---------------------------------------------------------------------