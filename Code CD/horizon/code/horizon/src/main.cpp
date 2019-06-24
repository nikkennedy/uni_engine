/*	--------------------------------------------------------------------------------- 
	Filename      - main.cpp
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - The starting point for the game executable
	--------------------------------------------------------------------------------- */

#include <windows.h>

#include <types.h>
#include <debug.h>
#include <global.h>
#include <win.h>
#include <render.h>
#include <ai.h>
#include <dxstuff.h>

    
// --------------------------------------------------------------------------------- 
// WINMAIN!
// --------------------------------------------------------------------------------- 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	// --------------------------------------------------------------------------------- 
	// raise the priority of this process
	// --------------------------------------------------------------------------------- 
	SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
	
	// --------------------------------------------------------------------------------- 
	// create DirectInput system
	// --------------------------------------------------------------------------------- 
	CDirectX *dxinput = new CDirectX();

	// --------------------------------------------------------------------------------- 
	// debugging wanted?
	// --------------------------------------------------------------------------------- 
#ifdef DEBUG
	c_debug = new CDebug(); // create the debug output class
	c_debug->debug_init("debug.dat");
#endif

	// --------------------------------------------------------------------------------- 
	// create windows specific code handler
	// --------------------------------------------------------------------------------- 
	CWin *c_win   = new CWin(hInstance, "Horizon V1.0 by Nicholas Kennedy "); // create the window class

	// --------------------------------------------------------------------------------- 
	// create game application class
	// --------------------------------------------------------------------------------- 
	c_game = new CGame();
	c_game->initialize();
	
	dbp("Program started\n");
	dbp("Initializing the renderer\n");
	// --------------------------------------------------------------------------------- 
	// collect an instance of CRend from the rendering DLL
	// --------------------------------------------------------------------------------- 
	c_game->c_rend = new CRenderer();

	if(!c_game->c_rend->start_renderer("render.dll"))
		return 0;

	// --------------------------------------------------------------------------------- 
	// do the same with the A.I. unit
	// --------------------------------------------------------------------------------- 
	dbp("Initializing the AI manager\n");
	c_game->c_rend->r_api->console_printf("Initializing the AI manager.\n");
	c_game->c_ai_manager = new CGameAI();	
	
	if(!c_game->c_ai_manager->start_ai("gameai.dll")) {
		return 0;
	}

	// --------------------------------------------------------------------------------- 
	// give the A.I. unit access to the rendering unit
	// --------------------------------------------------------------------------------- 
	c_game->c_ai_manager->ai_manager->set_rend(c_game->c_rend->r_api);

	c_game->c_rend->r_api->console_printf("Creating the Window\n");
	dbp("Creating the window.\n");

	// --------------------------------------------------------------------------------- 
	// initialise the output window
	// --------------------------------------------------------------------------------- 
	c_win->init_win();

	// --------------------------------------------------------------------------------- 
	// initialise the input
	// --------------------------------------------------------------------------------- 
	c_game->c_rend->r_api->console_printf("Initializing the Input\n");
	dbp("Initializing the Input\n");
	dxinput->set_hwnd(c_win->get_hwnd());

	if(dxinput->initialize() == false) {
		c_game->REND_UNIT->console_printf("DirectInput failed to initialise.\n");
		return 1;
	}	
	
	c_game->c_rend->r_api->set_dxstuff(dxinput);
	c_game->set_dxstuff(dxinput);

	c_game->c_rend->r_api->console_printf("DXInput Setup completed.\n");

	// --------------------------------------------------------------------------------- 
	// enter the main loop
	// --------------------------------------------------------------------------------- 
	c_win->enter_loop();

	// --------------------------------------------------------------------------------- 
	// exit
	// --------------------------------------------------------------------------------- 
	return 1;
}


// END OF FILE ------------------------------------------------------------------------