/*	--------------------------------------------------------------------------------- 
	Filename      - aibase.cpp 
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Implementation of CAIBase the abstraction of the A.I. unit 
	--------------------------------------------------------------------------------- */

#include <aibase.h>
#include <rend.h>
#include <object.h>
#include <bot.h>
#include <basictexture.h>

// --------------------------------------------------------------------------------- 
// add a new bot to the A.I. unit, load its MD2 and skin texture and add the model
// to the rendering unit
// --------------------------------------------------------------------------------- 
CObject *CAIBase::add_bot(char *md2_path, char *skin_name, CVert3D pos, CVert3D ori, CWayPointChain *wpc)
{
	char strbuffer[255];

	CBot *bot;

	bot = new CBot(md2_path, skin_name);
	bot->orientation = ori;
	bot->location = pos;
	bot->wpc = wpc;

	bots.push_back(bot);

	// Load the skin texture
	strcpy(strbuffer, ":");
	strcat(strbuffer, md2_path);
	strcat(strbuffer, skin_name);
	rend->get_tex_manager()->add_texture(strbuffer, skin_name);
	// add the weapon skin
	strcpy(strbuffer, ":");
	strcat(strbuffer, md2_path);
	strcat(strbuffer, "w_rlauncher.bmp");
	rend->get_tex_manager()->add_texture(strbuffer, "w_rlauncher.bmp");

	rend->add_object(bot);
	bot->active = 1;

	rend->console_printf("Bots waypoints are: \n");
	bot->wpc.start();
	for(int i=0;i<bot->wpc.count();i++) {
		bot->wpc.next_way_point()->dump(rend);	
	}

	// add object to rendering unit
	rend->add_object(bot->rocket);
	rend->console_printf("AI Unit: new bot has been added (startpos = %f,%f,%f).\n", bot->location.x, bot->location.y, bot->location.z);

	return (CObject*) bot;
	

}

// --------------------------------------------------------------------------------- 
// store a pointer to the rendering unit
// --------------------------------------------------------------------------------- 
void CAIBase::set_rend(CRend *r)
{
	rend = r;
}

// --------------------------------------------------------------------------------- 
// return the number of bots being controlled
// --------------------------------------------------------------------------------- 
int CAIBase::count()
{
	return bots.size();
}

// --------------------------------------------------------------------------------- 
// set a pointer to the current object that represents the player
// --------------------------------------------------------------------------------- 
void CAIBase::set_viewer(CObject *viewer)
{
	this->viewer = viewer;
}

// END OF FILE ---------------------------------------------------------------------