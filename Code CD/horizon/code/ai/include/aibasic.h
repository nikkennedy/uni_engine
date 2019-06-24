/*	--------------------------------------------------------------------------------- 
	Filename      - aibasic.h 
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Definition of CAIBasic the implementation of the A.I. unit 
		            that uses a finite state machine
	--------------------------------------------------------------------------------- */

#ifndef _AI_BASIC_H_
#define _AI_BASIC_H_

#include <aibase.h>
#include <debug.h>

#include <vector>
using namespace std;

// --------------------------------------------------------------------------------- 
// CAIBasic
// --------------------------------------------------------------------------------- 
class CAIBasic : public CAIBase {
public:
	CAIBasic(int ver) { version = ver; dbp("Basic AI Started\n");};
	int start(void);
	int update();
	void flush_system();
};

#endif

// END OF FILE ---------------------------------------------------------------------