/*	--------------------------------------------------------------------------------- 
	Filename      - parse.h
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Superclass definition for all parsing classes
	--------------------------------------------------------------------------------- */

#ifndef _PARSE_H_
#define _PARSE_H_

// --------------------------------------------------------------------------------- 
// limit on maximum length of line read
// --------------------------------------------------------------------------------- 
#define MAX_LINE_LENGTH  2000

// --------------------------------------------------------------------------------- 
// CParse
// --------------------------------------------------------------------------------- 
class CParse {

public:
	int fd; // file handle
	int o_fd; // output file handle
	char line[MAX_LINE_LENGTH]; // buffer for holding string read each line
	long savepoint;
	int verbose_output;
public:

public:
	int open_file(char *filename);
	void close_file();

	int get_string(char *str); // returns next string from just read line
	int read_string(char *str); // no read line reads from line to str

	int get_int(); // returns an integer entity
	float get_float();

public:
	CParse();
	CParse(char *filename);
	~CParse();
	void reset_file(void);
	void save_point();
	void restore_point();
	// --------------------------------------------------------------------------------- 
	// readline into argument
	// --------------------------------------------------------------------------------- 
	int read_line(char *);

	// --------------------------------------------------------------------------------- 
	// read new line int line member
	// --------------------------------------------------------------------------------- 
	int read_line(void);

	// --------------------------------------------------------------------------------- 
	// compares what was read with str (length of str)
	// --------------------------------------------------------------------------------- 
	int cmp(char *str); 

	// --------------------------------------------------------------------------------- 
	// keeps reading until it finds skip
	// --------------------------------------------------------------------------------- 
	int skip_until(char *skip); 

	int write_printf(char *str, ...);

};

#endif

// END OF FILE --------------------------------------------------------