/*	--------------------------------------------------------------------------------- 
	Filename      - player.h
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - The player object definition
	--------------------------------------------------------------------------------- */

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <object.h>
#include <bullet.h>

// --------------------------------------------------------------------------------- 
// create a vector of bullets that the player can hold
// --------------------------------------------------------------------------------- 
#include <list>
using namespace std;

typedef list<CBullet*> CBulletList;
typedef CBulletList::iterator CBulletListIterator;

// --------------------------------------------------------------------------------- 
// CPlayer
// --------------------------------------------------------------------------------- 
class CPlayer : public CObject
{
public:

	// --------------------------------------------------------------------------------- 
	// list of bullets held by player
	// --------------------------------------------------------------------------------- 
	CBulletList shells;  // available rockets

	int orbs;             // number of orbs collected
	int health;           // current health level, when = 0 dead

	CPlayer();            // create the player

	void pick_up_ammo();  // add a box of ammo to the players armory
	void pick_up_orb();   // increase orb count
	void pick_up_health(); // increase health
	void hit_by_rocket(); // reduce health
	int armed_shells(); // number of active rockets ready to fire
};

#endif

// END OF FILE --------------------------------------------------------------