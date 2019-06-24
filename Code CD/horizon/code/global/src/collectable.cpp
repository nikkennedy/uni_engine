/*	--------------------------------------------------------------------------------- 
	Filename      - collectable.cpp 
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Implementation of CCollectable, an object that player can collect
				    includes CAmmoBox, COrb and CHealthPack
	--------------------------------------------------------------------------------- */

#include <object.h>
#include <collectable.h>
#include <hrz_parse.h>
#include <rend.h>
#include <player.h>

// --------------------------------------------------------------------------------- 
// constructor
// --------------------------------------------------------------------------------- 
CCollectable::CCollectable(CRend *rend, CVert3D location, char *shape_filename, CObject *collector)
{
	// load the collectables model file
	CHrzParser *new_parser = new CHrzParser();
	new_parser->parse_file(shape_filename);

	this->location = location;
	this->shape = new_parser;
	this->md2_model = NULL;
	this->reappear_count = -1;
	this->collector = collector;

	activate();

	rend->add_object(this); // renderer needs to know about this
}

// --------------------------------------------------------------------------------- 
// has the collectable been picked up
// --------------------------------------------------------------------------------- 
int CCollectable::picked_up()
{
	CVert3D collector_loc = collector->location;
	
	float dist = (CVert3D(collector_loc - this->location)).length();

	if(dist < 5)
		return 1;

	return 0;
}

// --------------------------------------------------------------------------------- 
// set how long before collectable appears again
// --------------------------------------------------------------------------------- 
void CCollectable::set_appear_count(int count)
{
	reappear_count = count;
	active_count = count;
}

// --------------------------------------------------------------------------------- 
// check to see if collectable is to reappear
// --------------------------------------------------------------------------------- 
int CCollectable::reappeard()
{
	if(reappear_count == -1)
		return 0;

	if(--active_count <= 0) {
		active_count = reappear_count;
		return 1;
	}

	return 0;		
}

// --------------------------------------------------------------------------------- 
// rotate and move up and down the collectable
// --------------------------------------------------------------------------------- 
void CCollectable::animate()
{
	// rotate and bounce collectables
	apply_rotation(0,1,0);
	location.y += ((float)sin_lookup[(int)orientation.y] / 100.0f);
}

// --------------------------------------------------------------------------------- 
// CAmmoBox subclass ... overide collected 
// --------------------------------------------------------------------------------- 
void CAmmoBox::collected()
{
	CPlayer *player = (CPlayer *)collector;
	player->pick_up_ammo();
}

// --------------------------------------------------------------------------------- 
// COrbBox subclass ... overide collected 
// --------------------------------------------------------------------------------- 
void COrb::collected()
{
	CPlayer *player = (CPlayer *)collector;
	player->pick_up_orb();
}

// --------------------------------------------------------------------------------- 
// CHealthBox subclass ... overide collected 
// --------------------------------------------------------------------------------- 
void CHealthPack::collected()
{
	CPlayer *player = (CPlayer *)collector;
	player->pick_up_health();
}

// END OF FILE ----------------------------------------------------------------------