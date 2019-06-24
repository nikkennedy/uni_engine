/*	--------------------------------------------------------------------------------- 
	Filename      - rend.h
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Definition of CRend
					This IS the framework for the rendering unit, it provides a super
					class for all implementations, the following functions are then
					provided for a game unit, or other application to utilise.

				    int  add_particle_system(CParticleSystem *ps)
					int  move_object(CObject *obj)
					int  get_version()
					int  create_renderer(HWND hwnd)
					void set_dxstuff(CDirectX *dx) 
					void set_loc(float x, float y, float z)
					void send_msg(Message *msg)
					void set_state_var(int state_var, int value)
					void add_object(CObject *obj)
					void set_bsp(CLBSP *bsp)	
					void set_console_font(char *filename)
					void update(CObject *viewer)        
					void cons_printf(int pos, char *str, ...)
					void console_printf(char *str, ...)
					void update_screen_size(int width, int height)

	--------------------------------------------------------------------------------- */

#ifndef _REND_H_
#define _REND_H_

#include <windows.h>
#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>

#include <types.h>
#include <lbsp.h>
#include <image.h>
#include <texturemanager.h>
#include <vert3d.h>
#include <dxstuff.h>
#include <object.h>
#include <particlesystem.h>
#include <sprite.h>

// --------------------------------------------------------------------------------- 
// defines that help when using the send_msg(...) function
// --------------------------------------------------------------------------------- 
#define MAX_MESSAGES             100
#define MSG_KEY_PRESSED          1
#define MSG_CONSOLE_SCROLL_UP    2
#define MSG_CONSOLE_SCROLL_DOWN  3
#define MSG_BUFFER_ADD_CHAR      4
#define MSG_BUFFER_REMOVE_CHAR   5
#define MSG_BUFFER_PROCESS_INPUT 6
#define MSG_BUFFER_ADD_MSG       7

// --------------------------------------------------------------------------------- 
// change this according to how many particle systems will be support
// --------------------------------------------------------------------------------- 
#define MAX_PART_SYSTEMS 500

// --------------------------------------------------------------------------------- 
// The possible states the renderer can be in
// --------------------------------------------------------------------------------- 
#define STATE_FILL_TYPE              1
#define		STATE_FILL_POLY          1
#define		STATE_FILL_LINE          2
#define STATE_CLIPPING               2
#define		STATE_CLIP_ON            1
#define     STATE_CLIP_OFF           2
#define STATE_PARTICLES_ACTIVE       3
#define		STATE_PARTICLES_ON       1
#define		STATE_PARTICLES_OFF      0
#define STATE_DEBUG_CONSOLE          4
#define		STATE_DEBUG_CONSOLE_ON   1
#define		STATE_DEBUG_CONSOLE_OFF  0
#define STATE_RENDER_BSP             5
#define		STATE_RENDER_BSP_OFF     0
#define		STATE_RENDER_BSP_ON      1
#define STATE_DROP_DOWN_CONSOLE      6
#define		STATE_DROP_DOWN_ACTIVE   1
#define		STATE_DROP_DOWN_DISABLED 0
#define	STATE_LIGHTMAPS_ACTIVE		 7
#define		STATE_LIGHTMAPS_ON	     1
#define		STATE_LIGHTMAPS_OFF      0
#define STATE_OLD_STYLE_REND         8
#define		STATE_OLD_STYLE_REND_ON  1
#define		STATE_OLD_STYLE_REND_OFF 0

// --------------------------------------------------------------------------------- 
// The call backs
// --------------------------------------------------------------------------------- 
#define CALLBACK_LIGHTMAP_LOADED      1
#define CALLBACK_LOADING_BSP_TEXTURES 2
#define CALLBACK_CREATING_NODE_LIST   3
#define CALLBACK_CREATING_PPP	      4
#define CALLBACK_LOADING_SFX_TEX      5

class CRend {

	// Constructor
public:
	CRend(); 
	
	// Current Render Version Number
protected:
	int version;
public:
	int get_version() { return version; }

	// Initialization
public:
	virtual int  create_renderer(HWND hwnd)                = 0;

	// DirectX I/O
protected:
	CDirectX *dxstuff; 
public:
	void set_dxstuff(CDirectX *dx) { dxstuff = dx; }

	// Placeholders for the viewers location and orientation
protected:
	CVert3D   ori;
	CVert3D   loc;
public:
	void set_loc(float x, float y, float z) { loc.x = x; loc.y = y; loc.z = z; } 

	// Texture Manager 
protected:
	CTextureManager *texture_manager;  
public:
	CTextureManager *get_tex_manager() { return texture_manager; }
	
	// Message Parsing
protected:
	Message *msg_list;
	int current_msg;
	int total_msg;
	void process_message(Message *msg);
	void key_pressed(char *key);
	Message *get_next_message(void);
public:
	virtual void send_msg(Message *msg);
	
	// State Variable Management
protected:
	int state_fill_type;
	int state_clipping;
	int state_particles_active;
	int state_debug_console;
	int state_bsp_render;
	int state_drop_down_console;
	int state_lightmaps_active;
	int state_old_style_rend;
public:
	virtual void set_state_var(int state_var, int value) = 0;

	// Virtual Rendering Functions
protected:
	virtual void draw_polygons(CPolygonList *p_list)       = 0;
	virtual void draw_console(void)                        = 0;
	virtual void render_md2_model(CMD2 *model, CVert3D pos, CVert3D ori,CTextureManager *in_tm) = 0;
	virtual void render_hrz_shape(CObject *obj) = 0;


	// Objects and other Lists
protected:
	CObjectList objects;
	int num_objects;
	virtual void render_objects();
	virtual void render_object(CObject *obj) = 0;
public:
	virtual void add_object(CObject *obj) = 0;
	virtual void remove_object(CObject *obj);

	// Sprite code
protected:
	CSpriteVector sprites;
	virtual void render_sprites() = 0;
public:
	virtual void add_sprite(CSprite *sprite) = 0;	
	virtual void remove_sprite(char *sprite_name);

	// BSP Specific Code
protected:
	CBSPNode **node_list;
	CVert3D **vertex_list;          
	CVert3D **tvertex_list;         
	CVert3D **normal_list;          
	CPlane  **plane_list;      
	CLight  **light_list;
	CLBSP   *bsp_tree;              
	char    just_bsp_name[80];
	void set_vertex_list(CVert3D **vl)         { vertex_list  = vl; }
	void set_texture_vertex_list(CVert3D **vl) { tvertex_list = vl; }
	void set_normal_list(CVert3D **vl)         { normal_list  = vl; }
	void set_plane_list(CPlane **pl)           { plane_list   = pl; }
	void draw_bsp_tree(CBSPNode *bsp, CVert3D loc);
public:
	virtual void set_bsp(CLBSP *bsp) = 0;	

	// Rendering 
public:
	virtual void update(CObject *viewer)         = 0;

	// Both Consoles
protected:
	CFont *console_font;             
public:
	virtual void set_console_font(char *filename) = 0;
	
	// Debug Console
protected:
	int  current_line;
	char txt[100][100];
	virtual void write_console(char *str, int pos);    // on screen debug console
	virtual void add_console(char *str);	           // on screen debug console
public:
	virtual void cons_printf(int pos, char *str, ...); // on screen debug console

	// Drop Down Console 
protected:
	int  is_console_open;
	char console_input[100];
	int  current_console_in_pos;
	virtual void console_open(void)               = 0;
public:
	virtual void console_printf(char *str, ...);

	// Window Control
protected:
	u_int screen_width;
	u_int screen_height;
public:
	virtual void update_screen_size(int width, int height) = 0;
	
	// Particle Systems
protected:
	int  num_particle_systems;
public:
	virtual int add_particle_system(CParticleSystem *ps) = 0;

	// Collision Detection
protected:
	virtual void create_perpen_planes(CBSPNode *bsp);
	virtual void detect_collision(CObject *obj);
	virtual int  is_collision(CObject *obj, CPoly *poly);
	virtual int  check_collision(CObject *obj);
	virtual int  check_inside_poly(CPoly *poly, CVert3D point);
	virtual float chop_line(CVert3D *v1, CVert3D *v2, CPlane *pl, CVert3D &out);
public:
	virtual int  move_object(CObject *obj);

	// Callback 
public:
	callback_type callback; // stored function pointer
	void set_call_back(callback_type cb) { callback = (callback_type)cb; }
protected:
	void do_callback(int callback_num);


	// System
public:
	virtual void flush_system() = 0;
};

#endif

// END OF FILE --------------------------------------------------------------------