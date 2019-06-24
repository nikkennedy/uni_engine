/*	--------------------------------------------------------------------------------- 
	Filename      - vert3d.h
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Definition and Implementation of CVert3D
					This is one of the most important classes, it stores and provides
					functions for three-dimensional vertices and vectors
	--------------------------------------------------------------------------------- */

#ifndef _VERT3D_H_
#define _VERT3D_H_

#include <math.h>

class CVert3D {

public:
	float x;
	float y;
	float z;

public:
	// All the possible constructors
	inline CVert3D(float sx, float sy, float sz) : x(sx), y(sy), z(sz) {};
	inline CVert3D()           : x(0), y(0), z(0) {};
	// Copy Constructors
	inline CVert3D(CVert3D &v) : x(v.x), y(v.y), z(v.z) {};
	inline CVert3D(CVert3D *v) : x(v->x), y(v->y), z(v->z) {};		
	inline CVert3D(Vert3D v)   : x(v.x), y(v.y), z(v.z) {};

	// Cross Product Calculation
	inline CVert3D operator^(CVert3D &v) 
	{
		return CVert3D( (y*v.z-z*v.y), (z*v.x-x*v.z), (x*v.y-y*v.x) );
	}
	
	// Sum Calculation
	inline CVert3D operator+(CVert3D &v) 
	{
		return CVert3D( x + v.x, y + v.y, z + v.z);
	}

	// Sum Calculation
	inline CVert3D operator+=(CVert3D &v) 
	{
		return CVert3D( x + v.x, y + v.y, z + v.z);
	}

	// Sub calculation
	inline CVert3D operator-(CVert3D &v)
	{
		return CVert3D( x - v.x, y - v.y, z - v.z);
	}

	// Dot Product Calculation
	inline float operator*(CVert3D &v)
	{
		return (x * v.x + y * v.y + z * v.z);
	}

	// Mul Calculation
	inline CVert3D operator*(float f)
	{
		return CVert3D(x * f, y * f, z * f);
	}

	// Stores x,y and z in verts
	inline void get_floats(Vert3D *verts) {
		verts->x = x;
		verts->y = y;
		verts->z = z;
	}

	// Provide array access [0] = x, [1] = y, [2] = z
	inline float operator[](int i)
	{
		if(i == 0) return x;
		if(i == 1) return y;
		if(i == 2) return z;

	}

	// Equals
	inline CVert3D& operator=(CVert3D &v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	// Div Calculation
	inline CVert3D operator/(float div) 
	{
		float inv = 1.0f/div;
		return CVert3D(x * inv, y * inv, z * inv);
	}

	// Sub constant m from each component
	inline CVert3D operator-(float m) 
	{
		return CVert3D(x - m, y - m, z - m);
	}

	// Add constant m from each component
	inline CVert3D operator+(float p) 
	{
		return CVert3D(x + p, y + p, z + p);
	}

	// Normalise a vector
	inline void normalise(void)
	{
		float length = (float)sqrtf(x*x + y*y + z*z);

		if(length == 0) {
			x = 0;
			y = 0;
			z = 0;
		}

		*this = *this / length;
	}

	// Negate 
	inline CVert3D operator-() const
	{
		return CVert3D(-x, -y, -z);
	}

	// Compare two verts
	inline bool operator!=(CVert3D &v) 
	{
		if(v.x != x || v.y != y || v.z != z) return 1;
			else
		return 0;
	}

	// Compare two verts
	inline bool operator==(CVert3D &v)
	{
		if(v.x == x && v.y == y && v.z == z) 
			return true;
		else
			return false;
	}

	// Return length of vector
	inline float length()
	{
		return (float)sqrtf(x*x + y*y + z*z);
	}

};

#endif

// END OF FILE ---------------------------------------------------------------