/*	--------------------------------------------------------------------------------- 
	Filename      - console.cpp
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Code to add messages to and display the drop down console
	--------------------------------------------------------------------------------- */

#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <ogl.h>
#include <render_api.h>
#include <debug.h>
#include <texture.h>
#include <types.h>
#include <maths.h>
#include <ogl_texturemanager.h>
#include <console.h>

// allocate the static variables
int  CConsole::console_fd = 0;
int  CConsole::current_console_input_pos;
char CConsole::console_input[MAX_CONSOLE_LINE_LENGTH];
char CConsole::console_txt[CONSOLE_LINES][MAX_CONSOLE_LINE_LENGTH];
int  CConsole::number_console_lines = 0;
int  CConsole::scroll_pos = 0;

// --------------------------------------------------------------------------------- 
// initialise the console, open the console file, which holds all the messages
// --------------------------------------------------------------------------------- 
void CConsole::console_init()
{
	current_console_input_pos = 0;
	memset(console_input, 0, MAX_CONSOLE_LINE_LENGTH);
	console_fd = open("console.dat", O_APPEND | O_RDWR | O_TRUNC);
	if(console_fd <= 0)
		console_fd = 0;
}

// --------------------------------------------------------------------------------- 
// read the contents of the console file, only stores up to maximum of CONSOLE_LINES
// lines of text (last line will contain the last line of the file if this occurs)
// --------------------------------------------------------------------------------- 
void CConsole::console_read_file()
{
	// make sure console is valid
	if(console_fd == 0) return;

	for(int i=0;i<CONSOLE_LINES;i++)
		memset(console_txt[i], 0, MAX_CONSOLE_LINE_LENGTH);

	memset(console_input, 0, MAX_CONSOLE_LINE_LENGTH);
	current_console_input_pos = 0;
		
	lseek(console_fd, 0, SEEK_SET);
		
	char ch         = ' ';
	int  cur_lineno = 0;
	int  lineindex  = 0;

	while(1) {
		int check = read(console_fd, &ch, 1);
		if(check != 1) break;			
		if(ch != '\n' && ch != '\0' && ch != '\r' && ch != '\t') 
			console_txt[cur_lineno][lineindex++] = ch;
		if(ch == '\n') {
			console_txt[cur_lineno][lineindex] = '\0';
			cur_lineno++;
			lineindex = 0;
			if(cur_lineno >= CONSOLE_LINES) cur_lineno = CONSOLE_LINES;
		}
	}
	number_console_lines = cur_lineno;
	scroll_pos = 0;
}

// ------------------------------------------------------------------------------------
// add a message to the console, basically write the message to file
// ------------------------------------------------------------------------------------
void CConsole::console_add_msg(char *str, ...)
{
	if(console_fd == 0) return;
	
	char str_buffer[200];
	va_list v_list;

	memset(str_buffer, 0, 200);
	va_start(v_list, str);
	vsprintf(str_buffer, str, v_list);
	memcpy(console_txt[number_console_lines++], str_buffer, strlen(str_buffer));
	va_end(v_list);
	lseek(console_fd, 0, SEEK_END);
	write(console_fd, str_buffer, strlen(str_buffer));

	current_console_input_pos = 0;
	memset(console_input, 0, MAX_CONSOLE_LINE_LENGTH);
}

// ------------------------------------------------------------------------------------
// render the console using opengl and the console font
// ------------------------------------------------------------------------------------ 
void COpenGLRend::draw_console(void)
{

	glPushMatrix();

		glMatrixMode( GL_PROJECTION ); 
		glLoadIdentity();
		glOrtho(0,screen_width,0,screen_height,1,-1);
		glMatrixMode( GL_MODELVIEW ); 
		glLoadIdentity();

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_EQUAL, 1);
		glDisable(GL_TEXTURE_2D);
	
		for(int i=0;i<current_line;i++) {
			((COGLFont*) console_font)->draw_string((char*)txt[i], 0, ((float)i * 8));
		}

		glDisable(GL_ALPHA_TEST);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);

	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, screen_width/screen_height, 1, 3000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// END OF FILE -----------------------------------------------------------------------------------