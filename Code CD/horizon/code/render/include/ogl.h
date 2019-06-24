/*	--------------------------------------------------------------------------------- 
	Filename      - ogl.h
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Definition of COpenGLRend
					This is the main implementation of the OpenGL version of CRend
					It contains all the functions required to support the CRend
					superclasses framework allowing applications to use OpenGL as the
					rendering medium.
	--------------------------------------------------------------------------------- */

#ifndef _OGL_H_
#define _OGL_H_

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include <object.h>
#include <types.h>
#include <rend.h>
#include <lbsp.h>
#include <light.h>
#include <md2.h>
#include <ogl_particlesystem.h>

#define OGL_REND_VERSION 1

class COpenGLRend : public CRend {

	// Constructor
public:
	COpenGLRend();

	// Initialization
protected:
	void initialize(int width, int height);
	int  setup_pixel_format(void);
public:
	int  create_renderer(HWND hwnd);

	// BSP Specific
public:
	void set_bsp(CLBSP *bsp);

	// Renderer Unit State
public:
	void set_state_var(int state_var, int value);

	// Rendering
protected:
	CObject  *viewer;
	void first_pass_rend(CPoly *p);
	void draw_world();
	void update_screen_size(int width, int height);
	void draw_polygons(CPolygonList *p_list);
	void render_base();
	int  draw_scene(void);
public:
	void update(CObject *viewer);

	// Both Consoles
public:
	void  set_console_font(char *filename);
	
	// Drop Down Console Output
protected:
	float console_tex0_move;
	float console_tex1_move;
	int   console_open_amount;
	int   start_opening_console;
	int   start_closing_console;
	void  console_update();
	void  console_open();
	void  process_input(char *msg);
	
	// Debug Console
protected:
	void  draw_console(void);

	// OpenGL Specific
protected:
	char *get_vendor_name(void);
	
	// Extentions Initialization	
protected:
	int  init_nv(void);

	// Lighting and Textures
protected:
	void create_texture_manager(void);
	void load_light_maps(CBSPNode *bsp);
	void load_sfx_textures(void);
	
	// Particle System
protected:
	COGLParticleSystem **particle_systems;
	void render_particles();
public:
	int  add_particle_system(CParticleSystem *ps);

	// Objects
protected:
	void render_md2_model(CMD2 *model, CVert3D pos, CVert3D ori,CTextureManager *in_tm);
	void render_hrz_shape(CObject *obj);
	void render_object(CObject *obj);
public:
	void add_object(CObject *obj);

	// Sprites
protected:
	void render_sprites();
public:
	void add_sprite(CSprite *sprite);

	// Debugging Functions
private:
#ifdef DRAW_NODE_POINTS
	CVert3D starts[10000];
	CVert3D endss[10000];
	CVert3D hits[100000];
	int planeids[1000];
	int planecount;
	int sandes;
	int hitcount;
	void debug_load_node_points(int number_nodes);
	void debug_draw_node_points(CBSPNode *np);
#endif

	// System
public:
	void flush_system();
};

// END OF FILE ------------------------------------------------------------------------------

#endif