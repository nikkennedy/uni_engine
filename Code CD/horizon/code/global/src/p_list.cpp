/*	--------------------------------------------------------------------------------- 
	Filename      - p_list.cpp
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Implementation of the CPolygonList class, a holder for a set of
	                polygons.
	--------------------------------------------------------------------------------- */

#include <windows.h>
#include <stdio.h>
#include <types.h>
#include <debug.h>
#include <p_list.h>

// global variable to keep track of number of lists created
int polylist_counter = 0;

// --------------------------------------------------------------------------------- 
// constructor which pushes num_polygons onto list
// --------------------------------------------------------------------------------- 
CPolygonList::CPolygonList(int num_polygons, CPoly **polygons)
{
	using_ref = 0; // actual polygons not references
	for(int i=0;i<num_polygons;i++) {
		this->polygons.push_back(polygons[i]);
	}

	polylist_counter++;
}

// --------------------------------------------------------------------------------- 
// constructor which creates a polygon list that keeps tracks of references to 
// polygons not the actual CPoly classes themselves
// --------------------------------------------------------------------------------- 
CPolygonList::CPolygonList(int ref)
{
	using_ref          = 1;
	num_polygons       = 0;

	polylist_counter++;

	if((polygon_ref = (CPoly **) malloc(sizeof(CPoly*) * MAX_POLYS_PER_LIST)) == NULL) {
		printf("Can not allocate memory for polygon list, %d\n", polylist_counter);
		return;
	}


}

// --------------------------------------------------------------------------------- 
// constructor that doesn't do much
// --------------------------------------------------------------------------------- 
CPolygonList::CPolygonList()
{
	using_ref = 0;
	polylist_counter++;
}

// --------------------------------------------------------------------------------- 
// Copy Constructor
// --------------------------------------------------------------------------------- 
CPolygonList::CPolygonList(CPolygonList *pl)
{
	using_ref = 0;
	polylist_counter++;
	for(int i=0;i<pl->count();i++) {
		add(pl->get(i));
	}
}

// --------------------------------------------------------------------------------- 
// return the number of polygons contained in list or referenced
// --------------------------------------------------------------------------------- 
int CPolygonList::count(void)
{
	if(using_ref == 0)
		return polygons.size();
	else
		return num_polygons;
}

// --------------------------------------------------------------------------------- 
// return the polygon at index i
// --------------------------------------------------------------------------------- 
CPoly *CPolygonList::get(int i)
{	
	if(using_ref == 0)
		return (CPoly*) &polygons[i];
	else 
		return polygon_ref[i];
}

// --------------------------------------------------------------------------------- 
// add a polygon to the list of polygons (NOT FOR REFERENCED VALUES)
// --------------------------------------------------------------------------------- 
void CPolygonList::add(CPoly *p)
{
	polygons.push_back( new CPoly(p) );
}

// --------------------------------------------------------------------------------- 
// add a reference to a polygon
// --------------------------------------------------------------------------------- 
void CPolygonList::add_ref(CPoly *p)
{
	if(num_polygons+1 >= MAX_POLYS_PER_LIST) {
		printf("Can not add poly as reached maximum per list %d\n", MAX_POLYS_PER_LIST);
		return;
	}
	using_ref = 1;
	polygon_ref[num_polygons++] = p;
}

// --------------------------------------------------------------------------------- 
// destructor
// --------------------------------------------------------------------------------- 
CPolygonList::~CPolygonList()
{
	if(using_ref == 0) {
		if(polygons.size() > 0) {
			polygons.clear();
		}
	}

	if(using_ref == 1) {
		num_polygons = 0;
		free(polygon_ref);			
	}
}

// --------------------------------------------------------------------------------- 
// remove all the polygons from the list (NON REFERENCE)
// --------------------------------------------------------------------------------- 
void CPolygonList::clean()
{
	if(polygons.size() > 0) {
		polygons.clear();
	}
}

// --------------------------------------------------------------------------------- 
// remove all polygons from list (REFERENCED)
// --------------------------------------------------------------------------------- 
void CPolygonList::clean_ref()
{
	num_polygons = 0;
	using_ref = 1;
}

// END OF FILE ---------------------------------------------------------------------