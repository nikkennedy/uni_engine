/*	--------------------------------------------------------------------------------- 
	Filename      - player.cpp
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - The player object implementation
	--------------------------------------------------------------------------------- */

#include <player.h>
#include <game.h>

// --------------------------------------------------------------------------------- 
// constructor
// --------------------------------------------------------------------------------- 
CPlayer::CPlayer()
{
	active = 1;

	// no model representation
	shape = NULL;
	md2_model = NULL;

	set_orientation(0,0,0);
	set_location   (100,10,100);
	
	height      = 6.5f;
	width       = 5.0f;
	step_unit   = 0.1f;
	step_amount = 5.0;
	mass        = 2.0f;
	gravity     = 2.0f;

	health      = 100; 
	orbs = 0;
	
	// create the players rockets (pre-load them)
	for(int i=0;i<GAME_MAX_AMMO;i++) {
		CBullet *bullet = new CBullet();
		shells.push_back(bullet);
	}	
}

// --------------------------------------------------------------------------------- 
// called when ammo collectable is collected
// --------------------------------------------------------------------------------- 
void CPlayer::pick_up_ammo()
{
	int count = 0;

	for(CBulletListIterator itor = shells.begin(); itor != shells.end(); itor++) {
		CBullet *bullet = *itor;

		if(!bullet->isarmed()) {
			bullet->arm();
			count++;
		}
		if(count >= GAME_BOX_AMMO_SIZE)
			return;
	}
}

// --------------------------------------------------------------------------------- 
// called when orb is collected
// --------------------------------------------------------------------------------- 
void CPlayer::pick_up_orb()
{
	orbs++;
}

// --------------------------------------------------------------------------------- 
// increase health because health pack picked up
// --------------------------------------------------------------------------------- 
void CPlayer::pick_up_health()
{
	if(health < 100) health += 25;
}

// --------------------------------------------------------------------------------- 
// returns the number of rockets a player has ready to fire
// --------------------------------------------------------------------------------- 
int CPlayer::armed_shells()
{
	int count = 0;

	for(CBulletListIterator itor = shells.begin(); itor != shells.end(); itor++) {
		CBullet *bullet = *itor;

		if(bullet->isarmed()) {
			count++;
		}
	}
	return count;
}

// --------------------------------------------------------------------------------- 
// each rocket reduces players health by 25 units
// --------------------------------------------------------------------------------- 
void CPlayer::hit_by_rocket()
{
	health -= 25;
}

// END OF FILE ---------------------------------------------------------------------