/*	--------------------------------------------------------------------------------- 
	Filename      - debug.h
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - CDebug class
					provides a means to store any text to a file in a set way allowing
					easy debugging.
	--------------------------------------------------------------------------------- */

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <windows.h>
#include <console.h>
#include <types.h>

// --------------------------------------------------------------------------------- 
// defines
// --------------------------------------------------------------------------------- 
#define DEBUG_MAX_BUFFER_LENGTH 5000
#define DEBUG_FILENAME "debug.dat"

// --------------------------------------------------------------------------------- 
// debug files are not enabled, uncomment line below and recompile if debug comments
// are wanted
// --------------------------------------------------------------------------------- 
//#define DEBUG

// --------------------------------------------------------------------------------- 
// simplify code
// --------------------------------------------------------------------------------- 
#ifdef DEBUG
	#define dbp c_debug->debug_print
#else
	#define dbp //
#endif

class CDebug 
{
private:
	unsigned short init;
	int fd;

public:
	CDebug();
	int debug_init(char *filename);
	int debug_print(char *str, ...);
	int debug_close(void);
};

#endif

// --------------------------------------------------------------------------------- 
// globally available access to CDebug class
// --------------------------------------------------------------------------------- 
extern CDebug *c_debug;

// END OF FILE -------------------------------------------------------------------------