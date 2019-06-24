#ifndef _GLOBAL__H_
#define _GLOBAL__H_

#include <debug.h>
#include <render.h>
#include <game.h>

extern CDebug *c_debug;
//extern CRenderer *c_rend;
extern CGame *c_game; // defined in cgame.c

#define REND_UNIT c_rend->r_api

#endif