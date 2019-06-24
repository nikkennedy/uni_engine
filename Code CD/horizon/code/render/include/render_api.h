/*	--------------------------------------------------------------------------------- 
	Filename      - render_api.h
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - a few defines and external variables that allow renderer DLL 
	                functions to be exported
	--------------------------------------------------------------------------------- */

#ifndef _RENDER_API_H_
#define _RENDER_API_H_

// place this define infront of a function and it will be available to be
// called by external programs
#define EXPORT extern "C" __declspec(dllexport) 
#define EXPORTCPP __declspec(dllexport) 

#include <types.h>

extern HDC g_hdc;
extern HGLRC g_hrc;
extern HWND g_hwnd;

#endif

// END OF FILE ----------------------------------------------------------------------