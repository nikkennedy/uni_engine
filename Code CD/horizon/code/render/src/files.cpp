#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>

#include <debug.h>

// Reads in a text file and returns the whole file as a long string
// This is so i can load vertex programs from file.
char *read_file(char *filename)
{
	int fd, length, bytes;
	char *str_file;

	fd = open(filename, O_RDONLY);

	if(fd < 0) {
		dbp("read_file(): could not open %s\n", filename);
		return NULL;
	}
	lseek(fd, 0, SEEK_END);
	length = tell(fd);

	dbp("Reading file of length: %d\n", length);

	str_file = new char[length+1];

	lseek(fd, 0L, SEEK_SET);
	bytes = read(fd, str_file, length);

	str_file[bytes] = 0;

//	str[length-1] = '\0';
//	dbp("FILE is :%s:\n", str_file);

	close(fd);
	return str_file;	
}
