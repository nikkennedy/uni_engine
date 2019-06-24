/*	--------------------------------------------------------------------------------- 
	Filename      - init.cpp
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - This contains the only function exported by the renderer DLL
	--------------------------------------------------------------------------------- */

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <render_api.h>
#include <ogl.h>
#include <debug.h>

// windows specific OpenGL variables
HDC   g_hdc;
HGLRC g_hrc;
HWND  g_hwnd;

// OpenGL renderer specific
COpenGLRend *c_ogl_rend = NULL;

// --------------------------------------------------------------------------------- 
// this is the starting point for access to all the rending functions, the game
// unit relies on this to provide it with a pointer to a rendering class.
// --------------------------------------------------------------------------------- 
EXPORT CRend *get_renderer(void)
{

#ifdef DEBUG
	c_debug = new CDebug();
	c_debug->debug_init("rend_debug.dat");
#endif

	// make sure only one render is created (singleton)
	if(c_ogl_rend != NULL)
		return c_ogl_rend;

	// Create a new OpenGL implementation of the rendering functions
	c_ogl_rend = new COpenGLRend(); 

	return c_ogl_rend;
}



