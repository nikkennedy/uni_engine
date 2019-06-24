/*	--------------------------------------------------------------------------------- 
	Filename      - aibase.h 
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - definition of CAIBase the abstraction of the A.I. unit 
	--------------------------------------------------------------------------------- */

#ifndef _AI_BASE_H_
#define _AI_BASE_H_

#include <windows.h>

#define EXPORT extern "C" __declspec(dllexport) 

#define AI_OK     1
#define AI_BAD    0

#include <types.h>
#include <rend.h>
#include <bot.h>

#define CALLBACK_PLAYER_HIT 1

#define MAX_CONTROLABLE 20

// --------------------------------------------------------------------------------- 
// define a vector to hold list of bots
// --------------------------------------------------------------------------------- 
typedef vector<CBot*> CBotVector;
typedef CBotVector::iterator CBotIterator;

// --------------------------------------------------------------------------------- 
// CAIBase class
// --------------------------------------------------------------------------------- 
class CAIBase {
protected:	

	CBotVector bots; // the list of bots being controlled	
	CObject *viewer;
	int    version;
	CRend  *rend;

public:
	// --------------------------------------------------------------------------------- 
	// constructors
	// --------------------------------------------------------------------------------- 
	CAIBase() {};
	CAIBase(int ver) : version(ver) {} ;

	int get_version() { return version; }

	// --------------------------------------------------------------------------------- 
	// virtual methods to be overridden by implementation
	// --------------------------------------------------------------------------------- 
	virtual int start(void)  =0;
	virtual int update()     =0;
	virtual void set_rend(CRend *r);
	virtual void set_viewer(CObject *viewer);
	virtual int count();
	virtual void flush_system() = 0;

	// --------------------------------------------------------------------------------- 
	// this does not have to be overridden
	// --------------------------------------------------------------------------------- 
	virtual CObject *add_bot(char *md2_path, char *skin_name, CVert3D pos, CVert3D ori, CWayPointChain *wpc);

	// --------------------------------------------------------------------------------- 
	// callback 
	// --------------------------------------------------------------------------------- 
protected:
	callback_type callback;
public:
	void set_call_back(callback_type cb) { callback = (callback_type)cb; }
};

#endif

// END OF FILE -------------------------------------------------------------------------