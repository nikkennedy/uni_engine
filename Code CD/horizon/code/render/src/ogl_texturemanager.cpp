/*	--------------------------------------------------------------------------------- 
	Filename      - ogl_texturemanager.cpp 
	Last Modified - 09/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Implementation of COGLTextureManager (sub-class of CTextureManager)
					a container for textures and lightmaps.
	--------------------------------------------------------------------------------- */

#include <windows.h>
#include <gl/gl.h>
#include <types.h>
#include <debug.h>
#include <lightmap.h>
#include <basictexture.h>
#include <alphatexture.h>
#include <invistexture.h>
#include <ogl_texturemanager.h>

// --------------------------------------------------------------------------------- 
// the vertex passed to this function is drawn using the currently active texture
// different texture types could have different implementations here to deal with
// per-pixel shading/shadows and other effects, but this is the most basic use, just
// give the OpenGL API the vertex information.
// --------------------------------------------------------------------------------- 
void COGLTextureManager::process_vertex(CVert3D *v)
{	
	glVertex3fv((float*)v); // standard OpenGL vertex drawing
}


// END OF FILE ---------------------------------------------------------------------