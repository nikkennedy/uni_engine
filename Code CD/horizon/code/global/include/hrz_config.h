/*	--------------------------------------------------------------------------------- 
	Filename      - hrz_config.h
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Class to load and store configuration options, There are no 
		            accessor methods defined, this is purely to simplyfy access and
					keep this class simple.
	--------------------------------------------------------------------------------- */

#ifndef _HRZ_CONFIG_H_
#define _HRZ_CONFIG_H_

#include <parse.h>

#define CONFIG_CLIPPING_ON     1
#define CONFIG_CLIPPING_OFF    0

class CHrzConfig
{
private:
	static void set_option(char *line, CParse *parser);
	static int  which_value(char *value);

public:
	static int clipping; // is clipping enabled
	static int debug;    // is debug console displayed
	static int gravity;  // is gravity enabled
	static float gravity_strength; 
	static float move_amount_forward;
	static float move_amount_backward;
	static int invert_mouse;
	static int full_screen; // full screen display
	static char console_tex0_filename[255];
	static char console_tex1_filename[255];
	static char data_path[255]; // path to all data
	static int filltype;
	static char levels[10][255]; // filenames of all levels
	static int level_count;      // number of levels

public:
	CHrzConfig() {};
	static void read_config_file(char *filename);
};
#endif

// END OF FILE ----------------------------------------------------------------------------