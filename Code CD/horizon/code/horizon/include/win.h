/*	--------------------------------------------------------------------------------- 
	Filename      - win.h 
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Microsoft Windows specific code encapsulated in CWin
	--------------------------------------------------------------------------------- */

#ifndef _WIN_H_
#define _WIN_H_

#include <windows.h>

#include <render.h>

// --------------------------------------------------------------------------------- 
// windows message handler
// --------------------------------------------------------------------------------- 
extern LRESULT CALLBACK win_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

extern int window_active; // important set when window is active

// --------------------------------------------------------------------------------- 
// CWin
// --------------------------------------------------------------------------------- 
class CWin {
private:
	// --------------------------------------------------------------------------------- 
	// windows attributes
	// --------------------------------------------------------------------------------- 
	HINSTANCE  h_instance;
    MSG        msg; 
    WNDCLASS   wndclass; 
	int        init;
	char       app_name[80];
	HWND       gh_wnd;
	int        width, height;
public:
	CWin(HINSTANCE h_instance, char *app_name);
	int init_win(void);
	HWND get_hwnd(void) { return gh_wnd; }
	int enter_loop();
};

#endif

// END OF FILE -------------------------------------------------------------------