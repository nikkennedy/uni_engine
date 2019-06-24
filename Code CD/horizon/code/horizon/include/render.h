/*	--------------------------------------------------------------------------------- 
	Filename      - render.h
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - class that loads dll and returns instance of CRend
	--------------------------------------------------------------------------------- */

#ifndef _RENDER_H_
#define _RENDER_H_

#include <rend.h>

typedef CRend* (*gr_proc_class)(void); 

// --------------------------------------------------------------------------------- 
// CRenderer
// --------------------------------------------------------------------------------- 
class CRenderer {

private:
	HINSTANCE renderer_instance;
public:
	CRend *r_api;

private:
	// --------------------------------------------------------------------------------- 
	// function pointer for DLL exported function
	// --------------------------------------------------------------------------------- 
	CRend *(*get_renderer)();

public:
	int start_renderer(char *render_dll);

};

#endif

// END OF FILE -------------------------------------------------------------------------