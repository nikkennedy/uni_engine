/*	--------------------------------------------------------------------------------- 
	Filename      - ogl_vp.cpp
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - This code initialised and starts up the OpenGL extension required.
	                Also provides helper function to return the OpenGL driver name
	--------------------------------------------------------------------------------- */

#include <windows.h>
#include <ogl.h>
#define GLH_EXT_SINGLE_FILE
#include <glh_extensions.h>
#include <glh_obs.h>
using namespace glh;
#include <console.h>

// --------------------------------------------------------------------------------- 
// return OpenGL API developers name
// --------------------------------------------------------------------------------- 
char *COpenGLRend::get_vendor_name(void)
{
	return (char *) glGetString(GL_VENDOR);
}

// --------------------------------------------------------------------------------- 
// Initialise the multitexturing OpenGL extension
// --------------------------------------------------------------------------------- 
int COpenGLRend::init_nv(void)
{
	cprt("Initializing OpenGL Extenstions\n");

	if(!glh_init_extensions("GL_ARB_multitexture ")) {
		cprt("Can not initialize GL_ARB_multitexture,  we're gonna have a problem.\n");
		return 0;
	} else {
		cprt("Initialized GL_ARB_multitexture, all is well.\n");
	}
	return 1;
}

// END OF FILE ---------------------------------------------------------------