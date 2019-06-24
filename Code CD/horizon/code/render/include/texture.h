/*	--------------------------------------------------------------------------------- 
	Filename      - texture.h 
	Last Modified - 09/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Class definition of CTexture
					this is the super class of all texture formats used in the system
					it is render independent, so no specific API is required.
	--------------------------------------------------------------------------------- */

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <types.h>
#include <image.h>

#define MAX_TEXTURE_NAME_SIZE 255

class CTexture {

protected:
	u_char *data; // texture data, in various formats
	char   name[MAX_TEXTURE_NAME_SIZE]; // texture identification name
	u_int  id; // texture ID
	
	// texture dimensions
	int width;   
	int height;  
    
	CBitMap *cur_texture; // the current texture image being processed

public:
	// constructor
	CTexture(char *filename, char *texture_name); // load a texture

	// lightmaps need this constructor
	CTexture() { }

	// free memory used by image
	~CTexture() {  if(data != NULL) free(data); }

	virtual void bind_texture() = 0;
	
	// get-er method for the textures name
	char *get_name() 
	{ 
		return name; 
	}

	// get-er method for the texture id
	u_int get_id() 
	{
		return id;
	}	

	virtual void process_vertex(CVert3D *v) = 0; 
};

// provide a container for textures
#include <vector>
using namespace std;

typedef vector<CTexture *> CTextureVector;
typedef CTextureVector::iterator CTextureIterator;

#endif

// END OF FILE ---------------------------------------------------------------------