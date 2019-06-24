/*	--------------------------------------------------------------------------------- 
	Filename      - directx.cpp 
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Implementation of CDirectX, provides the DirectInput functionality
	--------------------------------------------------------------------------------- */

#include <windows.h>
#include <dxstuff.h>
#include <dinput.h>
#include <win.h>

// --------------------------------------------------------------------------------- 
// initialise the mouse and keyboard
// --------------------------------------------------------------------------------- 
int CDirectX::initialize(void)
{
	if(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&lpdi, NULL) != DI_OK) {
        MessageBox(NULL, "Could not initialise DirectInput.", "Error", MB_OK); 
		return 0;  
	}
	
	if(FAILED(lpdi->CreateDevice(GUID_SysMouse, &m_mouse, NULL))) {
        MessageBox(NULL, "Coult not initialise the mouse.", "Error", MB_OK); 
		return false;
	}

	if(FAILED(m_mouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE))) {
        MessageBox(NULL, "Coult not initialise the mouse.", "Error", MB_OK); 
		return false;
	}
	if(FAILED(m_mouse->SetDataFormat(&c_dfDIMouse))) {
        MessageBox(NULL, "Coult not initialise the mouse.", "Error", MB_OK); 
		return false;
	}

	if(FAILED(m_mouse->Acquire())) {
        MessageBox(NULL, "Coult not initialise the mouse.", "Error", MB_OK); 
		return false;
	}


	if (FAILED(lpdi->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL))) {
		MessageBox(NULL, "Coult not initialise the keyboard.", "Error", MB_OK); 
		return false;
	}

	DIPROPDWORD dipdw;

	ZeroMemory(&dipdw, sizeof(DIPROPDWORD));

	// set up a keyboard buffer holding 20 elements
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = 20;

	m_keyboard->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

	if (FAILED(m_keyboard->SetDataFormat(&c_dfDIKeyboard))) { 
		MessageBox(NULL, "Coult not initialise the keyboard.", "Error", MB_OK); 
		return false;
	}
  
	if (FAILED(m_keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))) {
		MessageBox(NULL, "Coult not initialise the keyboard.", "Error", MB_OK); 
		return false;
	}
	
	if (FAILED(m_keyboard->Acquire())) {
		MessageBox(NULL, "Coult not initialise the keyboard.", "Error", MB_OK); 
		return false;
	}

	// UK layout
	keyboard_layout = LoadKeyboardLayout("00000809", KLF_SUBSTITUTE_OK);
	input_active = 1;
	
	return 1;
}

// --------------------------------------------------------------------------------- 
// scan code to ascii code
// --------------------------------------------------------------------------------- 
char CDirectX::scan2ascii(DWORD scancode)
{
	unsigned int vk;
	vk = MapVirtualKeyEx(scancode, 1, keyboard_layout);
	return vk;
} 

// --------------------------------------------------------------------------------- 
// get the latest input
// --------------------------------------------------------------------------------- 
int CDirectX::updateinput(void)
{
	if(window_active == 0 && input_active == 1) {
		m_mouse->Unacquire();
		m_keyboard->Unacquire();
		input_active = 0;
	}
	if(window_active == 1 && input_active == 0) {
		m_mouse->Acquire();
		m_keyboard->Acquire();
		input_active = 1;
	}

	if(m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouse_state) != DI_OK) {
		input_active = 0;
		return 0;
	}
	if(m_keyboard->GetDeviceState(sizeof(unsigned char[256]), (LPVOID)keystate) != DI_OK) {  
		input_active = 0;
		return 0;
	}

	HRESULT            hr;
	DWORD              dw_cur_buffer;
	DIDEVICEOBJECTDATA did_keyboard_buffer[KEYBOARD_BUFFER_SIZE];
	DWORD              dw_items = KEYBOARD_BUFFER_SIZE;
	BYTE               byte_ascii;

	hr = m_keyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), did_keyboard_buffer, &dw_items, 0);

	if(dw_items) {

		for(dw_cur_buffer=0;dw_cur_buffer<dw_items;dw_cur_buffer++) {
			byte_ascii = scan2ascii(did_keyboard_buffer[dw_cur_buffer].dwOfs);

			if(did_keyboard_buffer[dw_cur_buffer].dwData & 0x80) {
				keystate[did_keyboard_buffer[dw_cur_buffer].dwOfs] = 1;
				keysascii[byte_ascii] = 1;
			} else {
				keystate[did_keyboard_buffer[dw_cur_buffer].dwOfs] = 0;		
				keysascii[byte_ascii] = 0;
			}
		}
	}

	return 1;
}

// END OF FILE --------------------------------------------------------------------