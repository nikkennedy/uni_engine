/*	--------------------------------------------------------------------------------- 
	Filename      - object.cpp
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Implementation of CObject the basic class which holds information
	                about all objects (player, bots, collectables etc)
	--------------------------------------------------------------------------------- */

#include <types.h>
#include <object.h>
#include <rend.h> 
#include <hrz_config.h>
#include <float.h>

#define GRAVITY_CONSTANT 1.0f

// --------------------------------------------------------------------------------- 
// Constructor for objects, sets initial values for every object, most will be
// changed depending on purpose that class is used for.
// --------------------------------------------------------------------------------- 
CObject::CObject() 
{
	poly_hit_list    = new CPolygonList(1);
	last_start_node  = NULL;
	last_end_node    = NULL;
	hit_something    = NO_HIT;
	start_position   = CVert3D(0,0,0);
	end_position     = CVert3D(0,0,0);
	gravity          = 1.1f;
	falltime         = 0;
	active           = 0;
	shape            = NULL;
	md2_model        = NULL;
}

// --------------------------------------------------------------------------------- 
// destructor, free up memory
// --------------------------------------------------------------------------------- 
CObject::~CObject()
{
	if(shape!=NULL)
		delete shape;
	if(md2_model != NULL) {
		delete md2_model;
	}
	if(poly_hit_list != NULL)
		delete poly_hit_list;
}

// --------------------------------------------------------------------------------- 
// calculate vector based of start and end positions
// --------------------------------------------------------------------------------- 
CVert3D CObject::get_dir_vector()
{
	dir_vector = end_position - start_position;
	dir_vector.normalise();
	return dir_vector;	
}

// --------------------------------------------------------------------------------- 
// given a location, set the start and end points of movement and current
// position to that
// --------------------------------------------------------------------------------- 
void CObject::set_location(float x, float y, float z) { 
	location       = CVert3D(x, y, z);
	start_position = location;
	end_position   = location;
}

// --------------------------------------------------------------------------------- 
// return current location
// --------------------------------------------------------------------------------- 
CVert3D CObject::get_location() {
	return location;
}

// --------------------------------------------------------------------------------- 
// set the objects orientation
// --------------------------------------------------------------------------------- 
void CObject::set_orientation(float x, float y, float z) { 
	orientation = CVert3D(x, y, z);
}

// --------------------------------------------------------------------------------- 
// return the current orientation
// --------------------------------------------------------------------------------- 
CVert3D CObject::get_orientation() {
	return orientation;
}

// --------------------------------------------------------------------------------- 
// given a force, set the start point to the current location and the end point to
// the current location * force (ready for the rendering unit to actually do the 
// moving whilst checking for collisions)
// --------------------------------------------------------------------------------- 
void CObject::apply_force(float force, float x, float y, float z) {

	start_position = location;
	end_position   = location;

	if(x!=0)
		end_position.x += x * force;
	if(y!=0)
		end_position.y += y * force;
	if(z!=0)
		end_position.z += z * force;

}

// --------------------------------------------------------------------------------- 
// change the objects orientation and performs bounds checking
// --------------------------------------------------------------------------------- 
void CObject::apply_rotation(float dx, float dy, float dz) {
		orientation.x += dx;
		orientation.y += dy;
		orientation.z += dz;
 
        if(orientation.x >= 360) orientation.x -= 360;
		if(orientation.x < 0) orientation.x += 360;
		if(orientation.y >= 360) orientation.y -= 360;
		if(orientation.y < 0) orientation.y += 360;
		if(orientation.z >= 360) orientation.z -= 360;
		if(orientation.z < 0) orientation.z += 360;
}		

// --------------------------------------------------------------------------------- 
// this is a utility function that requires the rendering unit to be passes as a 
// parameter, it attempts to move the object along the path specified by the vector
// in unit which is rotate by the objects orientation a distance of 'amount'
// --------------------------------------------------------------------------------- 
int CObject::move_object(matrix1x4 unit, float amount, CRend *api)
{	
	int hit = NO_HIT;

	matrix4x4 rotate;
	matrix1x4 move;

	// fix bug that occurs when orientation is 0
	if(orientation.y == 0) orientation.y = 1;

	// rotate the vector
	rotation_matrix((int)orientation.x, (int)orientation.y, (int)orientation.z, rotate);
	matrix_mul1x4_4x4(unit, rotate, move);

	// apply downward gravitational force
	if(CHrzConfig::gravity == 0) {
		apply_force(amount, 0, move[1], 0);
		if(api != NULL) 
			api->move_object(this);
	}

	// apply the force along the x axis
	apply_force(amount, move[0], 0, 0);

	if(api != NULL) {
		if(api->move_object(this) == HIT_POLY)
			hit = HIT_POLY;
	}

	// apply the force along the z axis
	apply_force(amount, 0, 0, move[2]);

	if(api != NULL) {
		if(api->move_object(this) == HIT_POLY)
			hit = HIT_POLY;
	}

	return hit;
}

// --------------------------------------------------------------------------------- 
// this applies downward force, and increase the force by constant factor
// --------------------------------------------------------------------------------- 
void CObject::apply_gravity(CRend *api)
{
	float start_y, end_y;

	start_y = location.y;

	apply_force(gravity, 0, -1, 0);		

	api->move_object(this);

	end_y = location.y;

	if(start_y != end_y) 
		falltime++;
	else {
		gravity  = mass;
		falltime = 0;
	}

	if(start_y != end_y && falltime >= 20) {
		gravity *= GRAVITY_CONSTANT;
	} 

}

// END OF FILE -----------------------------------------------------------------------