/*	--------------------------------------------------------------------------------- 
	Filename      - waypoint.cpp 
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Implementation of CWayPoint and CWayPointChain clases
	--------------------------------------------------------------------------------- */

#include <types.h>
#include <waypoint.h>

// --------------------------------------------------------------------------------- 
// constructor
// --------------------------------------------------------------------------------- 
CWayPoint::CWayPoint(CVert3D pos, int type)
{
	this->pos  = new CVert3D(pos);
	this->type = type;
}

// --------------------------------------------------------------------------------- 
// return waypoint position
// --------------------------------------------------------------------------------- 
CVert3D CWayPoint::get_pos()
{
	return pos;
}

// --------------------------------------------------------------------------------- 
// return waypoint type
// --------------------------------------------------------------------------------- 
int CWayPoint::get_type()
{
	return type;
}

// --------------------------------------------------------------------------------- 
// debug
// --------------------------------------------------------------------------------- 
void CWayPoint::dump(CRend *r)
{
	r->console_printf("WayPoint: %.2f, %.2f, %.2f %s\n", pos->x, pos->y, pos->z, 
		(type == WPT_WAIT_AND_TURN) ? "Wait and Turn" : "Turn and Go");
}

// --------------------------------------------------------------------------------- 
// CWayPointChain constructor
// --------------------------------------------------------------------------------- 
CWayPointChain::CWayPointChain()
{
	started = 0;
}

// --------------------------------------------------------------------------------- 
// add waypoint to waypoint chains vector
// --------------------------------------------------------------------------------- 
void CWayPointChain::add_way_point(CWayPoint wp)
{
	chain.push_back(wp);
}

// --------------------------------------------------------------------------------- 
// return next waypoint in chain
// --------------------------------------------------------------------------------- 
CWayPoint *CWayPointChain::next_way_point()
{
	CWayPoint *wpret;

	wpret = chain_itor++;

	if(chain_itor == chain.end())
		chain_itor = chain.begin();

	return wpret;
}

// --------------------------------------------------------------------------------- 
// return the current waypoint
// --------------------------------------------------------------------------------- 
CWayPoint *CWayPointChain::current_way_point()
{
	if(!started)
		return NULL;

	return chain_itor;
}

// --------------------------------------------------------------------------------- 
// initialise a waypoint chain , create iterator
// --------------------------------------------------------------------------------- 
void CWayPointChain::start()
{
	if(!started) {
		chain_itor = chain.begin();
		started = 1;
	}
	
}

// --------------------------------------------------------------------------------- 
// number of waypoints in waypoint chain
// --------------------------------------------------------------------------------- 
int CWayPointChain::count()
{
	return chain.size();
}

// END OF FILE ---------------------------------------------------------------------