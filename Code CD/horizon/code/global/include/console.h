/*	--------------------------------------------------------------------------------- 
	Filename      - console.h
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Class definition providing functions that add messages to and 
	                display the drop down console
	--------------------------------------------------------------------------------- */

#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#define CONSOLE_LINES           2000
#define MAX_CONSOLE_LINE_LENGTH 150

// a useful definition that simplyfies code
#define cprt CConsole::console_add_msg

class CConsole 
{
public:
	static int console_fd;
	static int current_console_input_pos;
	static char console_input[MAX_CONSOLE_LINE_LENGTH];
	static char console_txt[CONSOLE_LINES][MAX_CONSOLE_LINE_LENGTH];
	static int  number_console_lines;
	static int  scroll_pos;

	static void console_read_file();
	static void console_init();
	static void console_add_msg(char *str, ...);
};

#endif

// END OF FILE ------------------------------------------------------------------------