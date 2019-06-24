/*	--------------------------------------------------------------------------------- 
	Filename      - collectable.h 
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Definition of CCollectable, an object that player can collect
				    includes CAmmoBox, COrb and CHealthPack
	--------------------------------------------------------------------------------- */

#ifndef _COLLECTABLE_
#define _COLLECTABLE_

#include <object.h>
#include <vector>

using namespace std;

#define COLLECTABLE_AMMO_BOX 1

// --------------------------------------------------------------------------------- 
// CCollectable
// --------------------------------------------------------------------------------- 
class CCollectable : public CObject
{
protected:
	// --------------------------------------------------------------------------------- 
	// object which collects these objects
	// --------------------------------------------------------------------------------- 
	CObject *collector; 

	// --------------------------------------------------------------------------------- 
	// reappears when = 0
	// --------------------------------------------------------------------------------- 
	int reappear_count;
	int active_count;

public:
	// --------------------------------------------------------------------------------- 
	// constructor
	// --------------------------------------------------------------------------------- 
	CCollectable(CRend *rend, CVert3D location, char *shape_filename, CObject *collector);

	// --------------------------------------------------------------------------------- 
	// has collector got near enough to pick up
	// --------------------------------------------------------------------------------- 
	int picked_up(); 
	void set_appear_count(int count);
	int reappeard();
	// --------------------------------------------------------------------------------- 
	// move model
	// --------------------------------------------------------------------------------- 
	void animate();

	// --------------------------------------------------------------------------------- 
	// override this function to perform action when object is collected by player
	// --------------------------------------------------------------------------------- 
	virtual void collected() = 0;
};

// --------------------------------------------------------------------------------- 
// vector of collecables type
// --------------------------------------------------------------------------------- 
typedef vector<CCollectable*> CCollectableVector;
typedef CCollectableVector::iterator CCollectableIterator;

// --------------------------------------------------------------------------------- 
// CAmmoBox - ammo collectable
// --------------------------------------------------------------------------------- 
class CAmmoBox : public CCollectable
{
public:
	CAmmoBox(CRend *rend, CVert3D location, char *shape_filename, CObject *collector) : 
	  CCollectable(rend, location, shape_filename, collector) {}

	void collected();
};

// --------------------------------------------------------------------------------- 
// COrb - orb collectable
// --------------------------------------------------------------------------------- 
class COrb : public CCollectable
{
public:
	COrb(CRend *rend, CVert3D location, char *shape_filename, CObject *collector) : 
	  CCollectable(rend, location, shape_filename, collector) {}

	void collected();
};

// --------------------------------------------------------------------------------- 
// CHealthPack - health collectable
// --------------------------------------------------------------------------------- 
class CHealthPack : public CCollectable
{
public:
	CHealthPack(CRend *rend, CVert3D location, char *shape_filename, CObject *collector) : 
	  CCollectable(rend, location, shape_filename, collector) {}

	void collected();
};

#endif

// END OF FILE --------------------------------------------------------------