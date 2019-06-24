/*	--------------------------------------------------------------------------------- 
	Filename      - particlesystem.h
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Definition of CParticleSystem
				    Constains all the non-renderer specific code for controlling a 
					particle system
	--------------------------------------------------------------------------------- */

#ifndef _PARTICLE_SYSTEM_
#define _PARTICLE_SYSTEM_

#include <types.h>
#include <particle.h>
#include <texturemanager.h>

class CParticleSystem 
{
public:
	CTextureManager *tm;
	CParticle *particles;
	int       num_particles;
	CVert3D   system_origin;

	// --------------------------------------------------------------------------------- 
	// the texture used to represent each particle
	// --------------------------------------------------------------------------------- 
	char texturename[255];
	int type; // invis or alpha
	CVert3D type_color; 
	char filename[255]; // filename of particle system definition file
	void set_option(CParse *p); // parse the definition file

	int       depth_test; // perform depth test
	int       height;     // particles maximum height
	float     max_width;
	int       bigtosmall; //}
	int       agebased;   //} Types of particle system
	float     velocityx, velocityy, velocityz;
	float     velocityax, velocityay, velocityaz;
	float     velocitysx, velocitysy, velocitysz;
	int       src_blend, dst_blend;
	int       max_age;
	CVert3D   acceleration;
	int       death_time; // destroy particle system when this is reached

	// --------------------------------------------------------------------------------- 
	// constructors/destructor
	// --------------------------------------------------------------------------------- 
	CParticleSystem();
	CParticleSystem(int num_particles, CVert3D origin, char *pt_filename);
	~CParticleSystem();
	
	// --------------------------------------------------------------------------------- 
	// functions
	// --------------------------------------------------------------------------------- 
	void reset_system();	
	void set_particle_defaults(int i);
	int update_system(float time_passed);  // returns -1 when particle system should be destroyed

	// --------------------------------------------------------------------------------- 
	// Redefined in all subclasses which contain renderer specific code
	// --------------------------------------------------------------------------------- 
	void render(){}; // must be defined in subclass
};

#endif

// END OF FILE -------------------------------------------------------------------------