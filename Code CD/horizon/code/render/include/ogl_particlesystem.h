/*	--------------------------------------------------------------------------------- 
	Filename      - ogl_particlesystem.h
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Definition of COGLParticleSystem
				    Constains all the OpenGL-renderer specific code for rendering a 
					particle system
	--------------------------------------------------------------------------------- */

#ifndef _OGL_PARTSYSTEM_
#define _OGL_PARTSYSTEM_

#include <particlesystem.h>
#include <ogl_texturemanager.h>

class COGLParticleSystem : public CParticleSystem
{
public:
	// must provide this function in order to allow the particle system to be rendered
	void render();

	~COGLParticleSystem();
	COGLParticleSystem(CParticleSystem *ps, COGLTextureManager *tm);
};

#endif

// END OF FILE -----------------------------------------------------------------------