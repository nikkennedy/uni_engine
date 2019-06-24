/*	--------------------------------------------------------------------------------- 
	Filename      - parse.cpp
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Implementation of superclass of all file parsing classes used.
	--------------------------------------------------------------------------------- */

#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <debug.h>
#include <types.h>
#include <parse.h>

// --------------------------------------------------------------------------------- 
// constructor, defaults to non-verbose output 
// --------------------------------------------------------------------------------- 
CParse::CParse(void)
{
#ifdef DEBUG
	c_debug = new CDebug();
	c_debug->debug_init("CPARSE.DAT");
#endif
	verbose_output = 0;
}

// --------------------------------------------------------------------------------- 
// descructor, closes files
// --------------------------------------------------------------------------------- 
CParse::~CParse()
{
 close(fd);
 close(o_fd);
}

// --------------------------------------------------------------------------------- 
// constructor that opens a specific debug file, if debug has been enabled at
// compile time
// --------------------------------------------------------------------------------- 
CParse::CParse(char *filename)
{
#ifdef DEBUG
	c_debug = new CDebug();
	c_debug->debug_init(filename);
#endif
	verbose_output = 0;
}

// --------------------------------------------------------------------------------- 
// opens a file ready to parse
// --------------------------------------------------------------------------------- 
int CParse::open_file(char *filename)
{
	fd = open(filename, _O_RDONLY);
	if(fd < 0) {
		printf("Could not open file %s\n", filename);
		dbp("Could not open file %s\n", filename);
		return 0;
	}
	return 1;
}

// --------------------------------------------------------------------------------- 
// writes a string to file
// --------------------------------------------------------------------------------- 
int CParse::write_printf(char *str, ...)
{
	char str_buffer[2000];
	va_list v_list;

	va_start(v_list, str);
	vsprintf(str_buffer, str, v_list);
	_write(fd, str_buffer, strlen(str_buffer));
	_commit(fd);
	va_end(v_list);
	return strlen(str_buffer);
}

// --------------------------------------------------------------------------------- 
// returns to begining to file
// --------------------------------------------------------------------------------- 
void CParse::reset_file(void)
{
	lseek(fd, 0, SEEK_SET);
}

// --------------------------------------------------------------------------------- 
// store the current file pointer position
// --------------------------------------------------------------------------------- 
void CParse::save_point(void)
{
	savepoint = tell(fd);
}

// --------------------------------------------------------------------------------- 
// go to stored file pointer position
// --------------------------------------------------------------------------------- 
void CParse::restore_point(void)
{
	lseek(fd, savepoint, SEEK_SET);
}

// --------------------------------------------------------------------------------- 
// read the next line store in l
// --------------------------------------------------------------------------------- 
int CParse::read_line(char *l)
{
	int index = 0;
	int ch = 0;

	while(1) {
		memset(line, '\0', MAX_LINE_LENGTH);

		if(read(fd, &ch, 1) != 1) return -1;
		while(ch != '\n' && ch != '\r') {
			l[index++] = ch;
			if(read(fd, &ch, 1) != 1) return -1;
		}
		if(index > 0) break;
	}
	return index;
}

// --------------------------------------------------------------------------------- 
// grab the next line store in 'line' (variable of class)
// --------------------------------------------------------------------------------- 
int CParse::read_line(void)
{
	int index = 0;
	int ch = 0;

	while(1) {
		memset(line, '\0', MAX_LINE_LENGTH);

		if(read(fd, &ch, 1) != 1) return -1;
		while(ch != '\n' && ch != '\r') {
			line[index++] = (char) ch;
			if(read(fd, &ch, 1) != 1) return -1;
		}
		if(index > 0) break;
	}
	return index;
}

// --------------------------------------------------------------------------------- 
// compares the line stored with str, processing ignores spaces and tabs, returns
// 1 on match 0 otherwise
// --------------------------------------------------------------------------------- 
int CParse::cmp(char *str)
{
	char out[MAX_LINE_LENGTH];
	int count = 0, no_skip = 1;

	memset(out, '\0', MAX_LINE_LENGTH);
	for(int i=0;i<(int)strlen(line);i++) {
		if(line[i] != ' ' && line[i] != '\t')  {
			out[count++] = line[i];
			no_skip = 0;			
		} else if(!no_skip) {
			out[count++] = line[i];
		}
	}
	
	if(strncmp(str, out, strlen(str)) != 0) 
		return 0;
	else
		return 1;
}

// --------------------------------------------------------------------------------- 
// format of line: "A_STRING" "ANOTHER_STRING" -> this sets str to  "ANOTHER_STRING"
// and returns 1 
// --------------------------------------------------------------------------------- 
int CParse::get_string(char *str)
{
	int skip = 1;
	int count = 0;
	char entity_name[30];

	read_line(); // reads the next line

	for(int i=0;i<(int)strlen(line);i++) {
		if( (line[i] == ' ' || line[i] == '\t') && skip) continue;
		if( (line[i] == ' ' || line[i] == '\t') && !skip) break;
		if(line[i] != ' ') skip = 0;
		if(line[i] == '\"') continue;
		
		entity_name[count++] = line[i];
	}
	entity_name[count++] = '\0';
	count = 0;
	skip = 1;
	for(;i<(int)strlen(line);i++) {
		if( (line[i] == ' ' || line[i] == '\t') && skip) continue;
		if(line[i] == '\n' || line[i] == '\r') break;
		if(line[i] == '\"') continue;
		if(line[i] != ' ') skip =0;
		str[count++] = line[i];
	}
	str[count++] = '\0';
	return 1;
}

// --------------------------------------------------------------------------------- 
// same as previous function, only this one assumes line wanted is already stored
// --------------------------------------------------------------------------------- 
int CParse::read_string(char *str)
{
	int skip = 1;
	int count = 0;
	char entity_name[30];

	for(int i=0;i<(int)strlen(line);i++) {
		if( (line[i] == ' ' || line[i] == '\t') && skip) continue;
		if( (line[i] == ' ' || line[i] == '\t') && !skip) break;
		if(line[i] != ' ') skip = 0;
		if(line[i] == '\"') continue;
		
		entity_name[count++] = line[i];
	}
	entity_name[count++] = '\0';
	count = 0;
	skip = 1;
	for(;i<(int)strlen(line);i++) {
		if( (line[i] == ' ' || line[i] == '\t') && skip) continue;
		if(line[i] == '\n' || line[i] == '\r') break;
		if(line[i] == '\"') continue;
		if(line[i] != ' ') skip =0;
		str[count++] = line[i];
	}
	str[count++] = '\0';

	return 1;
}

// --------------------------------------------------------------------------------- 
// line format: "A_STRING" <int>, this returns the integer
// --------------------------------------------------------------------------------- 
int CParse::get_int(void)
{
	char entity_name[80];
	int val;
	read_line();
	sscanf(line, "%s %d", entity_name, &val);
	return val;
}

// --------------------------------------------------------------------------------- 
// line format: "A_STRING" <float>, this returns the float
// --------------------------------------------------------------------------------- 
float CParse::get_float(void)
{
	char entity_name[80];
	float val;
	read_line();
	sscanf(line, "%s %f", entity_name, &val);
	return val;
}

// --------------------------------------------------------------------------------- 
// reads the file until the string skip matches the one just read
// --------------------------------------------------------------------------------- 
int CParse::skip_until(char *skip)
{
	do { if(read_line()==-1) return -1; } while(!cmp(skip));
	return 1;
}

// --------------------------------------------------------------------------------- 
// closes the file being parsed
// --------------------------------------------------------------------------------- 
void CParse::close_file()
{
	close(fd);
}

// END OF FILE ---------------------------------------------------------------------