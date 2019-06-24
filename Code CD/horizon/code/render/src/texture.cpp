/*	--------------------------------------------------------------------------------- 
	Filename      - texture.h 
	Last Modified - 09/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Class implementation of CTexture
					this is the super class of all texture formats used in the system
					it is render independent, so no specific API is required.
	--------------------------------------------------------------------------------- */

#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <texture.h>
#include <image.h>
#include <types.h>
#include <debug.h>
#include <hrz_config.h>

// --------------------------------------------------------------------------------- 
// constuctor, this opens the file (filename) and reads the bitmap data ready for
// each individual texture type to process it as it seems fit.
// --------------------------------------------------------------------------------- 
CTexture::CTexture(char *filename, char *texture_name)
{
	char    correctfilename[200];
	int     finished = 0;
	int     index = 0;

	// clear filename holder
	memset(correctfilename, '\0', 200);	

	// filename begins with ':' then it is an absolute path, otherwise its relative 
	// to the DATA_PATH defined in the systems configuration file

	if(filename[0] != ':') { // if filename starts with ':' its an absolute path!

		// strip off everything but the filename (so could have path details put they
		// will be removed, also skip spaces, tabs and whitespace
		for(int j=0;j<200;j++) {
			if(filename[j] == '\"' && finished == 1) break;
			if(filename[j] != '\"' && filename[j] != ' ' && filename[j] != '\t') {
				correctfilename[index++] = filename[j];
				finished = 1;
			}
		}

		int filename_length = strlen(filename);

		for(int i=filename_length-1;i>=0;i--) {
			if(filename[i] == '\\')
				break;
		}
	
		char texture_filename[255];

		for(int k=i+1,count=0;k<filename_length;k++) {
			texture_filename[count++] = filename[k];
		}
		texture_filename[count] = '\0';

		// correctfilename = DATA_PATH + \\textures\\ + texture_filename
		strcpy(correctfilename, CHrzConfig::data_path);
		strcat(correctfilename, "\\textures\\");
		strcat(correctfilename, texture_filename);
	} else { 
		strcpy(correctfilename, filename+1); // absolute path
	}

	// create a new bitmap and read in the image data
	cur_texture = new CBitMap();
	if(!cur_texture->read_image(correctfilename)) {
		cprt("can not read texture called %s\n", correctfilename);
		return;
	}
}


// END OF FILE ---------------------------------------------------------------------