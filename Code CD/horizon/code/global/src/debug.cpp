/*	--------------------------------------------------------------------------------- 
	Filename      - debug.cpp
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Implementation of the CDebug class
					provides a means to store any text to a file in a set way allowing
					easy debugging.
	--------------------------------------------------------------------------------- */

#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <debug.h>
#include <types.h>

// --------------------------------------------------------------------------------- 
// globally available access to debug class
// --------------------------------------------------------------------------------- 
CDebug *c_debug;

// --------------------------------------------------------------------------------- 
// constructor
// --------------------------------------------------------------------------------- 
CDebug::CDebug()
{
	init = 0;
}

// --------------------------------------------------------------------------------- 
// opens the file to store debug information to 
// --------------------------------------------------------------------------------- 
int CDebug::debug_init(char *filename)
{
	if(init) 
		return 1;

	fd = _open(filename, _O_CREAT | _O_WRONLY | _O_TRUNC, _S_IREAD | _S_IWRITE);

	if(fd < 0) 
		return -1;
  
	init = 1;

	return 1;
}

// --------------------------------------------------------------------------------- 
// invaluable print method allowing strings to be stored to file in a convient way
// --------------------------------------------------------------------------------- 
int CDebug::debug_print(char *str, ...)
{
	char str_buffer[DEBUG_MAX_BUFFER_LENGTH];
	va_list v_list;

	va_start(v_list, str);
	vsprintf(str_buffer, str, v_list);
	_write(fd, str_buffer, strlen(str_buffer));
	_commit(fd);
	va_end(v_list);

	return strlen(str_buffer);
}

// --------------------------------------------------------------------------------- 
// close the debug file
// --------------------------------------------------------------------------------- 
int CDebug::debug_close(void)
{
	close(fd);
	return 1;
}

// END OF FILE ----------------------------------------------------------------------


