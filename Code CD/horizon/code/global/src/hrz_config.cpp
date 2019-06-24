/*	--------------------------------------------------------------------------------- 
	Filename      - hrz_config.cpp
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Code to load and store configuration options
	--------------------------------------------------------------------------------- */

#include <io.h>
#include <fcntl.h>
#include <windows.h>
#include <stdio.h>
#include <gl/gl.h>
#include <hrz_config.h>

// allocate static variables
int CHrzConfig::clipping = 0;
int CHrzConfig::debug    = 0;
int CHrzConfig::gravity  = 0;
float CHrzConfig::gravity_strength  = 0;
float CHrzConfig::move_amount_forward  = 0;
float CHrzConfig::move_amount_backward  = 0;
int CHrzConfig::invert_mouse = 0;
int CHrzConfig::full_screen = 0;
char CHrzConfig::console_tex0_filename[255];
char CHrzConfig::console_tex1_filename[255];
char CHrzConfig::data_path[255];
int  CHrzConfig::filltype = GL_FILL;
char CHrzConfig::levels[10][255];
int CHrzConfig::level_count = 0;

// --------------------------------------------------------------------------------- 
// translate ON/OFF to 1/0
// --------------------------------------------------------------------------------- 
int CHrzConfig::which_value(char *value)
{
	int val;

	if(strncmp(value, "ON", 2) == 0)
		return 1;
	if(strncmp(value, "OFF", 3) == 0)
		return 0;

	sscanf(value, "%d", &val);
	return val;
}

// --------------------------------------------------------------------------------- 
// set the various options available
// --------------------------------------------------------------------------------- 
void CHrzConfig::set_option(char *line, CParse *parser)
{
	char option[80];
	char value[20];
	char equals[20];

	sscanf(line, "%s %s %s", option, equals, value);

	if(strncmp(option, "DEBUG", 5) == 0) {
		debug = which_value(value);	
	}

	if(strncmp(option, "CLIPPING", 8) == 0) {
		clipping = which_value(value);		
	}

	if(strncmp(option, "GRAVITY_ON", 10) == 0) {
		gravity = which_value(value);		
	}

	if(strncmp(option, "GRAVITY_STRENGTH", 16) == 0) {
		gravity_strength = (float)which_value(value) / 10.0f;
	}

	if(strncmp(option, "MOVE_AMOUNT_FORWARD", 19) == 0) {
		move_amount_forward = (float)which_value(value) / 10.0f;
	}

	if(strncmp(option, "MOVE_AMOUNT_BACKWARD", 20) == 0) {
		move_amount_backward = (float)which_value(value) /10.0f;
	}

	if(strncmp(option, "INVERT_MOUSE", 12) == 0) {
		invert_mouse = which_value(value);
	}

	if(strncmp(option, "ADD_LEVEL", 11) == 0) {
		parser->read_string(levels[level_count++]);
	}

	if(strncmp(option, "CONSOLE_TEX0", 12) == 0) {
		parser->read_string(console_tex0_filename);
	}

	if(strncmp(option, "CONSOLE_TEX1", 12) == 0) {
		parser->read_string(console_tex1_filename);
	}

	if(strncmp(option, "DATAPATH", 8) == 0) {
		parser->read_string(data_path);
	}

	if(strncmp(option, "FULL_SCREEN", 11) == 0) {
		full_screen = which_value(value);
	}

}

// --------------------------------------------------------------------------------- 
// read each line of the configuration file, ignore lines begining with #
// use the standard parsing class. Each line is then check to see what option it
// represents with a call to set_option(...)
// --------------------------------------------------------------------------------- 
void CHrzConfig::read_config_file(char *filename)
{
	CParse *parser = new CParse();
	if(parser->open_file(filename) == 0) {
		MessageBox(NULL, "Unable to read configuration file.", "Error", MB_OK); 
		exit(1);
	}

	
	parser->read_line();

	int counter = 0;
	while(!parser->cmp("#END")) {

		if(!parser->cmp("#")) {
			set_option(parser->line, parser);
		}
		parser->read_line();
		counter++;
		if(counter >1000) break;
	}
	delete parser;
}


// END OF FILE -----------------------------------------------------------------------