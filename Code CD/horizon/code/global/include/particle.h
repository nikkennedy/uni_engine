/*	--------------------------------------------------------------------------------- 
	Filename      - particle.h
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Definition of CParticle
					Defines the attributes of a single particle
	--------------------------------------------------------------------------------- */

#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include <types.h>

class CParticle 
{
public:
		CVert3D position;
		CVert3D velocity;
		CVert3D acceleration;
		int     age;
		int     max_age; // maximum age particle can get to before being destroyed
		float   size;
		int     alive;
};

#endif

// END OF FILE -------------------------------------------------------------------