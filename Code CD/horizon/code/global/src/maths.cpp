/*	--------------------------------------------------------------------------------- 
	Filename      - maths.cpp
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Important globally available maths functions
	--------------------------------------------------------------------------------- */

#include <windows.h>
#include <math.h>

#include <maths.h>
#include <types.h>
#include <debug.h>

// --------------------------------------------------------------------------------- 
// globally available cosine and sine lookup tables
// --------------------------------------------------------------------------------- 
float cos_lookup[370];
float sin_lookup[370];

// --------------------------------------------------------------------------------- 
// calulate the lookup tables
// --------------------------------------------------------------------------------- 
void create_lookups(void) 
{
	float rad;
	for(int i=0;i<370;i++) {
		rad = (float) (M_PI * (float)i/(float)180);
		cos_lookup[i] = (float)cos((double)rad);
		sin_lookup[i] = (float)sin((double)rad);
	}

	sin_lookup[360] = 0;
}

// --------------------------------------------------------------------------------- 
// set a 4x4 matrix to the identity matrix
// --------------------------------------------------------------------------------- 
void identity_matrix(matrix4x4 m)
{
	m[0][1] = m[0][2] = m[0][3] = 0;
	m[1][0] = m[1][2] = m[1][3] = 0;
	m[2][0] = m[2][1] = m[2][3] = 0;
	m[3][0] = m[3][1] = m[3][2] = 0;
	m[0][0] = m[1][1] = m[2][2] = m[3][3]  = 1;
}

// --------------------------------------------------------------------------------- 
// multiply two 4x4 matrices
// --------------------------------------------------------------------------------- 
void matrix_mul4x4_4x4(matrix4x4 a, matrix4x4 b, matrix4x4 r)
{
	float sum;
	for(int i=0;i<4;i++) {
		for(int j=0;j<4;j++) {
			sum = 0;
			for(int k=0;k<4;k++) {
				sum += a[i][k] * b[k][j];
			}
			r[i][j] = sum;
		}
	}
}

// --------------------------------------------------------------------------------- 
// multiply 1x4 * 4x4 matrix
// --------------------------------------------------------------------------------- 
void matrix_mul1x4_4x4(matrix1x4 a, matrix4x4 b, matrix1x4 r)
{
	float sum;
	for(int j=0;j<4;j++) {
		sum = 0;
		for(int k=0;k<4;k++) {
			sum+=a[k] * b[k][j];
		}
		r[j] = sum;
	}
}

// --------------------------------------------------------------------------------- 
// compute the transpose of matrix 'in' 
// --------------------------------------------------------------------------------- 
void matrix_transpose(matrix4x4 in, matrix4x4 out)
{
	out[0][0] = in[0][0];
    out[0][1] = in[1][0];
    out[0][2] = in[2][0];
    out[0][3] = in[3][0];
    out[1][0] = in[0][1];
    out[1][1] = in[1][1];
    out[1][2] = in[2][1];
    out[1][3] = in[3][1];
    out[2][0] = in[0][2];
    out[2][1] = in[1][2];
    out[2][2] = in[2][2];
    out[2][3] = in[3][2];
    out[3][0] = in[0][3];
    out[3][1] = in[1][3];
    out[3][2] = in[2][3];
    out[3][3] = in[3][3];
}

// --------------------------------------------------------------------------------- 
// create a rotation matrix based angles x,y and z
// --------------------------------------------------------------------------------- 
void rotation_matrix(int x, int y, int z, matrix4x4 r)
{
	matrix4x4 xrot, yrot, zrot,temp;

	if(x == 0 && y == 0 && z == 0) 
		return;
	
	identity_matrix(r);
	identity_matrix(xrot);
	identity_matrix(yrot);
	identity_matrix(zrot);
	identity_matrix(temp);

	// x rotation
	if(x) {
		xrot[1][1] = cos_lookup[x];
		xrot[1][2] = sin_lookup[x];
		xrot[2][1] = -sin_lookup[x];
		xrot[2][2] = cos_lookup[x];
	}

	// y rotation
	if(y) {
		yrot[0][0] = cos_lookup[y];
		yrot[0][2] = -sin_lookup[y];
		yrot[2][0] = sin_lookup[y];
		yrot[2][2] = cos_lookup[y];
	}

	// z rotation
	if(z) {
		zrot[0][0] = cos_lookup[z];
		zrot[0][1] = sin_lookup[z];
		zrot[1][0] = -sin_lookup[z];
		zrot[1][1] = cos_lookup[z];
	}

	matrix_mul4x4_4x4(xrot, yrot, temp);
	matrix_mul4x4_4x4(temp, zrot, r);
}

// --------------------------------------------------------------------------------- 
// usefull function to match two numbers but allow for a margin of error
// --------------------------------------------------------------------------------- 
int match(float a, float b, float epsilon)
{
	if(a >= b-epsilon && a <= b+epsilon) return 1;
	else return 0;
}


// END OF FILE ---------------------------------------------------------------------

