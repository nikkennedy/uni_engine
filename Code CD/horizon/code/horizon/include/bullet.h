/*	--------------------------------------------------------------------------------- 
	Filename      - bullet.h 
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Definition of CBullet, represents a rocket 
	--------------------------------------------------------------------------------- */

#ifndef _BULLET_H_
#define _BULLET_H_

#include <object.h>
#include <particlesystem.h>

// --------------------------------------------------------------------------------- 
// CBullet
// --------------------------------------------------------------------------------- 
class CBullet : public CObject 
{
public:

	// --------------------------------------------------------------------------------- 
	// smoke and explode particle systems
	// --------------------------------------------------------------------------------- 
	CParticleSystem *bulletps;
	CParticleSystem *explode_ps;

	// --------------------------------------------------------------------------------- 
	// constructor/destructor
	// --------------------------------------------------------------------------------- 
	CBullet();
	~CBullet();

	// --------------------------------------------------------------------------------- 
	// functions
	// --------------------------------------------------------------------------------- 
	void set_details(CVert3D start_loc, CVert3D orientation);
	void move(CRend *rend);
	int  hit_wall();
	int  hit_object(CObject *obj);
	void explode(CRend *rend);

	int time_active;
	int armed;

	// --------------------------------------------------------------------------------- 
	// accessor methods
	// --------------------------------------------------------------------------------- 
	void arm() { armed = 1; }
	void disarm() { armed = 0; }
	int isarmed() { return armed; }

};

#endif

// END OF FILE ----------------------------------------------------------------------