/*	--------------------------------------------------------------------------------- 
	Filename      - win.cpp 
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Microsoft Windows specific code encapsulated in CWin
	--------------------------------------------------------------------------------- */

#include <windows.h>
#include <types.h>
#include <global.h>
#include <win.h>
#include <render.h>
#include <game.h>

int window_active;

// --------------------------------------------------------------------------------- 
// constructor
// --------------------------------------------------------------------------------- 
CWin::CWin(HINSTANCE h_instance, char *app_name) 
{
	init = 0;

	this->h_instance = h_instance;
	strcpy(this->app_name, app_name);

	width  = 800;
	height = 600;
}

// --------------------------------------------------------------------------------- 
// windows message handler
// --------------------------------------------------------------------------------- 
LRESULT CALLBACK win_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LONG    lRet = 1;		
	
	switch (uMsg) { 
 
		case WM_CREATE: // window created
			dbp("WM_CREATE:\n");
			// initialise the rendere
			c_game->c_rend->r_api->create_renderer(hWnd);
			RECT rect;
			GetClientRect(hWnd, &rect);
			ClipCursor(&rect);
			break; 

		case WM_CLOSE: // window close
			DestroyWindow (hWnd); 
			break; 
 
		case WM_SIZE: // window resized
			c_game->c_rend->r_api->update_screen_size(LOWORD(lParam), HIWORD(lParam));				
			break;

		case WM_DESTROY: // window destroyed
 	        PostQuitMessage (0); 
			ClipCursor(NULL);
	       break; 
     
		case WM_ACTIVATE: // mouse clicked on window
			// The windows has been activated
			if(WA_INACTIVE==wParam){
				window_active = 0;			
			} else {
				window_active = 1;
			}
			break;

		default: // anything else
			lRet = DefWindowProc (hWnd, uMsg, wParam, lParam); 
			break; 
    } 
 
    return lRet; 
}

// --------------------------------------------------------------------------------- 
// create the window and register it with MS Windows
// --------------------------------------------------------------------------------- 
int CWin::init_win(void)
{
	if(init) {
		return 1;
	}

	wndclass.style         = 0; 
    wndclass.lpfnWndProc   = (WNDPROC) win_proc; 
    wndclass.cbClsExtra    = 0; 
    wndclass.cbWndExtra    = 0; 
    wndclass.hInstance     = h_instance; 
    wndclass.hIcon         = LoadIcon (h_instance, app_name); 
    wndclass.hCursor       = NULL;//LoadCursor (NULL,IDC_ARROW); 
    wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1); 
    wndclass.lpszMenuName  = app_name; 
    wndclass.lpszClassName = app_name; 
	
 
    if (!RegisterClass (&wndclass) ) {
        MessageBox(NULL, "Window could not be registered.", "Error", MB_OK); 
        return -1; 
	}

	dbp("Window class has been registered.\n");

	if(CHrzConfig::full_screen) {
   /* Create the frame */ 
    gh_wnd = CreateWindow(app_name, app_name, 
						  WS_POPUP,
						  CW_USEDEFAULT, 
						  CW_USEDEFAULT, 
						  width, 
                          height, 
						  NULL, 
						  NULL, 
						  h_instance, 
						  NULL); 
	} else {
    gh_wnd = CreateWindow(app_name, app_name, 
						  WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 
						  CW_USEDEFAULT, 
						  CW_USEDEFAULT, 
						  width, 
                          height, 
						  NULL, 
						  NULL, 
						  h_instance, 
						  NULL); 
	}

	// switch to full screen mode
	if(CHrzConfig::full_screen) {
		DEVMODE mode;
		mode.dmSize=sizeof(DEVMODE);
		mode.dmPelsWidth=width;
		mode.dmPelsHeight=height;
		mode.dmFields=DM_PELSWIDTH|DM_PELSHEIGHT;
		ChangeDisplaySettings(&mode,CDS_FULLSCREEN);
		MoveWindow(gh_wnd,0,0,width,height,true);
	}

	ShowCursor(false);

    // make sure window was created 
    if (!gh_wnd) {
        MessageBox(NULL, "Window could not be created.", "Error", MB_OK); 
        return -1; 
	}

    // show and update main window 
    ShowWindow(gh_wnd, SW_SHOW); 
    UpdateWindow(gh_wnd); 

	SetCursor(NULL);

	dbp("Window is shown and updated, entering message loop.\n");
	dbp("Renderer has been created.\n");

	return 1;
}

// --------------------------------------------------------------------------------- 
// main loop of the whole system
// --------------------------------------------------------------------------------- 
int CWin::enter_loop()
{
	c_game->start(); // starting the game engine
	c_game->c_rend->r_api->console_printf("Entering the animation loop.\n");

	// animation loop
    while (1) { 
		// process pending messages 
        while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) == TRUE) 
        { 
            if (GetMessage(&msg, NULL, 0, 0) ) 
            { 
                TranslateMessage(&msg); 
                DispatchMessage(&msg); 
            } else { 
                return TRUE; 
            } 
        } 

		// --------------------------------------------------------------------------------- 
		// ********************************** update the game ******************************
		// --------------------------------------------------------------------------------- 
		if(window_active)
			c_game->update();
	}	
	return 1;
}

// END OF FILE -------------------------------------------------------------------------