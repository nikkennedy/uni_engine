/*	--------------------------------------------------------------------------------- 
	Filename      - rend.h
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Implementation of CRend
					This IS the framework for the rendering unit, it provides a super
					class for all implementations, the following functions are then
					provided for a game unit, or other application to utilise.

					These functions are generic and don't have to be re-implemented by
					a subclass of CRend
	--------------------------------------------------------------------------------- */

#include <windows.h>
#include <rend.h>
#include <console.h>

// --------------------------------------------------------------------------------- 
// constructor, sets a rendering unit into the default state
// --------------------------------------------------------------------------------- 
CRend::CRend(void)
{
	// clear the console
	current_line = 0;
	for(int i=0;i<30;i++) {
		memset(&txt[i], '\0', 80);
	}

	current_msg = 0;
	total_msg   = 0;
	msg_list = (Message *) malloc(sizeof(Message) * MAX_MESSAGES);

	// default state values
	state_fill_type = STATE_FILL_POLY;
	state_clipping  = STATE_CLIP_ON;
	state_particles_active = STATE_PARTICLES_ON;
	state_debug_console = STATE_DEBUG_CONSOLE_OFF;
	state_bsp_render = STATE_RENDER_BSP_OFF;
	state_drop_down_console = STATE_DROP_DOWN_ACTIVE;
	state_lightmaps_active = STATE_LIGHTMAPS_ON;
}

// --------------------------------------------------------------------------------- 
// used to pass messages to the rendering unit
// --------------------------------------------------------------------------------- 
void CRend::send_msg(Message *msg)
{
	if(total_msg < MAX_MESSAGES) {
		memcpy(&msg_list[current_msg], msg, sizeof(Message));
		total_msg++;
	}
}

// --------------------------------------------------------------------------------- 
// grabs the next message from the queue
// --------------------------------------------------------------------------------- 
Message *CRend::get_next_message(void)
{
	Message *msg = NULL;

	if(total_msg >= 1) {	
		msg = &msg_list[current_msg];
		current_msg++;
		total_msg--;
		if(current_msg == MAX_MESSAGES) {
			current_msg = 0;
		}
	}

	return msg;	
}

// ---------------------------------------------------------------------------
// This function calculates planes perpendicular to each of the sides of a 
// polygon, that way it will be possible to see if an object is within the
// bounds of a polygon
// ---------------------------------------------------------------------------
void CRend::create_perpen_planes(CBSPNode *bsp)
{	
	if(bsp->front == NULL && bsp->back == NULL) {

		for(int i=0;i<bsp->polygons->count();i++) {
			CPoly *poly = bsp->polygons->get(i);

			// also reset display list so a new one is calculated
			bsp->polygons->has_display_list = 0;
			poly->create_per_pen_plane(vertex_list, normal_list);

		}		
		return;
	}

	if(bsp->front != NULL)
		create_perpen_planes(bsp->front);
	if(bsp->back != NULL)
		create_perpen_planes(bsp->back);		
}

// ---------------------------------------------------------------------------
// add a string to the drop down console
// ---------------------------------------------------------------------------
void CRend::console_printf(char *str, ...)
{
	char str_buffer[200];
	va_list v_list;

	memset(str_buffer, 0, 200);

	va_start(v_list, str);
	vsprintf(str_buffer, str, v_list);
	va_end(v_list);
	
	CConsole::console_add_msg(str_buffer);
}

// ---------------------------------------------------------------------------
// redundant rending function, draw all the BSP leaves recursivley (i.e. if
// no PVS)
// ---------------------------------------------------------------------------
void CRend::draw_bsp_tree(CBSPNode *bsp, CVert3D loc)
{
	float classification;

	if(bsp->front == NULL && bsp->back == NULL) {
		draw_polygons(bsp->polygons);
		return;
	}

	CPlane *pl = plane_list[bsp->divider->plane_id];
	classification = pl->classify(loc); //classify_point(pl, &loc);

	if(classification > 0 )  {
		if(bsp->back != NULL)
			draw_bsp_tree(bsp->back, loc);
		if(bsp->front != NULL)
			draw_bsp_tree(bsp->front, loc);
	} else if(classification < 0 || classification == 0) {
		if(bsp->front != NULL)
			draw_bsp_tree(bsp->front, loc);
		if(bsp->back != NULL)
			draw_bsp_tree(bsp->back, loc);
	} 
}

// ---------------------------------------------------------------------------
// process the keypressed message
// ---------------------------------------------------------------------------
void CRend::key_pressed(char *key)
{
	if(is_console_open == 0){

		switch(key[0]) {		
		case '~': console_open();
			break;		
		default:
			cons_printf(12, "Unknown key pressed\n");
			break;
		}
		
	} else {
		if(key[0] == '~')
			console_open();
	}
}

// ---------------------------------------------------------------------------
// process all possible messages
// ---------------------------------------------------------------------------
void CRend::process_message(Message *msg)
{	

	switch(msg->dest) {
	case MSG_KEY_PRESSED: 
		key_pressed(msg->details);
		break;
	case MSG_CONSOLE_SCROLL_UP:
		CConsole::scroll_pos ++;
		break;
	case MSG_CONSOLE_SCROLL_DOWN:
		CConsole::scroll_pos --;
		break;
	case MSG_BUFFER_ADD_CHAR: // ADD CHAR TO THE CONSOLE BUFFER
		CConsole::console_input[CConsole::current_console_input_pos++] = msg->details[0];
		break;
	case MSG_BUFFER_REMOVE_CHAR: // REMOVE CHAR FROM CONSOLE BUFFER
		CConsole::console_input[--CConsole::current_console_input_pos] = ' ';
		break;
	case MSG_BUFFER_PROCESS_INPUT: // ADD CONSOLE INPUT TO CONSOLE MESSAGE BUFFER
		CConsole::console_add_msg(CConsole::console_input);			
		break;
	case MSG_BUFFER_ADD_MSG: // ADD A console MESSAGE
		CConsole::console_add_msg(msg->details);
		break;
	}

	msg->dest = -1;
}

// ---------------------------------------------------------------------------
// write a string to the debug console at position pos
// ---------------------------------------------------------------------------
void CRend::write_console(char *str, int pos)
{
	memset(txt[pos], '\0', 80);
	memcpy(txt[pos], str, strlen(str));
	current_line = 30;
}

// ---------------------------------------------------------------------------
// write a string to the debug console at position pos
// ---------------------------------------------------------------------------
void CRend::cons_printf(int pos, char *str, ...)
{
	char str_buffer[200];
	va_list v_list;

	memset(str_buffer, 0, 200);
	va_start(v_list, str);
	vsprintf(str_buffer, str, v_list);
	write_console(str_buffer, pos);
	va_end(v_list);
}

// ---------------------------------------------------------------------------
// add a string to the debug console to the next available position
// ---------------------------------------------------------------------------
void CRend::add_console(char *str)
{
	if(current_line >= 20) { // shift everything up one
		for(int i=5;i<20;i++) {
			memcpy(txt[i-5], txt[i], 80);
		}
		current_line = 15;
	}
	
	memset(txt[current_line], '\0', 80);
	memcpy(txt[current_line++], str, strlen(str));
}

// --------------------------------------------------------------------------------- 
// This renders all the objects that have been added to the rendering unit
// --------------------------------------------------------------------------------- 
void CRend::render_objects()
{
	for(CObjectListIterator itor = objects.begin(); itor != objects.end(); itor++) {
		render_object(*itor);		
	}
}

// --------------------------------------------------------------------------------- 
// remove an object from the system
// --------------------------------------------------------------------------------- 
void CRend::remove_object(CObject *obj)
{
	for(CObjectListIterator itor = objects.begin();itor != objects.end(); itor++) {
		if(*itor == obj) { // check addresses
			objects.remove(*itor);
			num_objects--;
			break;
		}
	}
}

// --------------------------------------------------------------------------------- 
// remove a sprite from the system
// --------------------------------------------------------------------------------- 
void CRend::remove_sprite(char *sprite_name)
{
	for(CSpriteIterator itor = sprites.begin();itor != sprites.end(); itor++) {
		CSprite *spr = (*itor);
		if(strcmp(spr->texname, sprite_name)==0) {
			sprites.erase(itor);
			break;
		}
	}
}

// --------------------------------------------------------------------------------- 
// Calls the global defined function callback in order to inform another unit of 
// the various happensing of this unit
// --------------------------------------------------------------------------------- 
void CRend::do_callback(int callback_num)
{
	callback(callback_num);
	// update screen content as it could be changed by callback reciever
	// but viewer could not have changed
	update(NULL); 
}



// END OF FILE -------------------------------------------------------