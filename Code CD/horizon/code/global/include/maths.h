/*	--------------------------------------------------------------------------------- 
	Filename      - maths.h
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Important globally available maths functions
	--------------------------------------------------------------------------------- */

#ifndef _MATHS_H_
#define _MATHS_H_

#include <math.h>
#include <types.h>

// --------------------------------------------------------------------------------- 
// PI
// --------------------------------------------------------------------------------- 
#define M_PI 3.14159

// --------------------------------------------------------------------------------- 
// allow access from outside code
// --------------------------------------------------------------------------------- 
extern float cos_lookup[370];
extern float sin_lookup[370];

// --------------------------------------------------------------------------------- 
// glbally available functions
// --------------------------------------------------------------------------------- 
void create_lookups(void);
void rotation_matrix(int x, int y, int z, matrix4x4 r);
void matrix_mul1x4_4x4(matrix1x4 a, matrix4x4 b, matrix1x4 r);
void matrix_transpose(matrix4x4 in, matrix4x4 out);
int  match(float a, float b, float epsilon);

#endif

// END OF FILE ----------------------------------------------------------------------