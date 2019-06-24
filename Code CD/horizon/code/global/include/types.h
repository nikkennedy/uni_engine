/*	--------------------------------------------------------------------------------- 
	Filename      - types.h
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Definition of important types used throughout
	--------------------------------------------------------------------------------- */

#ifndef _TYPES_H_
#define _TYPES_H_

#include <list>
#include <vector>
#include <iostream>

using namespace std;

// --------------------------------------------------------------------------------- 
// provided to simplify code
// --------------------------------------------------------------------------------- 
typedef unsigned int u_int;
typedef unsigned long u_long;
typedef unsigned char u_char,byte;
typedef unsigned short u_short;
typedef signed int s_int;
typedef signed long s_long;
typedef signed char s_char;
typedef signed short s_short;

// --------------------------------------------------------------------------------- 
// A three dimensional vector
// --------------------------------------------------------------------------------- 
typedef struct _Vector3D {
	float x;
	float y;
	float z;
} Vector3D;

#define VECTOR3D(v, x, y, z) v[0] = x; v[1] = y; v[2] = z;

// --------------------------------------------------------------------------------- 
// a two dimensional vertex
// --------------------------------------------------------------------------------- 
typedef struct _Vert2D {
	float x;
	float y;
} Vert2D;

// --------------------------------------------------------------------------------- 
// a three dimensional vertex
// --------------------------------------------------------------------------------- 
typedef struct _Vert3D {
	float x;
	float y;
	float z;
} Vert3D;

// --------------------------------------------------------------------------------- 
// include the CVert3D class definition
// --------------------------------------------------------------------------------- 
#include <vert3d.h>

// --------------------------------------------------------------------------------- 
// define the matrices
// --------------------------------------------------------------------------------- 
typedef float matrix4x4[4][4];
typedef float matrix1x4[4];

#define MATRIX1X4(m, x, y, z, w) m[0] = x; m[1] = y; m[2] = z; m[3] = w;


// --------------------------------------------------------------------------------- 
// structure to hold colour values
// --------------------------------------------------------------------------------- 
typedef struct _Color {
	float r;
	float g;
	float b;
} Color;

// --------------------------------------------------------------------------------- 
// structure to hold definition of a material
// --------------------------------------------------------------------------------- 
typedef struct _Material {
	char name[255];     // material name
	int num;            // material number
	char filename[255]; // filename of the BITMAP texture
} Material;


// --------------------------------------------------------------------------------- 
// structure to hold definition of a face/poly
// --------------------------------------------------------------------------------- 
typedef struct _Face {
	int v0; // indexes into list of verts
	int v1;
	int v2;
	int tv0; // indexes into the list of texture verts
	int tv1;
	int tv2; 
	int material_id;
	int is_static; // if this = 1 then the face should not be split 
	int lightmap_id;	
} Face; 

// --------------------------------------------------------------------------------- 
// structure to hold definition of a mesh
// --------------------------------------------------------------------------------- 
typedef struct _Mesh {
	int num_verts;
	int num_faces;
	int num_tverts; // number of texture verts

	CVert3D **tverts;       // list of all the texture verts 
	CVert3D **verts;        // list of all the verts in mesh
	Face *faces;          // all the faces with indexes into vertex list
	CVert3D **vert_normals; // a normal for each vertex

	int vert_offset; // offset to add, in the vert list of HRZ file 
	int face_offset; 
	int tvert_offset;

} Mesh;

// --------------------------------------------------------------------------------- 
// structure to hold definition of geometry
// --------------------------------------------------------------------------------- 
typedef struct _GeomObject {
	char name[100];  // name of the object
	Mesh mesh;       // mesh used by object
	int material_id; // index into list of materials for texture info
	int is_static; // if this = 1 then the face should not be split 
} GeomObject;

// --------------------------------------------------------------------------------- 
// structure to hold a message, this is used to pass information between game unit
// and render units
// --------------------------------------------------------------------------------- 
typedef struct _MESSAGE_
{
	char desc[100];
	int  dest;
	char details[250];
} Message;

// --------------------------------------------------------------------------------- 
// provide list of verts and corresponding iterator
// --------------------------------------------------------------------------------- 
typedef list<Vert3D> Vert3DList;
typedef Vert3DList::iterator Vert3DIterator;

// --------------------------------------------------------------------------------- 
// Pointer to a function, required for use by callbacks
// --------------------------------------------------------------------------------- 
typedef int (*callback_type)(int);

// --------------------------------------------------------------------------------- 
// CPlane - Class that holds and provides methods to manipulate planes
// --------------------------------------------------------------------------------- 
class CPlane {
public:
	float a; // definition of the plane
	float b;
	float c;
	float d;

	float x; // point on the plane
	float y;
	float z;

	CPlane() {};
	~CPlane() {};

	// --------------------------------------------------------------------------------- 
	// calculate a plane using the normal and a point on the plane
	// --------------------------------------------------------------------------------- 
	CPlane(CVert3D &n, CVert3D &v) 
	{
		a = n.x;
		b = n.y;
		c = n.z;
		x = v.x;
		y = v.y;
		z = v.z;
		d = -(a * x + b * y + c * z);
	}

	// --------------------------------------------------------------------------------- 
	// classify a point, is it in front or behind the plane
	// --------------------------------------------------------------------------------- 
	float classify(CVert3D &v) 
	{
		return (a * v.x) + (b * v.y) + (c * v.z) + d;	
	}

	// --------------------------------------------------------------------------------- 
	// return the planes normal
	// --------------------------------------------------------------------------------- 
	CVert3D normal()
	{
		return CVert3D(a,b,c);
	}

	// --------------------------------------------------------------------------------- 
	// invert the plane
	// --------------------------------------------------------------------------------- 
	void invert()
	{
		a *= -1;
		b *= -1;
		c *= -1;
		d = -(a * x + b * y + c * z);
	}

	// --------------------------------------------------------------------------------- 
	// Copy constructor
	// --------------------------------------------------------------------------------- 
	inline CPlane operator=(const CPlane &p) 
	{
		a = p.a;
		b = p.b;
		c = p.c;
		d = p.d;
		
		x = p.x;
		y = p.y;
		z = p.z;

		return *this;
	}

	// --------------------------------------------------------------------------------- 
	// boolean != operator
	// --------------------------------------------------------------------------------- 
	inline bool operator!=(CPlane &pl)
	{
		if(a != pl.a) 
			return true;
		if(b != pl.b)
			return true;
		if(c != pl.c)
			return true;
		if(d != pl.d)
			return true;

		return false;
	}

};

// --------------------------------------------------------------------------------- 
// CPoly Class definition and implementation which holds and provides methods for
// working with polygons
// --------------------------------------------------------------------------------- 
class CPoly {
public:
	int vi[3];			// vertices
	int ti[3];			// texture vertices
	int ni;				// normal
	int material_id;	// material
	int plane_id;		// plane
	int is_static;      // splittable?
	int lightmap_id;

	CPlane p1;          // perpendicular planes for each edge of poly
	CPlane p2;          // used for collision detection
	CPlane p3;

	float incline_angle; // will be precalculated

	float lm_u[3];
	float lm_v[3];
	int   lm_w;
	int   lm_h;

	// --------------------------------------------------------------------------------- 
	// destructor
	// --------------------------------------------------------------------------------- 
	~CPoly() {}

	// --------------------------------------------------------------------------------- 
	// simple constructor
	// --------------------------------------------------------------------------------- 
	CPoly(void)
	{ 
		lm_u[0] = 0;
		lm_v[0] = 0;
		lm_u[1] = 0;
		lm_v[1] = 0;
		lm_u[2] = 0;
		lm_v[2] = 0;		
		p1 = CPlane(CVert3D(0,0,0), CVert3D(0,0,0));
		lm_w = lm_h = 0;
		incline_angle = -1;
	} 
	
	// --------------------------------------------------------------------------------- 
	// copy constructor
	// --------------------------------------------------------------------------------- 
	CPoly(CPoly *p) {	
		vi[0] = p->vi[0];
		vi[1] = p->vi[1];
		vi[2] = p->vi[2];
		ti[0] = p->ti[0];
		ti[1] = p->ti[1];
		ti[2] = p->ti[2];
		p1 = p->p1;
		p2 = p->p2;
		p3 = p->p3;
		lightmap_id = p->lightmap_id;
		ni = p->ni;
		material_id = p->material_id;
		plane_id = p->plane_id;
		is_static = p->is_static;
		lm_u[0] = p->lm_u[0];
		lm_v[0] = p->lm_v[0];
		lm_u[1] = p->lm_u[1];
		lm_v[1] = p->lm_v[1];
		lm_u[2] = p->lm_u[2];
		lm_v[2] = p->lm_v[2];
		lm_w = p->lm_w;
		lm_h = p->lm_h;
		incline_angle = p->incline_angle;		
	}
	
	// --------------------------------------------------------------------------------- 
	// constructor that provides the vert and tvert indices
	// --------------------------------------------------------------------------------- 
	CPoly(int v1, int v2, int v3, int t1, int t2, int t3) {
		vi[0] = (v1); 
		vi[1] = (v2);
		vi[2] = (v3);
		ti[0] = (t1);
		ti[1] = (t2);
		ti[2] = (t3);
		lm_u[0] = 0;
		lm_v[0] = 0;
		lm_u[1] = 0;
		lm_v[1] = 0;
		lm_u[2] = 0;
		lm_v[2] = 0;			
		p1 = CPlane(CVert3D(0,0,0), CVert3D(0,0,0));
		lm_w = lm_h = 0;
		incline_angle = -1;		
	}
	
	// --------------------------------------------------------------------------------- 
	// check two CPolys match, based on list of vertices in vertex_list
	// --------------------------------------------------------------------------------- 
	bool check_match(CPoly *pin, CVert3D **vertex_list)
	{		
		if(pin == NULL) return 0;

		for(int i=0;i<3;i++) {
			if(vertex_list[pin->vi[i]] != vertex_list[vi[i]]) 
				return 0;			
		}
		return 1;
	}

	// --------------------------------------------------------------------------------- 
	// Copy 
	// --------------------------------------------------------------------------------- 
	inline CPoly operator=(const CPoly &p)
	{
		vi[0] = p.vi[0];
		vi[1] = p.vi[1];
		vi[2] = p.vi[2];

		ti[0] = p.ti[0];
		ti[1] = p.ti[1];
		ti[2] = p.ti[2];

		ni = p.ni;
		material_id = p.material_id;
		plane_id = p.plane_id;
		is_static = p.is_static;

		p1 = p.p1;
		p2 = p.p2;
		p3 = p.p3;

		lm_u[0] = p.lm_u[0];
		lm_v[0] = p.lm_v[0];
		lm_u[1] = p.lm_u[1];
		lm_v[1] = p.lm_v[1];
		lm_u[2] = p.lm_u[2];
		lm_v[2] = p.lm_v[2];

		lm_w = p.lm_w;
		lm_h = p.lm_h;

		incline_angle = p.incline_angle;
		return *this;
	}
	
	// --------------------------------------------------------------------------------- 
	// Comparison
	// --------------------------------------------------------------------------------- 
	inline bool operator==(CPoly &p)
	{
		for(int i=0;i<3;i++) {
			if(vi[i] != p.vi[i])
				return false;
			if(ti[i] != p.ti[i])
				return false;
			if(lm_u[i] != p.lm_u[i])
				return false;
			if(lm_v[i] != p.lm_v[i])
				return false;
		}

		if(ni != p.ni)
			return false;
		if(plane_id != p.plane_id)
			return false;
		if(material_id != p.material_id)
			return false;
		if(is_static != p.is_static)
			return false;

		return true;
	}

	// --------------------------------------------------------------------------------- 
	// Comparision !=
	// --------------------------------------------------------------------------------- 
	inline bool operator!=(CPoly &p)
	{
		return !(p == *this);
	}

	// --------------------------------------------------------------------------------- 
	// Create three planes that are perpendicular to the sides of this polygon
	// --------------------------------------------------------------------------------- 
	void create_per_pen_plane(CVert3D **vertex_list, CVert3D **normal_list)
	{
		CVert3D dir, normal;

		dir       = *vertex_list[vi[0]] - *vertex_list[vi[1]];
		normal    = dir ^ *normal_list[ni];
		normal.normalise();
		p1 = CPlane(normal, *vertex_list[vi[0]]);

		dir       = *vertex_list[vi[1]] - *vertex_list[vi[2]];
		normal    = dir ^ *normal_list[ni];
		normal.normalise();	
		p2 = CPlane(normal, *vertex_list[vi[1]]);

		dir       = *vertex_list[vi[2]] - *vertex_list[vi[0]];
		normal    = dir ^ *normal_list[ni];
		normal.normalise();
		p3 = CPlane(normal, *vertex_list[vi[2]]);			

		if(p1.classify(*vertex_list[vi[2]]) < 0) p1.invert();
		if(p2.classify(*vertex_list[vi[0]]) < 0) p2.invert();
		if(p3.classify(*vertex_list[vi[1]]) < 0) p3.invert();
	}	
};

// --------------------------------------------------------------------------------- 
// provide vector of polygons and corresponding iterator
// --------------------------------------------------------------------------------- 
typedef vector<CPoly> CPolyVector;
typedef CPolyVector::iterator CPolyIterator;

// --------------------------------------------------------------------------------- 
// CPointSet - Class that holds a list of three dimensional points
// --------------------------------------------------------------------------------- 
class CPointSet {
public:
	// --------------------------------------------------------------------------------- 
	// actual list that holds points
	// --------------------------------------------------------------------------------- 
	Vert3DList list;

	// --------------------------------------------------------------------------------- 
	// constructor
	// --------------------------------------------------------------------------------- 
	CPointSet() {}

	// --------------------------------------------------------------------------------- 
	// copy constructor
	// --------------------------------------------------------------------------------- 
	CPointSet(CPointSet *ps) {		
		Vert3DList l = ps->list;
		for(Vert3DIterator itor = l.begin(); itor != l.end(); itor++)
			list.push_back(*itor);
	}

	// --------------------------------------------------------------------------------- 
	// add a point
	// --------------------------------------------------------------------------------- 
	void add_point(Vert3D p) {
		list.push_back(p);
	}

	// --------------------------------------------------------------------------------- 
	// get the list of points
	// --------------------------------------------------------------------------------- 
	Vert3DList get_set() {
		return list;
	}
};

#endif

// END OF FILE ----------------------------------------------------------------------