/*	--------------------------------------------------------------------------------- 
	Filename      - aibasic.cpp 
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Implementation of CAIBasic the implementation of the A.I. unit 
		            that uses a finite state machine
	--------------------------------------------------------------------------------- */

#include <windows.h>

#include <types.h>
#include <aibase.h>
#include <aibasic.h>
#include <object.h>
#include <bot.h>
#include <hrz_config.h>

// --------------------------------------------------------------------------------- 
// initialise 
// --------------------------------------------------------------------------------- 
int CAIBasic::start(void)
{	
	create_lookups();
	CHrzConfig::read_config_file("horizon.ini");

	rend->console_printf("AI UNIT is starting up...\n");

	return AI_OK;
}

// --------------------------------------------------------------------------------- 
// called once per update of the screen, loops through each bot calling its
// current state to be executed. also checks to see if the players been hit
// by any bots rockets, if so it uses callback mechanism to inform the game
// such an event has occured.
// --------------------------------------------------------------------------------- 
int CAIBasic::update(void)
{
	for(CBotIterator itor=bots.begin();itor != bots.end(); itor++) {
		
		CBot *b = (*itor);

		b->apply_gravity(rend);
				
		// report position of user
		b->cur_state->report_affectors(viewer->location);
		// perform current state actions
		b->cur_state->do_state(rend);

		CBullet *rocket = b->rocket;

		if(rocket->active == 1) {
			if(rocket->hit_object(viewer)) {
				rocket->explode(rend);
				rocket->active = 0;	
				// inform game unit
				callback(CALLBACK_PLAYER_HIT);
			} 
		}
	}

//	rend->cons_printf(9, "View pos = %f,%f,%f\n", viewer->location.x, viewer->location.y, viewer->location.z);
	return AI_OK;
}

// --------------------------------------------------------------------------------- 
// remove all the bots
// --------------------------------------------------------------------------------- 
void CAIBasic::flush_system()
{
	for(CBotIterator itor=bots.begin();itor != bots.end(); itor++) {		
		CBot *b = (*itor);
		delete (CBot*) b;
	}	
	bots.clear();
	rend->console_printf("Flushed A.I. system\n");
}

// END OF FILE ---------------------------------------------------------------------
