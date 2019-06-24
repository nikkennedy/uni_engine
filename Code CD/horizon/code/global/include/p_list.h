/*	--------------------------------------------------------------------------------- 
	Filename      - p_list.h
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Definition of the CPolygonList class, a holder for a set of
	                polygons.
	--------------------------------------------------------------------------------- */

#ifndef _P_LIST_H_
#define _P_LIST_H_

#include <types.h>

#include <vector>
#include <iostream>

using namespace std;

// --------------------------------------------------------------------------------- 
// predetermined limits
// --------------------------------------------------------------------------------- 
#define MAX_POLYS_PER_LIST 6200
#define MAX_VERTS_PER_LIST 10000

// --------------------------------------------------------------------------------- 
// CPolygonList
// --------------------------------------------------------------------------------- 
class CPolygonList {
private:
	// --------------------------------------------------------------------------------- 
	// vector of polygons
	// --------------------------------------------------------------------------------- 
	CPolyVector polygons;
	
	// --------------------------------------------------------------------------------- 
	// reference to address of polygon
	// --------------------------------------------------------------------------------- 
	CPoly **polygon_ref; // the set of polygons in this list
	int num_polygons;    // total polygons in list	
	int using_ref;

public:
	// --------------------------------------------------------------------------------- 
	// constructors
	// --------------------------------------------------------------------------------- 
	CPolygonList(int num_polygons, CPoly **polygons);
	~CPolygonList();
	CPolygonList();    // generate a fixed length list of polygons
	CPolygonList(CPolygonList *pl);
	CPolygonList(int ref); // if ref = 1 then polygon list is reference only

	int count(void);   // the number of polygons in list
	CPoly *get(int i);  // get a polygon based on index
	void add(CPoly *p); // add a polygon to the list
	void clean();

	void add_ref(CPoly *p); // add a polygon to the list
	void clean_ref();

	// --------------------------------------------------------------------------------- 
	// for speedy rendering!
	// --------------------------------------------------------------------------------- 
	int has_display_list;
	int display_list_id;
};

#endif

// END OF FILE ------------------------------------------------------------------