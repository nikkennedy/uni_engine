/*	--------------------------------------------------------------------------------- 
	Filename      - ai.h 
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Definition of CGameAI
					This class provides the way to load a CAIBase implementation from
					a DLL.
	--------------------------------------------------------------------------------- */

#ifndef _AI_H_
#define _AI_H_

#include <aibase.h>

// --------------------------------------------------------------------------------- 
// CGameAI
// --------------------------------------------------------------------------------- 
class CGameAI {
protected:
	
	HINSTANCE ai_dll_instance;
	// --------------------------------------------------------------------------------- 
	// pointer to a function, this is initialised to the DLLS exported function
	// that will return an instance of CAIBase
	// --------------------------------------------------------------------------------- 	
	CAIBase *(*get_ai_manager)();
public:
	CAIBase *ai_manager;

public:
	CAIBase *base_ai;

	CGameAI() {};

	int start_ai(char *filename);
};

#endif

// END OF FILE -----------------------------------------------------------------------