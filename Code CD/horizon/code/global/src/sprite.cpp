/*	--------------------------------------------------------------------------------- 
	Filename      - sprite.cpp
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Constructor of CSprite, which holds infomation regarding the 
	                static images that the renderer can display
	--------------------------------------------------------------------------------- */

#include <sprite.h>

// --------------------------------------------------------------------------------- 
// constructor: a texture name, corresponding filename, invisibility colour, loading
// position, width and heigh of sprite texture. These are required to load, and
// display the sprite.
// --------------------------------------------------------------------------------- 
CSprite::CSprite(char *tex_name, char *tex_filename, CVert3D tex_invis, CVert3D pos, int width, int height)
{
	strcpy(this->texname, tex_name);
	strcpy(this->tex_filename, tex_filename);
	this->tex_invis = tex_invis;
	this->pos = pos;
	this->width = width;
	this->height = height;
	render = 1;
}

// END OF FILE ----------------------------------------------------------------------