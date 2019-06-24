/*	--------------------------------------------------------------------------------- 
	Filename      - bot.h 
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Defines the CBotState its subclasses and the CBot class.
	--------------------------------------------------------------------------------- */

#ifndef _BOT_H_
#define _BOT_H_

#include <object.h>
#include <types.h>
#include <rend.h>
#include <md2.h>
#include <bullet.h>
#include <waypoint.h>

// --------------------------------------------------------------------------------- 
// useful defines
// --------------------------------------------------------------------------------- 
#define BOT_STATE_TURNING  0
#define BOT_STATE_RUNNING  1
#define BOT_STATE_STANDING 2
#define BOT_STATE_ATTACK   3
#define BOT_STATE_DEAD     4
#define BOT_STATE_CHASING  5	

// --------------------------------------------------------------------------------- 
// forward referencing
// --------------------------------------------------------------------------------- 
class CBotState;

// --------------------------------------------------------------------------------- 
// need to be able to hold a list of states
// --------------------------------------------------------------------------------- 
typedef vector<CBotState*> CBotStateVector;
typedef CBotStateVector::iterator CBotStateIterator;

// --------------------------------------------------------------------------------- 
// CBot
// --------------------------------------------------------------------------------- 
class CBot : public CObject {

public:
	// --------------------------------------------------------------------------------- 
	// holds a copy of each state a bot can be in
	// --------------------------------------------------------------------------------- 
	CBotStateVector botstates;

	// --------------------------------------------------------------------------------- 
	// the bots waypoint chain
	// --------------------------------------------------------------------------------- 
	CWayPointChain wpc;

	// --------------------------------------------------------------------------------- 
	// an MD2 model used to represent a bots weapon
	// --------------------------------------------------------------------------------- 
	CMD2 *weapon;	

	// --------------------------------------------------------------------------------- 
	// the bots current state
	// --------------------------------------------------------------------------------- 
	CBotState *cur_state;

	// --------------------------------------------------------------------------------- 
	// the bots rocket
	// --------------------------------------------------------------------------------- 
	CBullet *rocket;

	// --------------------------------------------------------------------------------- 
	// constructors
	// --------------------------------------------------------------------------------- 
	CBot(char *md2filename, char *skinname);
	CBot(const CBot &b);
	~CBot();

	// --------------------------------------------------------------------------------- 
	// set the waypoint chain
	// --------------------------------------------------------------------------------- 
	void set_waypoint_chain(CWayPointChain wpc);

	
	// --------------------------------------------------------------------------------- 
	// copy operator
	// --------------------------------------------------------------------------------- 
	inline CBot operator=(const CBot bot) 
	{
		wpc = bot.wpc;
		return *this;
	}

	// --------------------------------------------------------------------------------- 
	// what is the bots current destination waypoint
	// --------------------------------------------------------------------------------- 
	CWayPoint *current_heading();
	
	// --------------------------------------------------------------------------------- 
	// distance to the waypoint
	// --------------------------------------------------------------------------------- 	
	float dist_to_way_point(CWayPoint *wp);

	// --------------------------------------------------------------------------------- 
	// calculate vector to the waypoint
	// --------------------------------------------------------------------------------- 
	CVert3D vector_to_waypoint(CWayPoint *wp);

	// --------------------------------------------------------------------------------- 
	// rotation angle to next waypoint
	// --------------------------------------------------------------------------------- 
	float rotate_to_next_waypoint(CWayPoint *wp);

	// --------------------------------------------------------------------------------- 
	// is point insight from the bot
	// --------------------------------------------------------------------------------- 
	int insight(CVert3D point,CRend *rend);

	// --------------------------------------------------------------------------------- 
	// is there a collision free path to point
	// --------------------------------------------------------------------------------- 
	int path_to(CVert3D point, CRend *rend);

	// --------------------------------------------------------------------------------- 
	// is point infront of the bot
	// --------------------------------------------------------------------------------- 
	int infrontof(CVert3D point);
	
	// --------------------------------------------------------------------------------- 
	// is point on the right of the bot
	// --------------------------------------------------------------------------------- 
	int rightof(CVert3D point);

	// --------------------------------------------------------------------------------- 
	// fire the rocket at location point
	// --------------------------------------------------------------------------------- 
	void fire(CVert3D topoint);

	// --------------------------------------------------------------------------------- 
	// animate the movement of the rocket and check to see if it hits the player
	// --------------------------------------------------------------------------------- 
	void move_rockets(CRend *rend);

	// --------------------------------------------------------------------------------- 
	// interpolate between the MD2 animation frames.
	// --------------------------------------------------------------------------------- 
	void animate(float speed);

};

// --------------------------------------------------------------------------------- 
// CBotState
// --------------------------------------------------------------------------------- 
class CBotState
{
public:
	// --------------------------------------------------------------------------------- 
	// frame numbers for MD2 model
	// --------------------------------------------------------------------------------- 
	int start;
	int end;

	// --------------------------------------------------------------------------------- 
	// position of the player
	// --------------------------------------------------------------------------------- 
	CVert3D target_pos;

	// --------------------------------------------------------------------------------- 
	// the bot instance that owns this class
	// --------------------------------------------------------------------------------- 
	CBot *bot;

	// --------------------------------------------------------------------------------- 
	// the previous state
	// --------------------------------------------------------------------------------- 
	CBotState *saved_state;

	// --------------------------------------------------------------------------------- 
	// constructor
	// --------------------------------------------------------------------------------- 
	CBotState(CBot *bot);

	// --------------------------------------------------------------------------------- 
	// perform the states actions - must be overridden by sub class
	// --------------------------------------------------------------------------------- 
	virtual void do_state(CRend *rend) = 0;

	// --------------------------------------------------------------------------------- 
	// perform these actions when this state is to be changed into
	// --------------------------------------------------------------------------------- 
	virtual void change_state() = 0;

	// --------------------------------------------------------------------------------- 
	// setup MD2 animation frames
	// --------------------------------------------------------------------------------- 
	void set_model_frames();

	// --------------------------------------------------------------------------------- 
	// report the position of the player to the bots state
	// --------------------------------------------------------------------------------- 
	void report_affectors(CVert3D target_pos);

	// --------------------------------------------------------------------------------- 
	// restore the state without changing the current saved state
	// --------------------------------------------------------------------------------- 
	virtual void restore_state();
};


// --------------------------------------------------------------------------------- 
// CBotStanding
// --------------------------------------------------------------------------------- 
class CBotStanding : public CBotState
{
public:
	// --------------------------------------------------------------------------------- 
	// current random stand count
	// --------------------------------------------------------------------------------- 
	int standcount;

	CBotStanding(CBot *bot);
	
	void do_state(CRend *rend);
	void change_state();
};

// --------------------------------------------------------------------------------- 
// CBotRunning
// --------------------------------------------------------------------------------- 
class CBotRunning : public CBotState
{
public:
	CVert3D direction;
	int next_rand_move;

	CBotRunning(CBot *bot);

	void do_state(CRend *rend);
	void change_state();
};

// --------------------------------------------------------------------------------- 
// CBotTurning
// --------------------------------------------------------------------------------- 
class CBotTurning : public CBotState
{
public:
	float angle_to_rotate;

	CBotTurning(CBot *bot);
	void do_state(CRend *rend);
	void change_state();
};

// --------------------------------------------------------------------------------- 
// CBotDead
// --------------------------------------------------------------------------------- 
class CBotDead : public CBotState
{
public:
	int deadcount;

	CBotDead(CBot *bot);
	void do_state(CRend *rend);
	void change_state();
};

// --------------------------------------------------------------------------------- 
// CBotAttacking
// --------------------------------------------------------------------------------- 
class CBotAttacking : public CBotState
{
public:
	int attack_count;
	int player_seen;
	int firecount;

	CVert3D saved_orientation;

	CBotAttacking(CBot *bot); 
	void do_state(CRend *rend);
	void change_state();
	void restore_state();;

};

// --------------------------------------------------------------------------------- 
// CBotChasing
// --------------------------------------------------------------------------------- 
class CBotChasing : public CBotState
{
public:
	int move_count;

	CBotChasing(CBot *bot);
	void do_state(CRend *rend);
	void change_state();
};

#endif

// END OF FILE ---------------------------------------------------------------------