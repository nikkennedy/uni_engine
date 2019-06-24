/*	--------------------------------------------------------------------------------- 
	Filename      - dxstuff.h 
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Definition of CDirectX, provides the DirectInput functionality
	--------------------------------------------------------------------------------- */

#ifndef _DXSTUFF_H_
#define _DXSTUFF_H_

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

#define KEYBOARD_BUFFER_SIZE 10

class CDirectX
{
public:
	int input_active;

	// --------------------------------------------------------------------------------- 
	// main direct input data structure
	// --------------------------------------------------------------------------------- 
	LPDIRECTINPUT8 lpdi;
	// --------------------------------------------------------------------------------- 
	// directX data structures for mouse input
	// --------------------------------------------------------------------------------- 
	LPDIRECTINPUTDEVICE8 m_mouse;
	DIMOUSESTATE mouse_state;
	// --------------------------------------------------------------------------------- 
	// needs access to window handle
	// --------------------------------------------------------------------------------- 
	HWND hWnd;
	// --------------------------------------------------------------------------------- 
	// data structures for keyboard input
	// --------------------------------------------------------------------------------- 
	LPDIRECTINPUTDEVICE8 m_keyboard;
	HKL keyboard_layout; 
	unsigned char keystate[256];
	unsigned char keysascii[256];

	int initialize(void);
	void set_hwnd(HWND hwnd) { hWnd = hwnd; }
	int updateinput();
	int key_down(int n) { return (keystate[n]); }
	// --------------------------------------------------------------------------------- 
	// scan code to ascii code
	// --------------------------------------------------------------------------------- 
	char scan2ascii(DWORD scancode);
};

#endif

// END OF FILE -------------------------------------------------------------------