/*	--------------------------------------------------------------------------------- 
	Filename      - init.cpp 
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - The exported DLL function that returns the DLLs instance of 
				    CAIBase - used as a way for implemented abstraction
	--------------------------------------------------------------------------------- */

#include <windows.h>

#include <aibase.h>
#include <aibasic.h>
#include <types.h>
#include <debug.h>

CAIBase *c_ai;

// --------------------------------------------------------------------------------- 
// exported function
// --------------------------------------------------------------------------------- 
EXPORT CAIBase *get_ai_manager(void)
{
#ifdef DEBUG
	c_debug = new CDebug();
	c_debug->debug_init("ai_debug.dat");
#endif

	// --------------------------------------------------------------------------------- 
	// this DLL returns a instance of CAIBasic
	// --------------------------------------------------------------------------------- 
	c_ai = new CAIBasic(1);

	return c_ai;
}

// END OF FILE ----------------------------------------------------------------

