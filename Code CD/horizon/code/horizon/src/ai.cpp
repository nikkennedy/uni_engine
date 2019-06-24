/*	--------------------------------------------------------------------------------- 
	Filename      - ai.cpp 
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Implementation of CGameAI
					This class provides the way to load a CAIBase implementation from
					a DLL.
	--------------------------------------------------------------------------------- */

#include <windows.h>
#include <aibase.h>
#include <ai.h>
#include <debug.h>

// --------------------------------------------------------------------------------- 
// pointer to a function, type
// --------------------------------------------------------------------------------- 
typedef CAIBase* (*p_aibase_class)(void); 

// --------------------------------------------------------------------------------- 
// performs all the work to load a DLL, setup the function pointer and retrieve
// the pointer to the instance of CBaseAI
// --------------------------------------------------------------------------------- 
int CGameAI::start_ai(char *filename)
{
	ai_dll_instance = LoadLibrary(filename);
	
	if(ai_dll_instance == NULL) {
        MessageBox(NULL, "Could not load the AI DLL.", "Error", MB_OK); 
        return 0; 
	}

	get_ai_manager = (p_aibase_class) GetProcAddress(ai_dll_instance, "get_ai_manager");

	if(get_ai_manager == NULL) {
        MessageBox(NULL, "Could not load the initialise AI DLL.", "Error", MB_OK); 
        return 0; 
	}

	ai_manager = get_ai_manager();

	if(ai_manager == NULL) {
        MessageBox(NULL, "Could not load create AI class.", "Error", MB_OK); 
        return 0; 
	}

	dbp("ai_manager API version %d loaded.\n", ai_manager->get_version());
	return 1;
}

// END OF FILE -----------------------------------------------------------------


