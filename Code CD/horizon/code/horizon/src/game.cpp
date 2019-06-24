/*	--------------------------------------------------------------------------------- 
	Filename      - game.cpp 
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Implementation of CGame
	--------------------------------------------------------------------------------- */

#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>

#include <game.h>
#include <global.h>
#include <lbsp.h>
#include <types.h>
#include <ai.h>

#include <particlesystem.h>
#include <hrz_config.h>
#include <rend.h>
#include <bot.h>
#include <player.h>
#include <collectable.h>
#include <sprite.h>

// --------------------------------------------------------------------------------- 
// global definition of game class (required by the callbacks and main.cpp
// --------------------------------------------------------------------------------- 
CGame *c_game;

// --------------------------------------------------------------------------------- 
// constructor
// --------------------------------------------------------------------------------- 
CGame::CGame(void)
{
	CHrzConfig::read_config_file("horizon.ini");
}

// --------------------------------------------------------------------------------- 
// set some variables to initial values and load configuration file
// --------------------------------------------------------------------------------- 
void CGame::initialize(void)
{
	console_active = 0;
	level_load_complete = 0;
	which_level_index = 0;
	game_started = 0;
	bsp = NULL;	
	create_lookups();

	player_one = new CPlayer();
	player_one->pick_up_ammo();

	game_running = 0;

}

// --------------------------------------------------------------------------------- 
// A.I. unit call back handler, used only at the moment to inform the game unit that
// the player has been hit by a bots rocket.
// --------------------------------------------------------------------------------- 
int CGame::ai_callback(int coz)
{
	switch(coz) {
	case 1:
		c_rend->r_api->console_printf("Player was HIT\n");
		player_one->hit_by_rocket();
		break;
	default:
		c_rend->r_api->console_printf("undealt with AI callback %d\n", coz);
	};

	return 1;
}

// --------------------------------------------------------------------------------- 
// callback used by REND_UNIT to inform this unit that certain operations have 
// occured, such as a lightmap being loaded. This allows this unit to display 
// relevant information to the user without the REND_UNIT needing to know anything
// --------------------------------------------------------------------------------- 
int CGame::rend_callback(int coz)
{
	static int count = 0;
	static long lasttime = 0;
	static long thistime;
	CSprite *loadingdone;
	char strbuffer[200];

	static CSprite *l1, *l2, *l3, *l4, *l5;

	switch(coz) {
	case 1: // LOADING LIGHTMAPS CALLBACK MESSAGE ID
		// display the loading lightmaps oscillating bar.
		if(loaded_lightmaps == 0) {


			// loading background
			sprintf(strbuffer, ":%s\\textures\\loadinglightmaps.bmp", CHrzConfig::data_path);

			CSprite *loading = new CSprite("LOADING_LIGHTMAPS", 
				                           strbuffer, 
										   CVert3D(0,0,0), 
										   CVert3D(300,200,0), 
										   512,
										   64);

			REND_UNIT->add_sprite(loading);
			loaded_lightmaps = 1;

			sprintf(strbuffer, ":%s\\textures\\loading_one.bmp", CHrzConfig::data_path);
			l1 = new CSprite("L1", 
							 strbuffer, 
							 CVert3D(0,0,0), 
							 CVert3D(600,200,0), 
							 64,
							 64);
			sprintf(strbuffer, ":%s\\textures\\loading_two.bmp", CHrzConfig::data_path);
			l2 = new CSprite("L2", 
				             strbuffer, 
							 CVert3D(0,0,0), 
							 CVert3D(600,200,0), 
							 64,
							 64);
			sprintf(strbuffer, ":%s\\textures\\loading_three.bmp", CHrzConfig::data_path);
			l3 = new CSprite("L3", 
				             strbuffer, 
							 CVert3D(0,0,0), 
							 CVert3D(600,200,0), 
							 64,
							 64);
			sprintf(strbuffer, ":%s\\textures\\loading_four.bmp", CHrzConfig::data_path);
			l4 = new CSprite("L4", 
				             strbuffer, 
							 CVert3D(0,0,0), 
							 CVert3D(600,200,0), 
							 64,
							 64);
			sprintf(strbuffer, ":%s\\textures\\loading_five.bmp", CHrzConfig::data_path);
			l5 = new CSprite("L5", 
				             strbuffer, 
							 CVert3D(0,0,0), 
							 CVert3D(600,200,0), 
							 64,
							 64);
			l2->render = 0;
			l3->render = 0;
			l4->render = 0;
			l5->render = 0;

			// add all the sprites to the rendering unit
			REND_UNIT->add_sprite(l1);
			REND_UNIT->add_sprite(l2);
			REND_UNIT->add_sprite(l3);
			REND_UNIT->add_sprite(l4);
			REND_UNIT->add_sprite(l5);
		}

		if(count == 1) {
			l1->render = 1;
			l2->render =l3->render =l4->render =l5->render = 0 ;
		}
		if(count == 2) {
			l2->render = 1;
			l1->render =l3->render =l4->render =l5->render = 0 ;
		}
		if(count == 3) {
			l3->render = 1;
			l2->render =l1->render =l4->render =l5->render = 0 ;
		}
		if(count == 4) {
			l4->render = 1;
			l2->render =l3->render =l1->render =l5->render = 0 ;
		}
		if(count == 5) {
			l5->render = 1;
			l2->render =l3->render =l4->render =l1->render = 0 ;
		}
		if(count == 6) {
			l4->render = 1;
			l2->render =l3->render =l1->render =l5->render = 0 ;
		}
		if(count == 7) {
			l3->render = 1;
			l2->render =l1->render =l4->render =l5->render = 0 ;
		}
		if(count == 8) {
			l2->render = 1;
			l1->render =l3->render =l4->render =l5->render = 0 ;
			count = 0;
		}

		
		thistime = GetTickCount();
		if(thistime-lasttime >= 100) {
			count++;
			lasttime = thistime;
		}
	
		break;
	case 5:
		REND_UNIT->remove_sprite("L1");
		REND_UNIT->remove_sprite("L2");
		REND_UNIT->remove_sprite("L3");
		REND_UNIT->remove_sprite("L4");
		REND_UNIT->remove_sprite("L5");
		sprintf(strbuffer, ":%s\\textures\\done.bmp", CHrzConfig::data_path);
		loadingdone = new CSprite("LIGHTMAPS_DONE", 
			                      strbuffer, 
								  CVert3D(0,0,0), 
								  CVert3D(600,200,0), 
								  128,
								  64);
		REND_UNIT->add_sprite(loadingdone);
		break;
	default:
		REND_UNIT->console_printf("GAME UNIT not handling callback with ID %d at the moment\n", coz);
	};
	return 1;
}

// --------------------------------------------------------------------------------- 
// helper function to parse the level definition file
// --------------------------------------------------------------------------------- 
int CGame::parse_def_file(char *str)
{
	char strbuffer[500];
	char dump[255];
	float f1, f2, f3, f4, f5, f6;
	int i1, i2;

	if(strncmp(str, "USEBSP", 6)==0) {
		// Loading the specified BSP file
		sscanf(str, "%s %s", dump, cur_bsp_filename);

		// free up memory from existing BSP
		if(bsp != NULL) {
			REND_UNIT->console_printf("Removing old BSP.\n");
			delete bsp;
		}

		bsp = new CLBSP();
		sprintf(strbuffer, "%s\\%s", CHrzConfig::data_path, cur_bsp_filename);
		if(bsp->read_bsp_tree(strbuffer) <= 0) {
			REND_UNIT->console_printf("Unable to load file %s\n", strbuffer);
			bsp = NULL;
			return 0;
		}
		sprintf(strbuffer, ":%s\\textures\\bsploaded.bmp", CHrzConfig::data_path);
		CSprite *loading = new CSprite("BSP_LOADED", 
								       strbuffer, 
									   CVert3D(0,0,0), 
									   CVert3D(300,250,0), 
									   256,
									   64);
		REND_UNIT->add_sprite(loading);

		char levelimage_filename[255];
		sprintf(levelimage_filename, ":%s\\%s%s\\levelimage.bmp", CHrzConfig::data_path, cur_bsp_filename, "DATA");
		CSprite *loadingimg = new CSprite("LEVEL_IMAGE", levelimage_filename, CVert3D(-1,-1,-1), CVert3D(20,100,0), 256,256);
		REND_UNIT->add_sprite(loadingimg);

		REND_UNIT->set_bsp(bsp);	
		REND_UNIT->console_printf("Loaded BSP file %s\n", cur_bsp_filename);
		return 1;
	}
	if(strncmp(str, "STARTPOS", 8) == 0) {
		sscanf(str, "%s %f %f %f", dump, &f1, &f2, &f3);
		player_one->set_location(f1, f2, f3);
		return 1;
	}
	if(strncmp(str, "ADDPARTICLES", 12) == 0) {
		char particle_name[255];
		sscanf(str, "%s %d %f %f %f %s", dump, &i1, &f1, &f2, &f3, particle_name);
		strcpy(strbuffer, CHrzConfig::data_path);
		strcat(strbuffer, particle_name);
		CParticleSystem *ps = new CParticleSystem(i1, CVert3D(f1,f2,f3), strbuffer);
		REND_UNIT->add_particle_system(new CParticleSystem(i1, CVert3D(f1,f2,f3), strbuffer));
		delete ps; // no longer needed as REND_UNIT has made a copy of it
		REND_UNIT->console_printf("Added particle system %s\n", particle_name);
		return 1;
	}
	if(strncmp(str, "WAYPOINT ", 9)==0) {
		int type = -1;
		sscanf(str, "%s %d %f %f %f", dump, &i1, &f1, &f2, &f3);
		loaded_waypoints.push_back(CWayPoint(CVert3D(f1, f2, f3), WPT_WAIT_AND_TURN));
		return 1;
	}
	if(strncmp(str, "WAYPOINTCHAIN", 13)==0) {
		char read_str[255];
		int points[255];
		strcpy(read_str, "%s %d %d %d %d %d %d %d %d %d %d ");
		sscanf(str, read_str, dump, &i1, &i2, &points[0],
											  &points[1],
											  &points[2],
											  &points[3],
											  &points[4],
											  &points[5],
											  &points[6],
											  &points[7]);

		if(i2 > 8) {
			REND_UNIT->console_printf("Can not have more than 8 waypoints per bot.\n");
			return 0;
		}
		CWayPointChain wpc;
		for(int i=0;i<i2;i++) 
			wpc.add_way_point(loaded_waypoints[points[i]]);

		loaded_wpc.push_back(wpc);
		return 1;
	}
	if(strncmp(str, "NEWBOT", 6)==0) {
		char md2_path[255];
		char skin_name[255];
		sscanf(str, "%s %s %s %f %f %f %f %f %f %d", dump, md2_path, skin_name, &f1, &f2, &f3, &f4, &f5, &f6, &i1);
		strcpy(strbuffer, CHrzConfig::data_path);
		strcat(strbuffer, md2_path);
		CObject *obj = c_ai_manager->ai_manager->add_bot(strbuffer, skin_name, CVert3D(f1, f2, f3), CVert3D(f4, f5, f6), &loaded_wpc[i1]);	
		objectlist.push_back(obj);

		return 1;
	}
	if(strncmp(str, "AMMO", 4)==0) {
		sscanf(str, "%s %f %f %f\n", dump, &f1, &f2, &f3);
		sprintf(dump, "%s\\ammobox.hrz", CHrzConfig::data_path);
		CAmmoBox *ammobox = new CAmmoBox(REND_UNIT, CVert3D(f1,f2,f3), dump, player_one); 	
		ammobox->set_appear_count(500);
		collectables.push_back(ammobox);
		return 1;
	}
	if(strncmp(str, "HEALTHPACK", 10)==0) {
		sscanf(str, "%s %f %f %f\n", dump, &f1, &f2, &f3);
		sprintf(dump, "%s\\healthpack.hrz", CHrzConfig::data_path);
		CHealthPack *healthpack = new CHealthPack(REND_UNIT, CVert3D(f1,f2,f3), dump, player_one); 	
		healthpack->set_appear_count(2000);
		collectables.push_back(healthpack);
		return 1;
	}
	if(strncmp(str, "ORB", 3)==0) {
		sscanf(str, "%s %f %f %f\n", dump, &f1, &f2, &f3);
		sprintf(dump, "%s\\orb.hrz", CHrzConfig::data_path);
		COrb *orb = new COrb(REND_UNIT, CVert3D(f1,f2,f3), dump, player_one); 	
		orb->set_appear_count(-1);
		collectables.push_back(orb);
		return 1;
	
	}
	if(strncmp(str, "TOTALORBS", 9)==0) {
		sscanf(str, "%s %d", dump, &i1);
		total_orbs_to_collect = i1;
		return 1;
	}	

	return 0;
}

// --------------------------------------------------------------------------------- 
// load a level definition file
// --------------------------------------------------------------------------------- 
void CGame::load_level_def(char *filename_in)
{
	char filename[255];
	// Parse the level definition file
	sprintf(filename, "%s\\%s%s", CHrzConfig::data_path, filename_in, ".def");

	CParse *parser = new CParse();	
	if(parser->open_file(filename) == 0) {
		REND_UNIT->console_printf("Unable to load file %s\n", filename);
		return;
	}

	// clear internal containers
	collectables.clear();
	loaded_wpc.clear();
	loaded_waypoints.clear();
	total_orbs_to_collect = -1;
	level_complete = 0;

	// reset players orbs count
	player_one->orbs = 0;

	// load level is just started
	level_load_complete = 0;

	// load lightmap indicator
	loaded_lightmaps = 0;

	// clear GAME UNIT containers
	collectables.clear();
	objectlist.clear();

	game_running = 0;

	// reset the other units
	REND_UNIT->flush_system();
	c_ai_manager->ai_manager->flush_system();
	

	// stop rendering the world
	REND_UNIT->set_state_var(STATE_RENDER_BSP, STATE_RENDER_BSP_OFF);

	// remove logo if it is present
	REND_UNIT->remove_sprite("TITLE_LOGO");
	REND_UNIT->remove_sprite("HEALTH_BAR_TEX");
	REND_UNIT->remove_sprite("AMMO_BAR_TEX");
	REND_UNIT->remove_sprite("HEALTH_ONE");
	REND_UNIT->remove_sprite("HEALTH_TWO");
	REND_UNIT->remove_sprite("HEALTH_THREE");
	REND_UNIT->remove_sprite("HEALTH_FOUR");
	REND_UNIT->remove_sprite("DEATH");
	REND_UNIT->remove_sprite("GAME_COMPLETE");

	char strbuffer[255];
	for(int i=0;i<10;i++) {
		sprintf(strbuffer, "AMMO%d\n", i);
		REND_UNIT->remove_sprite(strbuffer);
	}

	// add loading screen
	sprintf(strbuffer, ":%s\\textures\\loadingscreen.bmp", CHrzConfig::data_path);
	CSprite *loading = new CSprite("LOADING_SCREEN", 
		                           strbuffer, 
								   CVert3D(-1,-1,-1), 
								   CVert3D(0,-(1024-590),0), 
								   1024,
								   1024);
	REND_UNIT->add_sprite(loading);

	// close open console
	REND_UNIT->set_state_var(STATE_DROP_DOWN_CONSOLE, STATE_DROP_DOWN_DISABLED);

	parser->read_line();

	while(!parser->cmp("#END")) {
		if(!parser->cmp("#")) {
			parse_def_file(parser->line);
		}
		parser->read_line();
	}
	delete parser;
	
	REND_UNIT->console_printf("Loaded %d waypoint positions.\n", loaded_waypoints.size());
	REND_UNIT->console_printf("Loaded %d waypoint chains.\n", loaded_wpc.size());

	level_load_complete = 1;
	player_one->activate();
}

// --------------------------------------------------------------------------------- 
// iterate through the list of collectables for the current level. If one is near
// enough to the player to be collected, perform that collectables collected()
// function.
// --------------------------------------------------------------------------------- 
int CGame::process_collectables()
{
	
	for(CCollectableIterator itor = collectables.begin(); itor != collectables.end(); itor++) {
		CCollectable *collectable = (*itor);

		collectable->animate();

		if(collectable->isactive() && collectable->picked_up()) {			
			collectable->deactivate();
			collectable->collected();
		}

		if(!collectable->isactive() && collectable->reappeard()) {
			collectable->activate();
		}

	}

	return 1;
}

// --------------------------------------------------------------------------------- 
// as C++ forbids direct access to a classes functions, this global function is
// needed to pass the callback information to the game executable
// --------------------------------------------------------------------------------- 
int dummy_callback(int message)
{
	c_game->rend_callback(message);
	return 1;
}

// --------------------------------------------------------------------------------- 
// as above but for the AI units messages
// --------------------------------------------------------------------------------- 
int dummy_ai_callback(int message)
{
	c_game->ai_callback(message);
	return 1;
}

// --------------------------------------------------------------------------------- 
// initialisation steps
// --------------------------------------------------------------------------------- 
int CGame::start(void)
{	
	char filename[255];

	// set the call back function
	c_rend->r_api->set_call_back(dummy_callback);
	c_ai_manager->ai_manager->set_call_back(dummy_ai_callback);

	// Add font here so it can be used in loading screen
	sprintf(filename, "%s\\%s", CHrzConfig::data_path, "\\textures\\font.bmp");
	REND_UNIT->set_console_font(filename); 
	
	REND_UNIT->console_printf("Starting the AI unit\n");
	c_ai_manager->ai_manager->set_rend(REND_UNIT);
	c_ai_manager->ai_manager->set_viewer(player_one);
	c_ai_manager->ai_manager->start();
	
	bsp = NULL;
	REND_UNIT->set_bsp(NULL);		

	char strbuffer[200];
	c_rend->r_api->console_printf("DATA PATH IS %s\n", CHrzConfig::data_path);
	sprintf(strbuffer, ":%s\\textures\\titlescreen.bmp", CHrzConfig::data_path);

	CSprite *titlelogo = new CSprite("TITLE_LOGO", 
		                             strbuffer, 
									 CVert3D(-1,-1,-1), 
									 CVert3D(0,-(1024-590),0), 
									 1024,
									 1024);
	REND_UNIT->add_sprite(titlelogo);


	sprintf(strbuffer, ":%s\\textures\\health_1.bmp", CHrzConfig::data_path);
	health[0] = new CSprite("HEALTH_ONE", 
		                    strbuffer, 
							CVert3D(0,0,0), 
							CVert3D(130,10,0), 
							32, 
							32);
	REND_UNIT->add_sprite(health[0]);

	sprintf(strbuffer, ":%s\\textures\\health_2.bmp", CHrzConfig::data_path);
	health[1] = new CSprite("HEALTH_TWO", 
		                    strbuffer, 
							CVert3D(0,0,0), 
							CVert3D(160,10,0), 
							32, 
							32);
	REND_UNIT->add_sprite(health[1]);

	sprintf(strbuffer, ":%s\\textures\\health_3.bmp", CHrzConfig::data_path);
	health[2] = new CSprite("HEALTH_THREE", 
		                    strbuffer, 
							CVert3D(0,0,0), 
							CVert3D(190,10,0), 
							32, 
							32);
	REND_UNIT->add_sprite(health[2]);

	sprintf(strbuffer, ":%s\\textures\\health_4.bmp", CHrzConfig::data_path);
	health[3] = new CSprite("HEALTH_FOUR", 
		                    strbuffer, 
							CVert3D(0,0,0), 
							CVert3D(220,10,0), 
							32, 
							32);
	REND_UNIT->add_sprite(health[3]);

	health[0]->render = health[1]->render = health[2]->render = health[3]->render = 0;


	return 1;
}

// --------------------------------------------------------------------------------- 
// move the players rockets along their path until they hit the environment or a bot
// when they hit, cause the rocket to explode.
// --------------------------------------------------------------------------------- 
void CGame::move_bullets()
{
	int count = 0;

	for(CBulletListIterator itor = player_one->shells.begin(); itor != player_one->shells.end(); itor++) {
		CBullet *bullet = *itor;

		if(bullet->active == 1) {
			bullet->move(REND_UNIT);
			count++;	
			if(bullet->hit_wall()) {
				bullet->explode(REND_UNIT);
				bullet->active = 0;
				REND_UNIT->remove_object(bullet); // remove from REND UNIT
//				player_one->shells.remove(*itor++);     // remove from players armoury
				continue;
			}
			
			// check to see if bullet hit objects
			for(CObjectIterator obj_itor = objectlist.begin(); obj_itor != objectlist.end(); obj_itor++) {
				CObject *obj = (*obj_itor);
				if(obj->isactive()) {
					if(bullet->hit_object(obj)) {
						CBot *bot = (CBot*)obj;
						bot->botstates[BOT_STATE_DEAD]->change_state();
						bullet->explode(REND_UNIT);
						bullet->active = 0;
						REND_UNIT->remove_object(bullet); // remove from REND UNIT
//						player_one->shells.remove(*itor++);     // remove from players armoury
						break;
					}
				}
			}			
		}
	}
}

// --------------------------------------------------------------------------------- 
// launch one of the players available rockets
// --------------------------------------------------------------------------------- 
void CGame::fire_gun()
{
	if(player_one->armed_shells() > 0) {

		for(CBulletListIterator itor = player_one->shells.begin(); itor != player_one->shells.end(); itor++) {
			CBullet *bullet = *itor;
		
			if(bullet->isarmed()) {
				bullet->disarm();
				bullet->activate();
				CVert3D ori = player_one->get_orientation();
				if(ori.y == 0) ori.y = 1;
				bullet->set_details(player_one->location, ori);
				c_rend->r_api->add_object(bullet);
				break;
			}

		}
	}
}

// --------------------------------------------------------------------------------- 
// debug function to calculate the current framerate
// --------------------------------------------------------------------------------- 
void CGame::calculate_frame_rate()
{
    static float frames_per_second    = 0.0f;                           // This will store our fps
	static float last_time           = 0.0f;                           // This will hold the time from the last frame
   
	static float frame_time = 0.0f;				// This stores the last frame's time

	float current_time = GetTickCount() * 0.001f;                

	frame_time = current_time;

    ++frames_per_second;

    if( current_time - last_time > 1.0f )
    {
        last_time = current_time;   
        REND_UNIT->cons_printf(1, "Current Frames Per Second: %d", int(frames_per_second));
        frames_per_second = 0;
    }
}

// --------------------------------------------------------------------------------- 
// translate the mouse movements into changes of the players viewpoint
// --------------------------------------------------------------------------------- 
int CGame::process_mouse(void)
{
	static int oldx = -1;
	static int oldy = -1;
	static int countdown = 0;

	countdown--;

	if(game_running) {
		int x = dx_stuff->mouse_state.lX;
		int y = dx_stuff->mouse_state.lY;

		if(dx_stuff->mouse_state.rgbButtons[0] != 0 && countdown <= 0) {
			countdown = 10;
			fire_gun();	
		}

		if(oldx != -1 && oldy != -1) {

				if(CHrzConfig::invert_mouse)
					player_one->apply_rotation(0, (float)-x/5.0f, 0);
				else
					player_one->apply_rotation(0, (float)x/5.0f, 0);

				player_one->apply_rotation((float)y/5.0f, 0, 0);
		}

		oldx = x;
		oldy = y;
	}

	return 1;
}

// --------------------------------------------------------------------------------- 
// if gravity effects are enabled apply them to the player
// --------------------------------------------------------------------------------- 
void CGame::do_gravity(void)
{
	if(CHrzConfig::gravity == 1) {
		player_one->apply_gravity(REND_UNIT);		
	}
}

// --------------------------------------------------------------------------------- 
// process the possible drop down console command
// --------------------------------------------------------------------------------- 
int CGame::process_options(char *msg)
{
	if(strncmp(msg, "CLIP_ON", 7)==0) {
		REND_UNIT->set_state_var(STATE_CLIPPING, STATE_CLIP_ON);
		rend_send_msg(7, "Clipping is switched ON\n","");
		return 0;
	}
	if(strncmp(msg, "CLIP_OFF", 8)==0) {
		REND_UNIT->set_state_var(STATE_CLIPPING, STATE_CLIP_OFF);
		rend_send_msg(7, "Clipping is switched OFF\n","");
		return 0 ;
	}
	if(strncmp(msg, "GRAVITY_OFF", 11)==0) {
		CHrzConfig::gravity = 0;
		rend_send_msg(7, "Gravity is now OFF\n","");
		return 0;
	}
	if(strncmp(msg, "GRAVITY_ON", 10)==0) {
		CHrzConfig::gravity = 1;
		rend_send_msg(7, "Gravity is now ON\n","");
		return 0;
	}
	if(strncmp(msg, "FILL_OFF", 8) == 0) {
		REND_UNIT->set_state_var(STATE_FILL_TYPE, STATE_FILL_LINE);
		rend_send_msg(7, "Rendering outlines now.\n","");
		return 0;
	}
	if(strncmp(msg, "FILL_ON", 7) == 0) {
		REND_UNIT->set_state_var(STATE_FILL_TYPE, STATE_FILL_POLY);
		rend_send_msg(7, "Rendering polygons now.\n","");
		return 0;
	}
	if(strncmp(msg, "QUIT", 4) == 0) {
		exit(1);		
	}
	if(strncmp(msg, "PARTICLES_OFF", 13) == 0) {
		REND_UNIT->set_state_var(STATE_PARTICLES_ACTIVE, STATE_PARTICLES_OFF);
		rend_send_msg(7, "Particles are NOT being rendered.\n","");
		return 0;
	}
	if(strncmp(msg, "PARTICLES_ON", 12) == 0) {
		REND_UNIT->set_state_var(STATE_PARTICLES_ACTIVE, STATE_PARTICLES_ON);
		rend_send_msg(7, "Particles are being rendered.\n","");
		return 0;
	}
	if(strncmp(msg, "DEBUG_CON_ON", 12) == 0) {
		REND_UNIT->set_state_var(STATE_DEBUG_CONSOLE, STATE_DEBUG_CONSOLE_ON);
		rend_send_msg(7, "Debug console is being rendered.\n","");
		return 0;
	}
	if(strncmp(msg, "DEBUG_CON_OFF", 13) == 0) {
		REND_UNIT->set_state_var(STATE_DEBUG_CONSOLE, STATE_DEBUG_CONSOLE_OFF);
		rend_send_msg(7, "Debug console is NOT being rendered.\n","");
		return 0;
	}
	if(strncmp(msg, "LIGHTMAPS_ON", 12)==0) {
		REND_UNIT->set_state_var(STATE_LIGHTMAPS_ACTIVE, STATE_LIGHTMAPS_ON);
		rend_send_msg(7, "Lightmaps are being used.\n","");
		return 0;
	}
	if(strncmp(msg, "LIGHTMAPS_OFF", 13)==0) {
		REND_UNIT->set_state_var(STATE_LIGHTMAPS_ACTIVE, STATE_LIGHTMAPS_OFF);
		rend_send_msg(7, "Lightmaps are NOT being used.\n","");
		return 0;
	}
	if(strncmp(msg, "OLD_REND_ON", 11)==0) {
		REND_UNIT->set_state_var(STATE_OLD_STYLE_REND, STATE_OLD_STYLE_REND_ON);
		rend_send_msg(7, "Recursive rendering.\n","");
		return 0;
	}
	if(strncmp(msg, "OLD_REND_OFF", 12)==0) {
		REND_UNIT->set_state_var(STATE_OLD_STYLE_REND, STATE_OLD_STYLE_REND_OFF);
		rend_send_msg(7, "PVS rendering.\n","");
		return 0;
	}
	if(strncmp(msg, "LOAD ", 5) == 0) {
		char filename[100];
		char dump[100];
		sscanf(msg, "%s %s", dump, filename);
		load_level_def(filename);
		player_one->health = 100; // reset the players health
		return 0;
	}
	if(strncmp(msg, "START", 5)==0) {
		if(CHrzConfig::level_count <= 0) {
			rend_send_msg(7, "No levels have been added, check config file.\n","");
			return 0;
		}
		load_level_def(CHrzConfig::levels[0]);
		which_level_index = 1;
		game_started = 1;
		player_one->health = 100;
		return 0;
	}

	char buffer[100];
	sprintf(buffer, "Unknown MSG: %s\n", msg);
	rend_send_msg(7, buffer, "");

	return 1;
}


// --------------------------------------------------------------------------------- 
// handle input, redirect the output to the drop down console if its open. otherwise
// process it as movement commands for the player. 
// --------------------------------------------------------------------------------- 
int CGame::process_input(void)
{
	static char msg_buffer[100];
	static int  msg_pos   = 0;
	static char last_key  = ' ';
	static int  skipcount = 0;
	char strbuffer[255];

	matrix1x4 unit;

	skipcount--; // prevent letters being repeated

	dx_stuff->updateinput();

	process_mouse();

	if(dx_stuff->key_down(DIK_GRAVE) && skipcount <= 0) {
		msg_pos = 0; memset(msg_buffer, 0, 100); // clear message buffer
		if(console_active == 1) console_active = 0;
			else console_active = 1;
		rend_send_msg(MSG_KEY_PRESSED, "~", "Console Key Pressed");
		skipcount = 30;
	}		


	// Add input to the drop down console
	if( console_active ) {

		for(int i=0;i<256;i++) { // Deals with ASCII characters
			if(dx_stuff->keystate[i] == 1) { 
				char ascii_char = dx_stuff->scan2ascii(i);
				if( (last_key != ascii_char) || skipcount <= 0 ) {
					if( (ascii_char >= 'a' && ascii_char <= 'z') || (ascii_char >= 'A' && ascii_char <= 'Z') || (ascii_char >= '0' && ascii_char <= '9')) {
						char buf[2];
						sprintf(buf, "%c", ascii_char);
						rend_send_msg(MSG_BUFFER_ADD_CHAR, buf, "");
						msg_buffer[msg_pos++] = ascii_char;
						skipcount = 12;
						last_key = ascii_char;
					}
				}
			}
		}

		if(skipcount <= 0) {
			
			if(dx_stuff->key_down(DIK_BACKSPACE)) {
				if(msg_pos > 0) {
					msg_buffer[--msg_pos] = ' ';
					rend_send_msg(MSG_BUFFER_REMOVE_CHAR, "", "");
					skipcount = 12;
				}
			} else
			if(dx_stuff->key_down(DIK_RETURN)) {
				rend_send_msg(MSG_BUFFER_ADD_CHAR, "\n", "");
				rend_send_msg(MSG_BUFFER_PROCESS_INPUT, "","");
				process_options(msg_buffer);
				msg_pos = 0; memset(msg_buffer, 0, 100); // clear message buffer
				skipcount = 12;
			} else
			if(dx_stuff->key_down(DIK_MINUS) && dx_stuff->key_down(DIK_LSHIFT)) {
				rend_send_msg(MSG_BUFFER_ADD_CHAR, "_","");
				msg_buffer[msg_pos++] = '_';
				skipcount = 12;
			} else
			if(dx_stuff->key_down(DIK_SPACE)) {
				rend_send_msg(MSG_BUFFER_ADD_CHAR, " ","");
				msg_buffer[msg_pos++] = ' ';
				skipcount = 12;
			}
			if(dx_stuff->key_down(DIK_PGDN)) {
				rend_send_msg(MSG_CONSOLE_SCROLL_UP, "", "Scroll Down");
			} else
			if(dx_stuff->key_down(DIK_PGUP)) {
				rend_send_msg(MSG_CONSOLE_SCROLL_DOWN, "", "Scroll UP");
			} 

			if(msg_pos >= 100) {
				rend_send_msg(MSG_BUFFER_PROCESS_INPUT, "","");
				msg_pos = 0; memset(msg_buffer, 0, 100); // clear message buffer
				rend_send_msg(MSG_BUFFER_ADD_MSG, "Went over buffer limit so resetting input.\n", "");
			}
		
		}

	} else { // console is NOT active
		
		if(game_running) {

		if(dx_stuff->key_down(DIK_S)) {
			MATRIX1X4(unit, 0, 0, 1, 0);
			player_one->move_object(unit, CHrzConfig::move_amount_backward, REND_UNIT);
		} 

		if(dx_stuff->key_down(DIK_W)) {
			MATRIX1X4(unit, 0, 0, -1, 0);
			player_one->move_object(unit, CHrzConfig::move_amount_forward, REND_UNIT);
		} 
	
		if(dx_stuff->key_down(DIK_A)) {
			MATRIX1X4(unit, -1, 0, 0, 0);
			player_one->move_object(unit, CHrzConfig::move_amount_forward, REND_UNIT);
		} 

		if(dx_stuff->key_down(DIK_D)) {
			MATRIX1X4(unit, 1, 0, 0, 0);
			player_one->move_object(unit, CHrzConfig::move_amount_forward, REND_UNIT);
		} 

		if(dx_stuff->key_down(DIK_F)) {
			fire_gun();
		} 

		}

	}

	// handle spaces being pressed after level has loaded
	if(level_load_complete == 1) {
		if(dx_stuff->key_down(DIK_SPACE)) {
			level_load_complete = 2;

			REND_UNIT->remove_sprite("LOADING_SCREEN");
			REND_UNIT->remove_sprite("BSP_LOADED");
			REND_UNIT->remove_sprite("LOADING_LIGHTMAPS");
			REND_UNIT->remove_sprite("LIGHTMAPS_DONE");
			REND_UNIT->remove_sprite("LEVEL_IMAGE");

			sprintf(strbuffer, ":%s\\textures\\health.bmp", CHrzConfig::data_path);
			CSprite *healthbar = new CSprite("HEALTH_BAR_TEX", 
				                             strbuffer, 
											 CVert3D(0,0,0), 
											 CVert3D(10,10,0), 
											 128, 
											 32);
			REND_UNIT->add_sprite(healthbar);

			sprintf(strbuffer, ":%s\\textures\\ammo.bmp", CHrzConfig::data_path);
			CSprite *ammobar = new CSprite("AMMO_BAR_TEX", 
				                           strbuffer, 
										   CVert3D(0,0,0), 
										   CVert3D(320,10,0), 
										   128, 
										   32);
			REND_UNIT->add_sprite(ammobar);
			
			sprintf(strbuffer, ":%s\\textures\\health_1.bmp", CHrzConfig::data_path);
			health[0] = new CSprite("HEALTH_ONE", 
		                    strbuffer, 
							CVert3D(0,0,0), 
							CVert3D(130,10,0), 
							32, 
							32);
			REND_UNIT->add_sprite(health[0]);

			sprintf(strbuffer, ":%s\\textures\\health_2.bmp", CHrzConfig::data_path);
			health[1] = new CSprite("HEALTH_TWO", 
		                    strbuffer, 
							CVert3D(0,0,0), 
							CVert3D(160,10,0), 
							32, 
							32);
			REND_UNIT->add_sprite(health[1]);

			sprintf(strbuffer, ":%s\\textures\\health_3.bmp", CHrzConfig::data_path);
			health[2] = new CSprite("HEALTH_THREE", 
		                    strbuffer, 
							CVert3D(0,0,0), 
							CVert3D(190,10,0), 
							32, 
							32);
			REND_UNIT->add_sprite(health[2]);

			sprintf(strbuffer, ":%s\\textures\\health_4.bmp", CHrzConfig::data_path);
			health[3] = new CSprite("HEALTH_FOUR", 
		                    strbuffer, 
							CVert3D(0,0,0), 
							CVert3D(220,10,0), 
							32, 
							32);
			REND_UNIT->add_sprite(health[3]);

			health[0]->render = health[1]->render = health[2]->render = health[3]->render = 0;


			for(int i=0;i<10;i++) {
				char strbuffer2[255];
				sprintf(strbuffer, ":%s\\textures\\ammoshell.bmp", CHrzConfig::data_path);
				sprintf(strbuffer2, "AMMO%d\n", i);
				ammo[i] = new CSprite(strbuffer2, 
					                  strbuffer, 
									  CVert3D(0,0,0), 
									  CVert3D(440 + (i*32), 10, 0), 
									  32, 
									  64);
				REND_UNIT->add_sprite(ammo[i]);
			}
			

			health[0]->render = 0;
			health[1]->render = 0;
			health[2]->render = 0;
			health[3]->render = 0;

			REND_UNIT->set_state_var(STATE_DROP_DOWN_CONSOLE, STATE_DROP_DOWN_ACTIVE);
			REND_UNIT->set_state_var(STATE_RENDER_BSP, STATE_RENDER_BSP_ON);

			game_running = 1;
		}
	}

	// handle space being pressed after a level has been completed
	if(level_complete == 1) {
		if(dx_stuff->key_down(DIK_SPACE)) {
			REND_UNIT->remove_sprite("LEVEL_COMPLETE");
			if(!game_started) {
				load_level_def(CHrzConfig::levels[0]);
				which_level_index = 1;
			} else {
				if(which_level_index < CHrzConfig::level_count) {
					load_level_def(CHrzConfig::levels[which_level_index]);
					which_level_index++;
				} 
			}
		}
	}


	return 1;
}

// --------------------------------------------------------------------------------- 
// this is the main loop of CGame, called once per frame
// --------------------------------------------------------------------------------- 
int CGame::update(void)
{
	static float run_count = 0;
	char strbuffer[255];

	process_input();

	if(game_running) {
		
		// light the players health bars
		switch(player_one->health) {
		case 100: 
			health[0]->render = health[1]->render = health[2]->render = health[3]->render = 1;
			break;
		case 75:
			health[1]->render = health[2]->render = health[3]->render = 1;
			health[0]->render = 0;
			break;
		case 50:
			health[2]->render = health[3]->render = 1;
			health[0]->render = health[1]->render = 0;
			break;
		case 25:
			health[3]->render = 1;
			health[0]->render = health[1]->render = health[2]->render = 0;
			break;
		case 0:
			health[0]->render = health[1]->render = health[2]->render = health[3]->render = 0;
		}

		// display correct number of rocket icons
		int num_bullets = player_one->armed_shells() % 10;

		for(int i=0;i<num_bullets;i++) {
			ammo[i]->render = 1;
		}
		for(i=num_bullets;i<10;i++) {
			ammo[i]->render = 0;
		}

		if(num_bullets == 0 && player_one->armed_shells() >= 10) {
			for(int i=0;i<10;i++) {
				ammo[i]->render = 1;
			}
		}


		// if the players health is 0 display message and then restart the game.
		if(player_one->health <= 0 && player_one->isactive()) {
			
			sprintf(strbuffer, ":%s\\textures\\death.bmp", CHrzConfig::data_path);
			CSprite *death = new CSprite("DEATH", strbuffer, CVert3D(-1,-1,-1), CVert3D(0,-(1024-600),0), 1024, 1024);
			c_rend->r_api->add_sprite(death);
			game_running = 0;
			player_one->deactivate();
		}
		

		// apply gravity to player
		do_gravity();

		// update the renderer, so that the viewpoint is correct
		REND_UNIT->update(player_one);   // update RENDERER unit according to viewer player_one						 	

		// move all the active bullets
		move_bullets(); 
		
		// see if player can pick up anything
		process_collectables();

		// check if number of orbs collected is enough to complete the level.
		if(!level_complete) {
			if(player_one->orbs == total_orbs_to_collect) {
				c_rend->r_api->console_printf("Collected enough ORBS, level completed.\n");

				if(which_level_index < CHrzConfig::level_count) {
					level_complete = 1;
					sprintf(strbuffer, ":%s\\textures\\complete.bmp", CHrzConfig::data_path);
					CSprite *level_complete = new CSprite("LEVEL_COMPLETE", strbuffer, CVert3D(-1,-1,-1), CVert3D(160,100,0), 512,256);
					c_rend->r_api->add_sprite(level_complete);
				} else {
					// all levels complete so game over
					sprintf(strbuffer, ":%s\\textures\\gamecomplete.bmp", CHrzConfig::data_path);
					CSprite *game_comp = new CSprite("GAME_COMPLETE", strbuffer, CVert3D(-1,-1,-1), CVert3D(0,-(1024-600),0), 1024, 1024);
					c_rend->r_api->add_sprite(game_comp);			
					game_running = 0;
				}
			}
		}

		// update the A.I. unit
		c_ai_manager->ai_manager->update();  // update AI unit

	} else {

		// game is not running, but make sure screen images are still updated
		REND_UNIT->update(NULL);   // update RENDERER unit, no position required
	}

	if(CHrzConfig::debug) 
		calculate_frame_rate();

	return 1;
}

// --------------------------------------------------------------------------------- 
// routine to pass a message to the rendering unit
// --------------------------------------------------------------------------------- 
void CGame::rend_send_msg(int dest, char *details, char *desc)
{
	Message msg;

	msg.dest = dest;
	strcpy(msg.details, details);
	strcpy(msg.desc, desc);

	REND_UNIT->send_msg(&msg);
}

// END OF FILE ---------------------------------------------------------------------