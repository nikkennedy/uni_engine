/*	--------------------------------------------------------------------------------- 
	Filename      - render.cpp
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Implementation of CRenderer
					This class provides the way to load a CRend implementation from
					a DLL.
	--------------------------------------------------------------------------------- */

#include <windows.h>

#include <types.h>
#include <global.h>
#include <render.h>

// --------------------------------------------------------------------------------- 
// open the DLL. execute the exported function to create instance of CRend
// --------------------------------------------------------------------------------- 
int CRenderer::start_renderer(char *render_dll)
{
	
	renderer_instance = LoadLibrary(render_dll); // load the specified renderer dll
	
	if(renderer_instance == NULL) {
        MessageBox(NULL, "Could not load the renderer DLL.", "Error", MB_OK); 
        return 0; 
	}

	get_renderer = (gr_proc_class) GetProcAddress(renderer_instance, "get_renderer");

	if(get_renderer == NULL) {
        MessageBox(NULL, "Could not initialise the renderer.", "Error", MB_OK); 
        return 0; 
	}

	r_api = get_renderer();

	if(r_api == NULL) {
        MessageBox(NULL, "Renderer API could not be initialised.", "Error", MB_OK); 
        return 0; 
	}

	dbp("API version %d loaded.\n", r_api->get_version());
	return 1;
}

// END OF FILE -----------------------------------------------------------------------

