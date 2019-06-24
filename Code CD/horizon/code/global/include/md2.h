/*	--------------------------------------------------------------------------------- 
	Filename      - md2.h
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Class defintion and helper structures used to load a .MD2 model
	--------------------------------------------------------------------------------- */

#ifndef _MD2_H_
#define _MD2_H_

#include <types.h>

// --------------------------------------------------------------------------------- 
// the complete .MD2 header
// --------------------------------------------------------------------------------- 
typedef struct 
{ 
   int magic; 
   int version; 
   int skinWidth; 
   int skinHeight; 
   int frameSize; 
   int numSkins; 
   int numVertices; 
   int numTexCoords; 
   int numTriangles; 
   int numGlCommands; 
   int numFrames; 
   int offsetSkins; 
   int offsetTexCoords; 
   int offsetTriangles; 
   int offsetFrames; 
   int offsetGlCommands; 
   int offsetEnd; 
} model_t;

// --------------------------------------------------------------------------------- 
// a vertex stored in the .MD2
// --------------------------------------------------------------------------------- 
typedef struct
{
   byte vertex[3];
   byte lightNormalIndex;
} triangleVertex_t;

// --------------------------------------------------------------------------------- 
// a key frame stored in the .MD2
// --------------------------------------------------------------------------------- 
typedef struct
{
   float scale[3];
   float translate[3];
   char name[16];
   triangleVertex_t vertices[2048];

   // Added for use by this project, not part of .MD2 spec
   CVert3D verts[2048];   
} frame_t;

// --------------------------------------------------------------------------------- 
// .MD2 contains a helper structure to speed up OpenGL rendering so going to use it
// --------------------------------------------------------------------------------- 
typedef struct
{
   float s, t;
   int vertexIndex;
} glCommandVertex_t;


// --------------------------------------------------------------------------------- 
// CMD2
// --------------------------------------------------------------------------------- 
class CMD2 
{
private:
	FILE *fp;
public:
	model_t              header;
	frame_t              frames[512];
	glCommandVertex_t    **command_verts;
	int                  *command_verts_type;
	char skinname[100];  // internal rep
	float cur_frame;     // current frame being rendered (can be a number between frames)
	                     // this is used for interpolation
	int max_frame;       // max animation frame to get to 
	int min_frame;       // start animation frame
	CMD2();
	~CMD2();
	int load_md2(char *path, char *filename);
	int find_frame_num(char *frame_name);
};


#endif

// END OF FILE ------------------------------------------------------------------------