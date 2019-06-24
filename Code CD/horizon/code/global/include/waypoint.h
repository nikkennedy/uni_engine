/*	--------------------------------------------------------------------------------- 
	Filename      - waypoint.h
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Definition of CWayPoint and CWayPointChain clases
	--------------------------------------------------------------------------------- */

#ifndef _WAYPOINT_H_
#define _WAYPOINT_H_

#include <types.h>
#include <rend.h>

// --------------------------------------------------------------------------------- 
// not used currently - future expansion ?
// --------------------------------------------------------------------------------- 
#define WPT_WAIT_AND_TURN 0
#define WPT_TURN_AND_GO   1

// --------------------------------------------------------------------------------- 
// CWayPoint
// --------------------------------------------------------------------------------- 
class CWayPoint
{
public:
	
	CVert3D *pos; // the waypoint position
	int type;

	CWayPoint(CVert3D pos, int type);

	inline CWayPoint operator=(const CWayPoint &wp)
	{
		pos = new CVert3D(wp.pos);
		type = wp.type;

		return *this;
	}

	CVert3D get_pos();
	int get_type();

	void dump(CRend *r);
	
};

// --------------------------------------------------------------------------------- 
// set up a new vector of waypoint type
// --------------------------------------------------------------------------------- 
#include <vector>
#include <iostream>

using namespace std;

typedef vector<CWayPoint> CWayPointVector;
typedef CWayPointVector::iterator CWayPointIterator;

// --------------------------------------------------------------------------------- 
// CWayPointChain
// --------------------------------------------------------------------------------- 
class CWayPointChain
{
	int started;

public:
	// --------------------------------------------------------------------------------- 
	// vector of waypoints and iterator for the vector
	// --------------------------------------------------------------------------------- 	
	CWayPointVector chain;
	CWayPointIterator chain_itor;

	// --------------------------------------------------------------------------------- 
	// constructor
	// --------------------------------------------------------------------------------- 
	CWayPointChain();

	void add_way_point(CWayPoint wp);

	// --------------------------------------------------------------------------------- 
	// initialise iterator and return next waypoint, loop round at end of vector
	// --------------------------------------------------------------------------------- 
	void start();
	CWayPoint *next_way_point();
	CWayPoint *current_way_point();

	// --------------------------------------------------------------------------------- 
	// number of waypoints
	// --------------------------------------------------------------------------------- 
	int count();

	// --------------------------------------------------------------------------------- 
	// override two different versions of the = operator
	// --------------------------------------------------------------------------------- 
	inline CWayPointChain operator=(CWayPointChain wp)
	{
		wp.start();
		for(int i=0;i<wp.count();i++) {
			add_way_point(*wp.next_way_point());
		}

		return *this;
	}

	inline CWayPointChain operator=(CWayPointChain *wp)
	{
		wp->start();
		for(int i=0;i<wp->count();i++) {
			add_way_point(*wp->next_way_point());
		}

		return *this;
	}

};

// --------------------------------------------------------------------------------- 
// create a vector of waypoints type
// --------------------------------------------------------------------------------- 
typedef vector<CWayPointChain> CWayPointChainVector;
typedef CWayPointChainVector::iterator CWayPointChainIterator;

#endif

// END OF FILE ----------------------------------------------------------------