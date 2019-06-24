/*	--------------------------------------------------------------------------------- 
	Filename      - bullet.cpp 
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Implementation of CBullet, represents a rocket 
	--------------------------------------------------------------------------------- */

#include <types.h>
#include <bullet.h>
#include <maths.h>
#include <global.h>
#include <rend.h>

// --------------------------------------------------------------------------------- 
// constructor
// --------------------------------------------------------------------------------- 
CBullet::CBullet() : CObject()
{
	// object settings
	active      = 0;
	width       = 0.1f;
	height      = 0.11f;
	step_amount = 0;
	step_unit   = 1;
	mass        = 0;
	speed       = 2;
	armed       = 0;

	// load particle systems required
	char strbuffer[200];

	strcpy(strbuffer, CHrzConfig::data_path);
	strcat(strbuffer, "\\particles\\rocketpuff.pt");

	bulletps = new CParticleSystem(10, CVert3D(location.x,location.y,location.z), strbuffer);

	strcpy(strbuffer, CHrzConfig::data_path);
	strcat(strbuffer, "\\particles\\rocketexplode.pt");

	explode_ps = new CParticleSystem(100, CVert3D(location.x,location.y,location.z), strbuffer);

	strcpy(strbuffer, CHrzConfig::data_path);
	strcat(strbuffer, "\\rocket.hrz");

	// load the model of the rocket
	shape = new CHrzParser();
	shape->parse_file(strbuffer);
}

// --------------------------------------------------------------------------------- 
// destructor
// --------------------------------------------------------------------------------- 
CBullet::~CBullet()
{
	delete explode_ps; // delete particle systems
	delete bulletps;
}

// --------------------------------------------------------------------------------- 
// change rockets position
// --------------------------------------------------------------------------------- 
void CBullet::set_details(CVert3D start_loc, CVert3D orientation)
{
	matrix1x4 unit, move;
	matrix4x4 rotate;
		
	MATRIX1X4(unit, 0, 0, -1, 0);

	rotation_matrix((int)orientation.x, (int)orientation.y, (int)orientation.z, rotate);
	matrix_mul1x4_4x4(unit, rotate, move);

	dir_vector  = CVert3D(move[0], move[1], move[2]);		
	time_active = 0;
	this->location    = CVert3D(start_loc);
	this->orientation = CVert3D(orientation);
	active      = 1;
	
}

// --------------------------------------------------------------------------------- 
// add explode particle system to rendering unit
// --------------------------------------------------------------------------------- 
void CBullet::explode(CRend *rend)
{
	explode_ps->system_origin = CVert3D(location.x, location.y, location.z);		
	rend->add_particle_system(explode_ps);		
}

// --------------------------------------------------------------------------------- 
// move bullet along its path, creating smoke particle systems as it goes
// --------------------------------------------------------------------------------- 
void CBullet::move(CRend *rend)
{
	apply_force(speed, dir_vector.x, dir_vector.y, dir_vector.z);
	rend->move_object(this); 

	time_active++;

	if((time_active % 2)==0) {
		bulletps->system_origin = CVert3D(location.x, location.y, location.z);			
		rend->add_particle_system(bulletps);		
	}
}

// --------------------------------------------------------------------------------- 
// has the bullet hit a wall?
// --------------------------------------------------------------------------------- 
int CBullet::hit_wall()
{
	if(start_position == location) { 
		return 1;
	} else {
		return 0;
	}
}

// --------------------------------------------------------------------------------- 
// has the bullet hit the object *obj
// --------------------------------------------------------------------------------- 
int CBullet::hit_object(CObject *obj) 
{
	CVert3D bulletpos = location;

	CVert3D dist = bulletpos - obj->location;

	if(dist.length() < obj->width) {
		return 1;
	}
	return 0;
}

// END OF FILE ----------------------------------------------------------------------