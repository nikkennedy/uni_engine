/*	--------------------------------------------------------------------------------- 
	Filename      - bot.cpp (new implementation)
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Implements the CBotState its subclasses and the CBot class.
	--------------------------------------------------------------------------------- */

#include <types.h>
#include <bot.h>
#include <rend.h>
#include <float.h>
#include <waypoint.h>

// --------------------------------------------------------------------------------- 
// constructor
// --------------------------------------------------------------------------------- 
CBot::CBot(char *md2filename, char *skinname)
{
	// --------------------------------------------------------------------------------- 
	// each bot needs to create representation of its rocket
	// --------------------------------------------------------------------------------- 
	rocket = new CBullet();		
	
	// --------------------------------------------------------------------------------- 
	// load the bots MD2 model
	// --------------------------------------------------------------------------------- 
	md2_model = new CMD2();
	md2_model->load_md2(md2filename, "tris.md2");
	strcpy(md2_model->skinname, skinname);

	// --------------------------------------------------------------------------------- 
	// load the weapon MD2 model
	// --------------------------------------------------------------------------------- 
	weapon = new CMD2();
	weapon->load_md2(md2filename, "w_rlauncher.md2");
	strcpy(weapon->skinname, "w_rlauncher.bmp"); // must have this texture

	// --------------------------------------------------------------------------------- 
	// set the bots CObject attributes
	// --------------------------------------------------------------------------------- 
	shape = NULL;
	height      = 3.5;
	step_amount = 5;
	step_unit   = 0.1f;
	width       = 5.0;
	gravity     = 3.0;
	mass        = 3.0;

	// --------------------------------------------------------------------------------- 
	// create an instance of all the possible state classes
	// --------------------------------------------------------------------------------- 
	CBotState *running   = new CBotRunning(this);
	CBotState *turning   = new CBotTurning(this);
	CBotState *standing  = new CBotStanding(this);
	CBotState *attacking = new CBotAttacking(this);
	CBotState *dead      = new CBotDead(this);
	CBotState *chasing   = new CBotChasing(this);

	// --------------------------------------------------------------------------------- 
	// add these instances to a single vector
	// --------------------------------------------------------------------------------- 
	botstates.push_back(turning);
	botstates.push_back(running);
	botstates.push_back(standing);
	botstates.push_back(attacking);
	botstates.push_back(dead);
	botstates.push_back(chasing);

	// --------------------------------------------------------------------------------- 
	// set the initial state as the RUNNING state
	// --------------------------------------------------------------------------------- 
	botstates[BOT_STATE_STANDING]->change_state();
}

// --------------------------------------------------------------------------------- 
// descructor
// --------------------------------------------------------------------------------- 
CBot::~CBot()
{
	delete botstates[BOT_STATE_RUNNING];
	delete botstates[BOT_STATE_TURNING];
	delete botstates[BOT_STATE_STANDING];
	delete botstates[BOT_STATE_ATTACK];
	delete botstates[BOT_STATE_DEAD];
	delete botstates[BOT_STATE_CHASING];

	delete weapon;
	delete (CObject*) rocket;
}

// --------------------------------------------------------------------------------- 
// animate the MD2 model by cycling though the frames set
// --------------------------------------------------------------------------------- 
void CBot::animate(float speed)
{
	md2_model->cur_frame += speed;
	if(md2_model->cur_frame >= md2_model->max_frame+1) 
		md2_model->cur_frame = md2_model->min_frame;

	weapon->cur_frame += speed;
	if(weapon->cur_frame >= weapon->max_frame+1) 
		weapon->cur_frame = weapon->min_frame;

}

// --------------------------------------------------------------------------------- 
// copy constructor
// --------------------------------------------------------------------------------- 
CBot::CBot(const CBot &b) : CObject()
{
	this->wpc = b.wpc;
}

// --------------------------------------------------------------------------------- 
// set the waypoint chain for the bot
// --------------------------------------------------------------------------------- 
void CBot::set_waypoint_chain(CWayPointChain wpc)
{
	this->wpc = wpc;
}

// --------------------------------------------------------------------------------- 
// return the current waypoint destination
// --------------------------------------------------------------------------------- 
CWayPoint *CBot::current_heading()
{
	return wpc.current_way_point();
}

// --------------------------------------------------------------------------------- 
// calculate the distance to the next waypoint
// --------------------------------------------------------------------------------- 
float CBot::dist_to_way_point(CWayPoint *wp)
{
	CVert3D wp_pos  = wp->get_pos();
	CVert3D bot_pos = location;
	// ignore height changes
	wp_pos.y = bot_pos.y = 0;
	CVert3D dist = wp_pos - bot_pos;
	return dist.length();
}

// --------------------------------------------------------------------------------- 
// create the vector to the next waypoint
// --------------------------------------------------------------------------------- 
CVert3D CBot::vector_to_waypoint(CWayPoint *wp)
{
	CVert3D bot_pos = location;
	CVert3D wp_pos = wp->get_pos();
	// ignore height differences
	wp_pos.y = bot_pos.y = 0;
	CVert3D normaled = wp_pos - bot_pos;
	normaled.normalise();
	return normaled;
}

// --------------------------------------------------------------------------------- 
// calculate the angle of rotation to the next waypoint
// --------------------------------------------------------------------------------- 
float CBot::rotate_to_next_waypoint(CWayPoint *wp)
{
	matrix1x4 unit;
	matrix4x4 rotate;
	matrix1x4 move;
	
	CWayPoint *cur_head = wp;

	CVert3D vector_to_point = vector_to_waypoint(cur_head);

	MATRIX1X4(unit, 0, 0, -1, 0);
	CVert3D orientation = get_orientation();
	rotation_matrix(0, (int)orientation.y, 0, rotate);
	matrix_mul1x4_4x4(unit, rotate, move);

	CVert3D current_dir = CVert3D(move[0], 0, move[2]);
	current_dir.normalise();

	CVert3D p1 = current_dir;
	CVert3D p2 = vector_to_point;

	// dot product
	float angle = p1 * p2;

	if(angle != 1) {
		float angley = (float) (acos(angle) / (float) M_PI) * 180.0f;

		if(!_isnan(angley)) {
			return angley;
		} 
	}	

	return 0.0f;
}

// --------------------------------------------------------------------------------- 
// is the point insight of the bot
// --------------------------------------------------------------------------------- 
int CBot::insight(CVert3D point, CRend *rend)
{	
	// --------------------------------------------------------------------------------- 
	// check distance first
	// --------------------------------------------------------------------------------- 
	float dist = (point - location).length();
	if(dist > 100) return 0;	

	// --------------------------------------------------------------------------------- 
	// now make sure the path of sight is a clear one
	// --------------------------------------------------------------------------------- 
	return path_to(point, rend);

/*	CObject test_hit;
	test_hit.start_position = location;
	test_hit.end_position   = point;
	test_hit.get_dir_vector();
	test_hit.width = 1;
	test_hit.height = 1;
	test_hit.step_amount = 1;
	test_hit.step_unit = 1;
	if(rend->move_object(&test_hit) != NO_HIT)
		return 0;
*/

/*	matrix1x4 unit;
	matrix4x4 rotate;
	matrix1x4 move;

	MATRIX1X4(unit, 0, 0, -1, 0);
	CVert3D orientation = get_orientation();
	if(orientation.y == 0) orientation.y = 1;
	rotation_matrix(0, (int)orientation.y, 0, rotate);
	matrix_mul1x4_4x4(unit, rotate, move);

	CVert3D current_dir = CVert3D(move[0], 0, move[2]);
	current_dir.normalise();
	CPlane p = CPlane(current_dir, location);
	
	if(p.classify(point) < 0) {
		return 0; // player is behind
	} else {
		return 1; // player is infront
	}	
*/
}

// --------------------------------------------------------------------------------- 
// is the line of sight from the bot to point free of polygons?
// --------------------------------------------------------------------------------- 
int CBot::path_to(CVert3D point, CRend *rend)
{
	CObject test_hit;
	test_hit.start_position = location;
	test_hit.end_position   = point;
	test_hit.get_dir_vector();
	test_hit.width = 1;
	test_hit.height = 1;
	test_hit.step_amount = 0;
	test_hit.step_unit = 1;
	
	if(rend->move_object(&test_hit) != NO_HIT)
		return 0;

	return 1;
}

// --------------------------------------------------------------------------------- 
// move the bots rocket along its path,if it hits something cause explosion
// --------------------------------------------------------------------------------- 
void CBot::move_rockets(CRend *rend)
{
	if(rocket->active == 1) {
		rocket->move(rend);
		if(rocket->hit_wall()) {
			rocket->explode(rend);
			rocket->active = 0;			
		} 
	}
}

// --------------------------------------------------------------------------------- 
// fire the rocket off in the direction of the player
// --------------------------------------------------------------------------------- 
void CBot::fire(CVert3D topoint)
{
	matrix1x4 unit;
	matrix4x4 rotate;
	matrix1x4 move;
	
	if(rocket->active == 0) {
		CVert3D ori = get_orientation();

		MATRIX1X4(unit, 0, 0, -1, 0);

		rotation_matrix((int)ori.x, (int)(ori.y+90)%360, (int)ori.z, rotate);
		matrix_mul1x4_4x4(unit, rotate, move);
		CVert3D r_current_dir = CVert3D(move[0], move[1], move[2]);
		r_current_dir.normalise();

		CPlane p2 = CPlane(r_current_dir, location);

		// find Y-Axis rotation for rocket to player
		if(p2.classify(topoint) <= 0) { 
			ori.y -= rotate_to_next_waypoint(&CWayPoint(topoint,0));
			if(ori.y <= 0) ori.y+=360;
		} else { // left		
			ori.y += rotate_to_next_waypoint(&CWayPoint(topoint,0));
			if(ori.y >= 360) ori.y-=360;
		}
		
		float xdist = topoint.x - location.x;
		float ydist = topoint.y - location.y;
		float zdist = topoint.z - location.z;

		CVert3D hypo = CVert3D(xdist,ydist, zdist);

		// Calculate X-Axis rotation
		float angle = ydist / hypo.length();
		float rad = asin(angle);
		rad = (float) (rad / M_PI) * 180.0f;

		// X-Axis 90 = UP, 0 = INFRONT
		ori.x += rad;

		if(ori.x <= 0) ori.x += 360;
		if(ori.x > 360) ori.x -= 360;

		rocket->set_details(location, ori);		
	}
}

// --------------------------------------------------------------------------------- 
// is point infront of the bot use the plane calculation
// --------------------------------------------------------------------------------- 
int CBot::infrontof(CVert3D point)
{
	matrix1x4 unit;
	matrix4x4 rotate;
	matrix1x4 move;

	MATRIX1X4(unit, 0, 0, -1, 0);
	rotation_matrix(0, (int)orientation.y, 0, rotate);
	matrix_mul1x4_4x4(unit, rotate, move);

	CVert3D current_dir = CVert3D(move[0], 0, move[2]);
	current_dir.normalise();
	CPlane p = CPlane(current_dir, this->location);
	
	if(p.classify(point) <= 0) {
		return 0; // behind
	} else {
		return 1; // infront
	}
}

// --------------------------------------------------------------------------------- 
// is the bot on the right of point, again use the plane calculation
// --------------------------------------------------------------------------------- 
int CBot::rightof(CVert3D point)
{
	matrix1x4 unit;
	matrix4x4 rotate;
	matrix1x4 move;

	MATRIX1X4(unit, 0, 0, -1, 0);
	float oriy = orientation.y;

	oriy += 90;
	if(oriy >= 360) oriy -= 359;

	rotation_matrix((int)0, (int)oriy, (int)0, rotate);
	matrix_mul1x4_4x4(unit, rotate, move);
	CVert3D r_current_dir = CVert3D(move[0], 0, move[2]);
	r_current_dir.normalise();

	CPlane p2 = CPlane(r_current_dir, location);

	if(p2.classify(point) < 0) {
		return 1; // right
	} 
	if(p2.classify(point) > 0)
	{
		return 0;
	}

	return -1;
}

// --------------------------------------------------------------------------------- 
// BOT STATES
// --------------------------------------------------------------------------------- 

// --------------------------------------------------------------------------------- 
// constuctor
// --------------------------------------------------------------------------------- 
CBotState::CBotState(CBot *bot)
{
	this->bot = bot;
}

// --------------------------------------------------------------------------------- 
// set up the MD2 model frames
// --------------------------------------------------------------------------------- 
void CBotState::set_model_frames()
{
	bot->weapon->min_frame = bot->weapon->cur_frame = bot->md2_model->min_frame = bot->md2_model->cur_frame = start;
	bot->weapon->max_frame = bot->md2_model->max_frame = end;
}

// --------------------------------------------------------------------------------- 
// store the players current position
// --------------------------------------------------------------------------------- 
void CBotState::report_affectors(CVert3D target_pos)
{
	this->target_pos = target_pos;
}

// --------------------------------------------------------------------------------- 
// overridden if needed
// --------------------------------------------------------------------------------- 
void CBotState::restore_state()
{
	// do nothing
}

// --------------------------------------------------------------------------------- 
// CBotRunning constructor
// --------------------------------------------------------------------------------- 
CBotRunning::CBotRunning(CBot *bot) : CBotState(bot)
{
	start  = bot->md2_model->find_frame_num("run1"); // RUN1
	end    = bot->md2_model->find_frame_num("run1") + 5;

	next_rand_move = -1;
}

// --------------------------------------------------------------------------------- 
// CBotRunning functions
// --------------------------------------------------------------------------------- 
void CBotRunning::do_state(CRend *rend)
{

	// if player is insight move to attack state
	if(bot->insight(target_pos, rend)) {
		CBotAttacking *state = (CBotAttacking*) bot->botstates[BOT_STATE_ATTACK];		
		state->change_state();
		return;
	}

	// animate
	bot->move_rockets(rend);
	bot->animate(0.1f);		

	CWayPoint *cur_head = bot->current_heading();

	// Check to see if way point is within sight (i.e. reachable)
	if(!bot->path_to(cur_head->get_pos(), rend)) {
		// there is no direct path to the next waypoint

		// after random count reaches zero rotate bot
		if(--next_rand_move <= 0) {
			next_rand_move = rand() / 100;
			if((next_rand_move % 2) == 0)
				bot->apply_rotation(0, 5,0);
			else
				bot->apply_rotation(0,-5,0);	
		}

		// move bot in current direction
		matrix1x4 unit;
		MATRIX1X4(unit, 0, 0, -1, 0);
		CVert3D start_pos = bot->start_position;				
		if(bot->move_object(unit, 0.2f, rend) == HIT_POLY) {
			// if a polygon is hit change heading
			if((rand() % 2) == 0)
				bot->apply_rotation(0, 5,0);
			else
				bot->apply_rotation(0,-5,0);
		}
	} else {
		// there is a direct path to the next waypoint head towards it
		float dist = bot->dist_to_way_point(cur_head);
		if(dist <= 10) {				
			// if waypoint has been reached get next waypoint and head towards that
			bot->wpc.next_way_point();
			cur_head = bot->current_heading();				
			// need to orientate bot towards the next waypoiny
			CBotTurning *newstate = (CBotTurning*) bot->botstates[BOT_STATE_TURNING];		
			newstate->change_state();
			return;
		}	

		// if waypoint is not inline with current direction alter heading
		if(bot->rotate_to_next_waypoint(cur_head) >= 2 || bot->rotate_to_next_waypoint(cur_head) <= -2) {
			CBotTurning *newstate = (CBotTurning*) bot->botstates[BOT_STATE_TURNING];		
			newstate->change_state();
			return;
		}

		// move the bot along its current path
		matrix1x4 unit;
		MATRIX1X4(unit, 0, 0, -1, 0);
		CVert3D start_pos = bot->start_position;		
		if(bot->move_object(unit, 0.2f, rend) == HIT_POLY) {
			if((rand() % 2) == 0)
				bot->apply_rotation(0, 5,0);
			else
				bot->apply_rotation(0,-5,0);
		}
	}
}

/*void CBotRunning::report_affectors(CVert3D target_pos)
{
	this->target_pos = target_pos;
}*/

// --------------------------------------------------------------------------------- 
// change into running state
// --------------------------------------------------------------------------------- 
void CBotRunning::change_state()
{
	// set running MD2 animation frames
	set_model_frames();
	// change bots current state to RUNNING
	bot->cur_state = this;
}

// --------------------------------------------------------------------------------- 
// CBotTurning - turning state
// --------------------------------------------------------------------------------- 
CBotTurning::CBotTurning(CBot *bot) : CBotState(bot)
{
	start  = bot->md2_model->find_frame_num("stand01") + 20;
	end    = bot->md2_model->find_frame_num("stand01") + 30;
}

// --------------------------------------------------------------------------------- 
// perform turning actions
// --------------------------------------------------------------------------------- 
void CBotTurning::do_state(CRend *rend)
{
	// animate
	bot->animate(0.08f);		
	bot->move_rockets(rend);

	// rotate bot to next waypoint
	if(bot->rightof(bot->current_heading()->get_pos()) == 1) {
		bot->apply_rotation(0,-1,0);
		angle_to_rotate--;
	} else {
		bot->apply_rotation(0,1,0);
		angle_to_rotate--;
	}

	if((angle_to_rotate <= 1 && angle_to_rotate >= -1) ) {
		// rotation complete move to standing state
		CBotStanding *state = (CBotStanding*) bot->botstates[BOT_STATE_STANDING];
		state->change_state();
		return;
	}

	// player insight so change to attack state
	if(bot->insight(target_pos, rend)) {
		CBotAttacking *state = (CBotAttacking*) bot->botstates[BOT_STATE_ATTACK];		
		state->change_state();
		return;
	}
}

// --------------------------------------------------------------------------------- 
// change into turning state
// --------------------------------------------------------------------------------- 
void CBotTurning::change_state()
{
	set_model_frames();
	bot->cur_state = this;
	CWayPoint *cur_head = bot->current_heading();
	angle_to_rotate = bot->rotate_to_next_waypoint(cur_head);
}

/*void CBotTurning::report_affectors(CVert3D target_pos)
{
	this->target_pos = target_pos;
}*/

// --------------------------------------------------------------------------------- 
// CBotStanding - standing state
// --------------------------------------------------------------------------------- 
CBotStanding::CBotStanding(CBot *bot) : CBotState(bot)
{
	start = bot->md2_model->find_frame_num("st") + 0;
	end   = bot->md2_model->find_frame_num("st") + 20;	
}	

// --------------------------------------------------------------------------------- 
// standing functions
// --------------------------------------------------------------------------------- 
void CBotStanding::do_state(CRend *rend)
{
/*
	// player insight so attack state
	if(bot->insight(target_pos, rend)) {
		CBotAttacking *state = (CBotAttacking*) bot->botstates[BOT_STATE_ATTACK];		
		state->change_state();
		return;
	}
*/	
	// animate
	bot->animate(0.08f);		
/*
	bot->move_rockets(rend);
	// stand state count over change to running state
	if(standcount-- <= 0) {
		CBotRunning *state = (CBotRunning*) bot->botstates[BOT_STATE_RUNNING];
		state->change_state();
		return;
	}
	*/
}

// --------------------------------------------------------------------------------- 
// change into standing state
// --------------------------------------------------------------------------------- 
void CBotStanding::change_state()
{
	set_model_frames();
	standcount = rand() / 100; // random stand count
	bot->cur_state = this;	
}

/*void CBotStanding::report_affectors(CVert3D target_pos)
{
	this->target_pos = target_pos;
}*/


// --------------------------------------------------------------------------------- 
// CBotAttacking - attack state
// --------------------------------------------------------------------------------- 
CBotAttacking::CBotAttacking(CBot *bot) : CBotState(bot)
{
	start = 47;
	end   = 54;
	firecount = 0;
}

// --------------------------------------------------------------------------------- 
// perform attacking functions
// --------------------------------------------------------------------------------- 
void CBotAttacking::do_state(CRend *rend)
{
	player_seen = 1;

	// animate
	bot->animate(0.08f);		

	// keep the bot orientate towards the player
	matrix1x4 unit;
	matrix4x4 rotate;
	matrix1x4 move;

	MATRIX1X4(unit, 0, 0, -1, 0);
	CVert3D orientation = bot->orientation;
	rotation_matrix((int)0, (int)orientation.y, 0, rotate);
	matrix_mul1x4_4x4(unit, rotate, move);
	CVert3D current_dir = CVert3D(move[0], 0, move[2]); // current directory
	current_dir.normalise();
	CVert3D dir_to_get_to = (target_pos - bot->location);
	dir_to_get_to.y = 0;
	dir_to_get_to.normalise();

	float angle = current_dir * dir_to_get_to;

	if(bot->location.x != target_pos.x && bot->location.z != target_pos.z) {
		if(angle != 1) {
			float angley = (float) (acos(angle) / (float) M_PI) * 180.0f;

			if(!_isnan(angley)) {
				if(bot->rightof(target_pos) == 1) {		// Target is on the right
					bot->orientation.y -= angley;
					if(bot->orientation.y < 0)
						bot->orientation.y += 360;
				} else {
					bot->orientation.y += angley;

					if(bot->orientation.y > 360)
						bot->orientation.y -= 360;
				}
			}
		}
	}

	// reduce counter till next rocket launch
	firecount--;

	// move rocket if already launched, otherwise see if time to launch another
	if(bot->rocket->active == 1) {
		bot->move_rockets(rend);
	} else if(firecount <= 0 ){	
		bot->fire(target_pos); 
		firecount = 50;
	}

	// if bot move out of site change back to previous state
	if(!bot->insight(target_pos, rend) && attack_count <= 0) {		
		player_seen = 0;
		saved_state->change_state();
		return;
	}

	// it count reaches 0 move to chasing state
	if(attack_count-- <= 0) {
		CBotChasing *state = (CBotChasing *)bot->botstates[BOT_STATE_CHASING];
		state->change_state();
		return;
	}	
}

// --------------------------------------------------------------------------------- 
// change to attack state
// --------------------------------------------------------------------------------- 
void CBotAttacking::change_state()
{
	if(bot->cur_state == this)
		return;
	
	set_model_frames();
	// save current state
	saved_state = bot->cur_state;
	bot->cur_state = this;	
	attack_count = rand() / 100;
}

// --------------------------------------------------------------------------------- 
// restore the attack state after chasing player, i.e. do not update saved state
// --------------------------------------------------------------------------------- 
void CBotAttacking::restore_state()
{
	set_model_frames();
	bot->cur_state = this;	
	attack_count = rand() / 100;
}

/*void CBotAttacking::report_affectors(CVert3D target_pos)
{
	this->target_pos = target_pos;
}*/

// --------------------------------------------------------------------------------- 
// CBotDead - dead state
// --------------------------------------------------------------------------------- 
CBotDead::CBotDead(CBot *bot) : CBotState(bot)
{
	start  = bot->md2_model->find_frame_num("death201");
	end    = bot->md2_model->find_frame_num("death201") + 5;
}

// --------------------------------------------------------------------------------- 
// dead functions
// --------------------------------------------------------------------------------- 
void CBotDead::do_state(CRend *rend)
{
	// finish off any launced rockets
	bot->move_rockets(rend);
	
	// animate till last frame then stop
	if(bot->md2_model->cur_frame <= bot->md2_model->max_frame) 
		bot->animate(0.08f);					

	// once bot is dead for deadcount cycles do not render anymore
	if(deadcount-- <= 0) {
		bot->rocket->deactivate();
		bot->deactivate();		
		return;
	}
}

// --------------------------------------------------------------------------------- 
// change into dead state
// --------------------------------------------------------------------------------- 
void CBotDead::change_state()
{
	if(bot->cur_state == this)
		return;

	set_model_frames();
	bot->cur_state = this;	
	bot->width = 1.0f; // change width as now bot is lying down.
	deadcount = 1000;
}

// --------------------------------------------------------------------------------- 
// CBotChasing - chasing state
// --------------------------------------------------------------------------------- 
CBotChasing::CBotChasing(CBot *bot) : CBotState(bot)
{
	start  = bot->md2_model->find_frame_num("run1");
	end    = bot->md2_model->find_frame_num("run1") + 5;
}

// --------------------------------------------------------------------------------- 
// chasing functions
// --------------------------------------------------------------------------------- 
void CBotChasing::do_state(CRend *rend)
{
	matrix1x4 unit;
	matrix4x4 rotate;
	matrix1x4 move;

	MATRIX1X4(unit, 0, 0, -1, 0);

	// animate
	bot->animate(0.1f);		
	bot->move_rockets(rend);

	// rotate bot towards player
	if(bot->location.x != target_pos.x && bot->location.z != target_pos.z) {

		CVert3D orientation = bot->orientation;
		rotation_matrix(0, (int)orientation.y, 0, rotate);
		matrix_mul1x4_4x4(unit, rotate, move);
		CVert3D current_dir = CVert3D(move[0], 0, move[2]); // current direction
		current_dir.normalise();
		CVert3D dir_to_get_to = (target_pos - bot->location );
		dir_to_get_to.y = 0;
		dir_to_get_to.normalise();

		float angle = current_dir * dir_to_get_to;
	
		if(angle != 1) {
			float angley = (float) (acos(angle) / (float) M_PI) * 180.0f;
			if(!_isnan(angley)) {
			
				// Target is on the right
				if(bot->rightof(target_pos) == 1) {
					bot->apply_rotation(0,-angley,0);	
				} else {
					bot->apply_rotation(0,angley,0);	
				}		
			}
		} 
	}
	
	// move towards the player
	CVert3D vector_to_point = bot->vector_to_waypoint(&CWayPoint(target_pos,0));
	bot->apply_force(0.21f, vector_to_point.x, 0, 0);
	rend->move_object(bot);	
	bot->apply_force(0.21f, 0, 0, vector_to_point.z);
	rend->move_object(bot);	

	// once chased enough return to attack state
	if(move_count-- <= 0) {
		CBotAttacking *state = (CBotAttacking *) bot->botstates[BOT_STATE_ATTACK];
		state->restore_state(); // do not update saved state
		return;
	}
}

// --------------------------------------------------------------------------------- 
// change to chasing state
// --------------------------------------------------------------------------------- 
void CBotChasing::change_state()
{
	set_model_frames();	

	bot->cur_state = this;	
	move_count = rand() / 100;
}

/*void CBotChasing::report_affectors(CVert3D target_pos)
{
	this->target_pos = target_pos;
}*/

// END OF FILE ----------------------------------------------------------------------