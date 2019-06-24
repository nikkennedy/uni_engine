/*	--------------------------------------------------------------------------------- 
	Filename      - lightmap.h 
	Last Modified - 09/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Class definition of CLightMapTexture
					this is a basic lightmap class, it is just another textureformat
	--------------------------------------------------------------------------------- */

#ifndef _LIGHTMAP_H_
#define _LIGHTMAP_H_

#include <texture.h>

class CLightMapTexture : public CTexture
{
public:
	CLightMapTexture(char *filename, int width, int height);

	void process_vertex(CVert3D *v) {}
	void stop_texture() {};
	void bind_texture();
};

// need to create container of lightmaps
#include <vector>
using namespace std;

typedef vector<CLightMapTexture *> CLightMapTextureVector;
typedef CLightMapTextureVector::iterator CLightMapIterator;

#endif

// END OF FILE ---------------------------------------------------------------------