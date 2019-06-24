/*	--------------------------------------------------------------------------------- 
	Filename      - object.h
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Definition of CObject the basic class which holds information
	                about all objects (player, bots, collectables etc)
	--------------------------------------------------------------------------------- */

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include <types.h>
#include <maths.h>
#include <lbsp.h>
#include <md2.h>
#include <hrz_parse.h>

// --------------------------------------------------------------------------------- 
// useful defines
// --------------------------------------------------------------------------------- 
#define NO_HIT   0
#define HIT_POLY 1
#define NO_COLLISION -1

// --------------------------------------------------------------------------------- 
// need to know what CRend is
// --------------------------------------------------------------------------------- 
class CRend;

// --------------------------------------------------------------------------------- 
// need a vector
// --------------------------------------------------------------------------------- 
#include <vector>
using namespace std;

// --------------------------------------------------------------------------------- 
// CObject
// --------------------------------------------------------------------------------- 
class CObject {

public:
	
	// --------------------------------------------------------------------------------- 
	// the two possible model pointers
	// --------------------------------------------------------------------------------- 
	CHrzParser *shape;        // objects can have a physical representation (i.e. a bullet)
	CMD2       *md2_model;    // objects can have a MD2 as their representation

	float   height;           
	float   speed;            
	float   step_amount;      
	float   step_unit;        
	float   width;            
	int     falltime;         // amount to frames so far in which object fallen
	float   mass;             // mass of object 
	float   gravity;          // value of accelearation due to gravity for object

	int     active;           // is this object active? (used for bullets etc.)
	int     hitable;          // does this object hitable?

	// --------------------------------------------------------------------------------- 
	// curren location and orientation of object
	// --------------------------------------------------------------------------------- 
	CVert3D location;
	CVert3D orientation;

	// --------------------------------------------------------------------------------- 
	// collision detection
	// --------------------------------------------------------------------------------- 
	CVert3D start_position;
	CVert3D end_position;
	CVert3D dir_vector;

	CBSPNode *last_start_node;
	CBSPNode *last_end_node;

	int hit_something;
	int hit_poly_index;

	CPolygonList *poly_hit_list;
	CPolygonList *hit_list_pointer; // could point to poly_hit_list or start_node->polygons

	// --------------------------------------------------------------------------------- 
	// constructor/destructor
	// --------------------------------------------------------------------------------- 
	CObject();
	~CObject();	

	// --------------------------------------------------------------------------------- 
	// movement
	// --------------------------------------------------------------------------------- 
	void apply_rotation(float dx, float dy, float dz);
	void apply_force(float force, float x, float y, float z);
	CVert3D get_orientation();
	void set_orientation(float x, float y, float z);
	CVert3D get_location();
	void set_location(float x, float y, float z);
	int move_object(matrix1x4 unit, float amount, CRend *api);
	void apply_gravity(CRend *api);
	CVert3D get_dir_vector();

	// --------------------------------------------------------------------------------- 
	// accessor methods
	// --------------------------------------------------------------------------------- 
	int isactive() { return active; }
	void activate() { active = 1;} 
	void deactivate() { active = 0; }

	// --------------------------------------------------------------------------------- 
	// override the = operator to copy the correct information
	// --------------------------------------------------------------------------------- 
	inline CObject operator=(CObject o)
	{
		height = o.height;           
		speed = o.speed;            
		step_amount = o.step_amount;      
		step_unit = o.step_unit;        
		width = o.width;            
		falltime = o.falltime;         
		mass = o.mass;             
		gravity = o.gravity;          
		active = o.active;
		hitable = o.hitable;

		location = o.location;
		orientation = o.orientation;

		start_position = o.start_position;
		end_position = o.end_position;
		dir_vector = o.dir_vector;

		last_start_node = o.last_start_node;
		last_end_node = o.last_end_node;

		hit_something = o.hit_something;
		hit_poly_index = o.hit_poly_index;

		poly_hit_list = o.poly_hit_list;
		hit_list_pointer = o.hit_list_pointer; 
		
		return *this ;
	}
};

// --------------------------------------------------------------------------------- 
// create a vector of objects
// --------------------------------------------------------------------------------- 
typedef vector<CObject*> CObjectVector;
typedef CObjectVector::iterator CObjectIterator;

#include <list>

// --------------------------------------------------------------------------------- 
// create a list of objects
// --------------------------------------------------------------------------------- 
typedef list<CObject*> CObjectList;
typedef CObjectList::iterator CObjectListIterator;

#endif

// END OF FILE ---------------------------------------------------------------------