/*	--------------------------------------------------------------------------------- 
	Filename      - collisiondetection.cpp
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Code to handle collision detection
	--------------------------------------------------------------------------------- */

#include <windows.h>
#include <stdio.h>
#include <ogl.h>
#include <debug.h>
#include <types.h>
#include <maths.h>
#include <object.h>
#include <hrz_config.h>

// --------------------------------------------------------------------------------- 
// Returns 1 is point is within polygon, 0 otherwise
// --------------------------------------------------------------------------------- 
int CRend::check_inside_poly(CPoly *poly, CVert3D point)
{
	// check against per-pen-planes

	float a = poly->p1.classify(point);// + c_radius;
	if(a < 0) return 0;
	float b = poly->p2.classify(point);// + c_radius;
	if(b < 0) return 0;
	float c = poly->p3.classify(point);// + c_radius;
	if(c < 0) return 0;

	return 1; // polygon was hit

}

// --------------------------------------------------------------------------------- 
// Calculates the point of intersection between plane and vector
// --------------------------------------------------------------------------------- 
float CRend::chop_line(CVert3D *v1, CVert3D *v2, CPlane *pl, CVert3D &out)
{
	CVert3D norm = CVert3D(pl->a, pl->b, pl->c);
	CVert3D vec = *v1 - *v2;

	float start_dist_from_plane = pl->classify(*v1); 
	float projected_line_length = vec*norm;          			

	if(projected_line_length == 0) {
		return -1;
	}

	float scale = start_dist_from_plane / projected_line_length;			
	out = *v1 - (vec * scale);
	return scale;
}

// --------------------------------------------------------------------------------- 
// returns 1 if object will pass through the polygon, poly otherwise returns 0
// --------------------------------------------------------------------------------- 
int CRend::is_collision(CObject *obj, CPoly *poly)
{
	CPlane plane    = *plane_list[poly->plane_id];

	CVert3D altered_end_pos = obj->end_position;

	altered_end_pos.y -= obj->height;
	altered_end_pos.z += obj->dir_vector.z * obj->width;
	altered_end_pos.x += obj->dir_vector.x * obj->width;

	float start_side = plane.classify(obj->start_position);
	float end_side   = plane.classify(altered_end_pos);

	// if start_side and end_side multiplied together are < 0 then the object
	// passed across the plane of the polygon being tested.
	if( (start_side * end_side) < 0) { 

		// now check to see if the object passed within the bounds of the 
		// polygon being tested

		// simple check first, just end and start point
		if(check_inside_poly(poly, altered_end_pos) || check_inside_poly(poly, obj->start_position)) {
			return 1; 
		}
		// calculate intersections point and check that just in case
		CVert3D out;
		chop_line(&obj->start_position, &altered_end_pos, &plane, out);
		if(check_inside_poly(poly,out)) {
			return 1; 
		}
	}
	return 0; // object did not hit this polygon	
}

// --------------------------------------------------------------------------------- 
// go through all polygons that must be checked for collision, return the index
// of a polygon that was collided with otherwise return -1
// --------------------------------------------------------------------------------- 
int CRend::check_collision(CObject *obj)
{
	int num_polys = obj->hit_list_pointer->count();

	for(int i=0;i<num_polys;i++) {
		CPoly *p = obj->hit_list_pointer->get(i);
		if(is_collision(obj, p)) 
			return i;
	}

	return -1;
}

//CPolygonList *g_hit_p_list  = new CPolygonList();//NULL;
//CBSPNode     *g_last_s_node = NULL, *g_last_e_node = NULL;
//CPolygonList *g_poly_list;


// --------------------------------------------------------------------------------- 
// this function is called whenever an object must be moved and collision detection
// is required, it deals with calculating which leaf the start and end points of 
// the movement vector are in, then it compiles the list of polygons to check for
// collision against, then it calls the function detect_collision(...) which performs
// the check, if a collision occurs then the object remains in the same position, if
// not then the object is positioned at the end of the movement vector
// --------------------------------------------------------------------------------- 
int CRend::move_object(CObject *obj)
{
	CBSPNode *start_node;
	CBSPNode *end_node;

	CVert3D start_point = obj->start_position;
	CVert3D end_point   = obj->end_position;

	obj->get_dir_vector();
	obj->hit_something = NO_HIT;

	// Only perform collision detection if the object has moved
	if(end_point != start_point) {

		end_point.x += obj->dir_vector.x * obj->width;
		end_point.y -= obj->height;
		end_point.z += obj->dir_vector.z * obj->width;
		
		int node_counter;
		start_node = bsp_tree->which_node(&start_point, node_counter);
		end_node   = bsp_tree->which_node(&end_point, node_counter);

		end_point.x -= obj->dir_vector.x * obj->width;
		end_point.y += obj->height;
		end_point.z -= obj->dir_vector.z * obj->width;

		// If the nodes are different from last time the object moved then recalculate
		// list of polygons that the object passed through
		if(obj->last_start_node != start_node || obj->last_end_node != end_node) {

			obj->last_start_node = start_node;
			obj->last_end_node   = end_node;

			// If the start node and end node are different
			if(start_node != end_node && end_node != NULL) {
				obj->poly_hit_list->clean_ref(); // empty the list of polygons

				// concatenated start and end nodes polygons
				int start_poly_count = start_node->polygons->count();
				int end_poly_count   = end_node->polygons->count();

				for(int i=0;i<start_poly_count;i++) {					
					CPoly *p = start_node->polygons->get(i);
					obj->poly_hit_list->add_ref(p);
				}

				for(    i=0;i<end_poly_count;  i++) {
					CPoly *p = end_node->polygons->get(i);
					obj->poly_hit_list->add_ref(p);
				}

				for(i=0;i<start_node->num_vis_nodes;i++) {
					CBSPNode *n = node_list[start_node->vis_node_list[i]];
					CPolygonList *plist = n->polygons;
					int numpolys = plist->count();
					for(int j=0;j<numpolys;j++) {						
						CPoly *p =plist->get(j);
						obj->poly_hit_list->add_ref(p);
					}					
				}
				for(i=0;i<end_node->num_vis_nodes;i++) {
					CBSPNode *n = node_list[end_node->vis_node_list[i]];
					CPolygonList *plist = n->polygons;
					int numpolys = plist->count();
					for(int j=0;j<numpolys;j++) {
						CPoly *p = plist->get(j);
						obj->poly_hit_list->add_ref(p);
					}					
				}
				
				// set the global polygon list to point to the concatenated
				// polygon list
				obj->hit_list_pointer = obj->poly_hit_list;			

			} else {
				// Otherwise set global polygon list to just the start nodes
				// polygons
				obj->hit_list_pointer = start_node->polygons;
			}
		}

		detect_collision(obj);
	}

	if(obj->hit_something == NO_HIT || state_clipping == STATE_CLIP_OFF) {
		obj->location    = obj->end_position;
		obj->orientation = obj->orientation;
		return NO_HIT;
	} else {
		obj->end_position = obj->start_position;
		obj->hit_something = HIT_POLY;
		return HIT_POLY;
	}
}

// --------------------------------------------------------------------------------- 
// the guts of the collision detection routing, if a collision is detected with a 
// polygon, then a check is made to see if the polygon is part of a staircase which
// means object is moved up the staircase.
// --------------------------------------------------------------------------------- 
void CRend::detect_collision(CObject *obj)
{
	CVert3D a_normal;
	float incline_angle;

	obj->hit_something = NO_HIT;

	if((obj->hit_poly_index = check_collision(obj)) != NO_COLLISION) {

		CPoly p        = obj->hit_list_pointer->get(obj->hit_poly_index);
		CVert3D normal = *normal_list[p.ni];

		// Polygon has been hit, check to see the incline of the polygon, if its too steep
		// then the object can not move up, so the object is told not to move, otherwise
		// an attempt is made to climb up/down the slope, or stairs

		if(p.incline_angle != -1) {
			incline_angle = p.incline_angle;
		} else {
			a_normal.x = (float)fabs(normal.x);
			a_normal.y = (float)fabs(normal.y);
			a_normal.z = (float)fabs(normal.z);

			if(a_normal.x >= a_normal.z && a_normal.x >= a_normal.y) {
				if(normal.x > 0)
					incline_angle = CVert3D(0,1,0) * normal;
				else
					incline_angle = CVert3D(0,-1,0) * normal;
			} else
			if(a_normal.y > a_normal.z && a_normal.y > a_normal.x) {
				if(normal.y > 0)
					incline_angle = CVert3D(0,1,0) * normal;
				else
					incline_angle = CVert3D(0,-1,0) * normal;
			} else
			if(a_normal.z > a_normal.x && a_normal.z > a_normal.y) {
				if(normal.z > 0)
					incline_angle = CVert3D(0,1,0) * normal;
				else
					incline_angle = CVert3D(0,-1,0) * normal;
			} 
			incline_angle = (float)acos(incline_angle) / (float)M_PI * 180.0f;
			p.incline_angle = incline_angle;
		}		

		if(incline_angle > 60.0f && (incline_angle < 89 || incline_angle > 91))  {
			obj->hit_something = HIT_POLY;
			return;
		}

		obj->hit_something = HIT_POLY;

		// Now check to see if the object moving up makes the collisions stop, i.e. there
		// is a possibility that the object hit is part of a set of stairs that can be
		// climbed

		obj->start_position.y += obj->step_amount;
		obj->end_position.y   += obj->step_amount;

		if(check_collision(obj) == NO_COLLISION) {
			// A step has been found, now only move up an amount approx equal to the size
			// of the step
			obj->hit_something = NO_HIT;

			obj->start_position.y -= (obj->step_amount - obj->step_unit); // okay now need to work out proper height to jump up...
			obj->end_position.y   -= (obj->step_amount - obj->step_unit);

			for(int i=0;i < (obj->step_amount / obj->step_unit) - obj->step_unit ;i++) { // 5interations

				if(check_collision(obj) == NO_COLLISION) {
					break;
				} else {
					obj->start_position.y += obj->step_unit; 
					obj->end_position.y   += obj->step_unit;
				}
			}		
		} else {
			// polygon is too high to be a step so reset start and end positions 
			obj->start_position.y   -= obj->step_amount; // dont move up
			obj->end_position.y     -= obj->step_amount;
		}
	}	
}

// END OF FILE -----------------------------------------------------------------------