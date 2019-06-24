/*	--------------------------------------------------------------------------------- 
	Filename      - sprite.h
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Definition of CSprite, which holds infomation regarding the 
	                static images that the renderer can display
	--------------------------------------------------------------------------------- */

#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <types.h>

// --------------------------------------------------------------------------------- 
// CSprite
// --------------------------------------------------------------------------------- 
class CSprite
{
public:
	CVert3D pos; // position to draw sprite (bottom-left corner)
	int width;   // width of image
	int height;  // height of image
	char texname[255]; // texturename
	char tex_filename[255]; // filename for bitmap
	CVert3D tex_invis; // colours with r,g,b values less than x,y,z are see thru
	int texture_id; // texture id
	int render; // display the sprite if 1, otherwise do not

public:
	CSprite(char *tex_name, char *tex_filename, CVert3D tex_invis, CVert3D pos, int width, int height);
	~CSprite() {};
	
};

// --------------------------------------------------------------------------------- 
// provide a vector of pointers to sprites and a corresponding iterator
// --------------------------------------------------------------------------------- 
#include <vector>
using namespace std;
typedef vector<CSprite*> CSpriteVector;
typedef CSpriteVector::iterator CSpriteIterator;

#endif

// END OF FILE ----------------------------------------------------------------------