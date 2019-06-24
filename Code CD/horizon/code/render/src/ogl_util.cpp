/*	--------------------------------------------------------------------------------- 
	Filename      - obj_util.cpp
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Implementation of code to set up the window used for OpenGL 
				    output.
	--------------------------------------------------------------------------------- */

#include <windows.h>
#include <ogl.h>
#include <render_api.h>

// --------------------------------------------------------------------------------- 
// sets up the window used for output so that it can support opengl at the correct
// resolution of 32bits per pixel with a 16 bit depth buffer, 8 bit alpha buffer and
// 8 bit stencil buffer.
// --------------------------------------------------------------------------------- 
int COpenGLRend::setup_pixel_format(void) 
{ 
    PIXELFORMATDESCRIPTOR pfd, *ppfd; 
    int pixelformat; 
 
    ppfd = &pfd; 
 
    ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR); 
    ppfd->nVersion = 1; 
    ppfd->dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |  
                        PFD_DOUBLEBUFFER | PFD_TYPE_RGBA; 
    ppfd->dwLayerMask  = PFD_MAIN_PLANE; 
    ppfd->iPixelType   = PFD_TYPE_RGBA; 
    ppfd->cColorBits   = 32; 
    ppfd->cDepthBits   = 16; 
	ppfd->cAlphaBits   = 8;
    ppfd->cStencilBits = 8; 
 
    pixelformat = ChoosePixelFormat(g_hdc, ppfd); 
 
    if ( (pixelformat = ChoosePixelFormat(g_hdc, ppfd)) == 0 ) 
    { 
        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 
    if (SetPixelFormat(g_hdc, pixelformat, ppfd) == FALSE) 
    { 
        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK); 
        return FALSE; 
    } 
 
    return TRUE; 
} 


// END OF FILE -------------------------------------------------------------------