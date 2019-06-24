/*	--------------------------------------------------------------------------------- 
	Filename      - ogl.cpp
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Implementation of COpenGLRend
					This is the main implementation of the OpenGL version of CRend
					It contains all the functions required to support the CRend
					superclasses framework allowing applications to use OpenGL as the
					rendering medium.
	--------------------------------------------------------------------------------- */

#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

// --------------------------------------------------------------------------------- 
// if hvis's debug node output was used when calculating the PVS's, uncommenting this
// renders the points used during the process of calculating the PVS's 
// --------------------------------------------------------------------------------- 
//#define DRAW_NODE_POINTS 

#include <ogl.h>

#include <render_api.h>
#include <debug.h>
#include <texture.h>
#include <types.h>
#include <maths.h>
#include <ogl_texturemanager.h>
#include <light.h>
#include <hrz_config.h>
#include <object.h>
#include <ogl_particlesystem.h>
#include <console.h>
#include <md2.h>
#include <bot.h>
#include <sprite.h>
#include <basictexture.h>
#include <glh_extensions.h>
#include <glh_obs.h>
#include <alphatexture.h>
using namespace glh;

// --------------------------------------------------------------------------------- 
// global variable used to calculate the number of polygons rendered each frame
// --------------------------------------------------------------------------------- 
int  g_poly_count;

// --------------------------------------------------------------------------------- 
// COpenGLRend constructor
// --------------------------------------------------------------------------------- 
COpenGLRend::COpenGLRend() : CRend()
{
	version = OGL_REND_VERSION; 
}

// --------------------------------------------------------------------------------- 
// remove all texture, sprites, objects, particle systems 
// --------------------------------------------------------------------------------- 
void COpenGLRend::flush_system()
{
	COGLTextureManager *ogl_tm = (COGLTextureManager*) texture_manager;

	// remove all objects from the system, actual pointers will be removed elsewhere
	sprites.clear();
	ogl_tm->clear_sprites(); 
	objects.clear();
	num_objects = 0;
	
	// remove all particle systems
	for(int i=0;i<num_particle_systems;i++) {
		delete particle_systems[i];
		particle_systems[i] = NULL;
	}

	num_particle_systems = 0;
}

// --------------------------------------------------------------------------------- 
// must be the first call to the rendering unit, it sets up everthing required.
// --------------------------------------------------------------------------------- 
int COpenGLRend::create_renderer(HWND hwnd)
{
	RECT   rect;

	g_hwnd = hwnd;
	g_hdc  = GetDC(hwnd); 
    
	if(!setup_pixel_format()) {
		MessageBox(NULL, "SetupPixelFormat failed", "Error", MB_OK);  
		return 0;
	}

    g_hrc = wglCreateContext(g_hdc); 
    wglMakeCurrent(g_hdc, g_hrc); 
    GetClientRect(hwnd, &rect);     

	initialize(rect.right, rect.bottom); 

	create_lookups();

	return 1;
}

// --------------------------------------------------------------------------------- 
// places the rendering unit into the correct state
// --------------------------------------------------------------------------------- 
void COpenGLRend::initialize(int width, int height) 
{ 
	int depth_bits, stencil_bits, alpha_bits;
	
	CConsole::console_init(); // initialise drop down console
	console_open_amount   = 0;
	start_opening_console = 0;
	is_console_open       = 0;
	start_closing_console = 0;

	// welcome message
	cprt("Horizon Engine v1.0 Created by NiK Kennedy 2003 (c)\n");
	cprt("---------------------------------------------------\n");

	// read configuration file
	cprt("Reading Configuration File: horizon.ini\n");
	CHrzConfig::read_config_file("horizon.ini");
	cprt("Path to data is: %s\n", CHrzConfig::data_path);
	cprt("Completed.\n");

	cprt("Initializing window with width %d, height %d\n", width, height);	
	screen_width  = width;
	screen_height = height;

	glViewport(0, 0, width, height);

	glPolygonMode(GL_FRONT, GL_FILL );
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glShadeModel(GL_FLAT);
    
	glFrontFace(GL_CCW);   // polygons are front facing if their vertices are counter-clockwise
    glEnable(GL_CULL_FACE);// remove back faces
	glCullFace(GL_BACK);	

    glDrawBuffer(GL_BACK);
    glReadBuffer(GL_BACK);

	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glGetIntegerv(GL_ALPHA_BITS,   &alpha_bits);
	glGetIntegerv(GL_DEPTH_BITS,   &depth_bits);
	glGetIntegerv(GL_STENCIL_BITS, &stencil_bits);

	cprt("Alpha Buffer %d bits, Depth Buffer %d bits, Stencil Buffer %d bits\n", alpha_bits, depth_bits, stencil_bits);	
	cprt("OpenGL Vendor Name: %s\n", get_vendor_name());

	init_nv();  // initialize OpenGL extension for multitexturing

	create_texture_manager(); // create the texture manager

	num_objects = 0;

	// create space for the particle systems
	num_particle_systems = 0;
	particle_systems = (COGLParticleSystem **) malloc(sizeof(COGLParticleSystem*) * MAX_PART_SYSTEMS);

	if(particle_systems == NULL)
		num_particle_systems = -1;

	for(int i=0;i<MAX_PART_SYSTEMS;i++) 
		particle_systems[i] = NULL;

	cprt("Allocated space for %d particle systems.\n", MAX_PART_SYSTEMS);

	bsp_tree = NULL;	
} 

// --------------------------------------------------------------------------------- 
// if window size changes then alter the OpenGL matrices accordinly
// --------------------------------------------------------------------------------- 
void COpenGLRend::update_screen_size(int width, int height)
{
	if(width == 0 || height == 0)
		return;

	cprt("Update Screen Size: Width %d, Height %d\n", width, height);

	screen_width  = width;
	screen_height = height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, width/height, 1, 9000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// --------------------------------------------------------------------------------- 
// global variable to keep track of number of lightmaps loaded
// --------------------------------------------------------------------------------- 
int total_lightmaps = 0;

// --------------------------------------------------------------------------------- 
// load all the lightmaps required by the bsp node, work all the way through the 
// bsp nodes, recursively
// --------------------------------------------------------------------------------- 
void COpenGLRend::load_light_maps(CBSPNode *bsp)
{
	char filenameroot[255];// = "e:\\graphics\\lightmaps";
	char filename[255];

	char just_filename[255];	
	int filename_length = strlen(bsp_tree->bsp_name);

	for(int i=filename_length-1;i>=0;i--) {
		if(bsp_tree->bsp_name[i] == '\\')
			break;
	}

	for(int k=i+1,count=0;k<filename_length;k++) {
		just_filename[count++] = bsp_tree->bsp_name[k];
	}
	just_filename[count] = '\0';

	strcpy(just_bsp_name, just_filename);
	sprintf(filenameroot, "%s\\%s%s\\lightmaps", CHrzConfig::data_path, just_filename, "DATA");

	COGLTextureManager *ogl_tm = (COGLTextureManager*) texture_manager;
	
	if(bsp->front == NULL && bsp->back == NULL) {
		for(int i=0;i<bsp->polygons->count();i++) {
			CPoly p = *bsp->polygons->get(i);
			sprintf(filename, "%s\\lightmap%d.raw", filenameroot, p.lightmap_id);			
			do_callback(CALLBACK_LIGHTMAP_LOADED); // inform game unit that lightmap has been loaded
			ogl_tm->add_lightmap(filename, p.lm_h, p.lm_w);
			total_lightmaps++;
		}

		return;
	}
	if(bsp->front != NULL)
		load_light_maps(bsp->front);
	if(bsp->back != NULL)
		load_light_maps(bsp->back);
}

// --------------------------------------------------------------------------------- 
// load the two console textures and add them to the texture manager
// --------------------------------------------------------------------------------- 
void COpenGLRend::load_sfx_textures(void)
{
	char strbuffer[500];
	COGLTextureManager *ogl_tm = (COGLTextureManager*) texture_manager;
	
	// Console Textures ----------------------------------
	strcpy(strbuffer, CHrzConfig::data_path);
	strcat(strbuffer, CHrzConfig::console_tex0_filename);
	ogl_tm->add_texture(new CBasicTexture(strbuffer, "CONSOLE_TEX0"));

	strcpy(strbuffer, CHrzConfig::data_path);
	strcat(strbuffer, CHrzConfig::console_tex1_filename);
	ogl_tm->add_texture(new CBasicTexture(strbuffer, "CONSOLE_TEX1"));
}

#ifdef DRAW_NODE_POINTS
// --------------------------------------------------------------------------------- 
// load all the visibilty node points and visibility rays
// --------------------------------------------------------------------------------- 
void COpenGLRend::debug_load_node_points(int number_nodes)
{
	char strbuffer[500];

	planecount = 0;
	sandes = 0;
	hitcount = 0;

	// load the visibility lines
	CParse *pars1 = new CParse();
	sprintf(strbuffer, "%s\\%s%s\\raylist.pts", CHrzConfig::data_path, just_bsp_name, "DATA");
	pars1->open_file(strbuffer);

	while(1) {
		if(pars1->read_line()==-1) break;
		if(strncmp(pars1->line, "END",3) == 0) break;
		sscanf(pars1->line, "%f %f %f %f %f %f", &starts[sandes].x, &starts[sandes].y, &starts[sandes].z, &endss[sandes].x, &endss[sandes].y, &endss[sandes].z);
		sandes++;
	}
	
	// load the visibility points
	for(int i=0;i<number_nodes;i++) {
		CParse *pars = new CParse();
		sprintf(strbuffer, "%s\\%s%s\\node%d.pts", CHrzConfig::data_path, just_bsp_name, "DATA", i);
		pars->open_file(strbuffer);
		pars->read_line();
		int l;
		sscanf(pars->line, "%d", &l);
		node_list[i]->pointset = new CPointSet();
		for(int j=0;j<l;j++) {
			pars->read_line();
			Vert3D v;
			sscanf(pars->line, "%f %f %f", &v.x, &v.y, &v.z);
			node_list[i]->pointset->add_point(v);
		}
	}
}
#endif

// --------------------------------------------------------------------------------- 
// setup a new BSP tree to render
// --------------------------------------------------------------------------------- 
void COpenGLRend::set_bsp(CLBSP *bsp)
{
	COGLTextureManager *ogl_tm = (COGLTextureManager*) texture_manager;

	// remove all previously used textures
	ogl_tm->clean_all();

	// remove all previously particle systems
	for(int i=0;i<MAX_PART_SYSTEMS;i++)
		particle_systems[i] = NULL;		

	// if called with NULL then stop rendering and just load console textures
	if(bsp == NULL) {
		load_sfx_textures(); // need the console texture	
		state_bsp_render = STATE_RENDER_BSP_OFF;
		return;
	}

	// Load all the textures --------------------------------------------------------
	callback(CALLBACK_LOADING_BSP_TEXTURES);
	for(i=0;i<bsp->get_num_materials();i++) {
		Material *mat = bsp->get_material(i);		
		ogl_tm->add_texture(new CBasicTexture(mat->filename, mat->name));
	}

	bsp_tree     = bsp;
	vertex_list  = bsp_tree->get_vertex_list();
	tvertex_list = bsp_tree->get_tverts_list();
	normal_list  = bsp_tree->get_normal_list();
	plane_list   = bsp_tree->get_plane_list();
	light_list   = bsp_tree->get_light_list();

	callback(CALLBACK_CREATING_NODE_LIST);
	node_list = (CBSPNode**) malloc(sizeof(CBSPNode*) * 200);
	int number_nodes = 0;
	bsp->create_node_list(bsp->get_bsp(), node_list, &number_nodes);
	console_printf("There are %d nodes\n", number_nodes);


#ifdef DRAW_NODE_POINTS
	debug_load_node_points(number_nodes);
#endif

	// create the perpendicular planes of each polygon for use during collision detection
	callback(CALLBACK_CREATING_PPP);
	create_perpen_planes(bsp_tree->get_bsp());

	// load all the lightmaps
	total_lightmaps = 0;
	load_light_maps(bsp_tree->get_bsp());
	console_printf("Total Lightmaps is %d\n", total_lightmaps);

	loc = CVert3D(0,0,0);

	// load console textures again
	callback(CALLBACK_LOADING_SFX_TEX);
	load_sfx_textures(); // make sure these are loaded after the levels material 
	                     // textures

}

// --------------------------------------------------------------------------------- 
// create a new OpenGL specific texture manager
// --------------------------------------------------------------------------------- 
void COpenGLRend::create_texture_manager(void)
{
	texture_manager = new COGLTextureManager();
	cprt("Texture Manager created.\n");
}

// --------------------------------------------------------------------------------- 
// Set the console font wanted
// --------------------------------------------------------------------------------- 
void COpenGLRend::set_console_font(char *filename)
{
	console_font = new COGLFont(filename);
}

// --------------------------------------------------------------------------------- 
// Draw the polygon p, using multitexturing to apply the lightmap
// --------------------------------------------------------------------------------- 
void COpenGLRend::first_pass_rend(CPoly *p)
{	
	COGLTextureManager *ogl_tm = (COGLTextureManager *) texture_manager;

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	ogl_tm->bind_texture(p->material_id); 

	if(state_lightmaps_active == STATE_LIGHTMAPS_ON) {
		glActiveTextureARB(GL_TEXTURE1_ARB);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		ogl_tm->bind_lightmap(p->lightmap_id);
	}

	glBegin(GL_TRIANGLES);	
			glMultiTexCoord2fvARB(GL_TEXTURE0_ARB, (float*)tvertex_list[p->ti[0]]);
			if(state_lightmaps_active == STATE_LIGHTMAPS_ON)
				glMultiTexCoord2fARB(GL_TEXTURE1_ARB, p->lm_u[0], p->lm_v[0]);
			ogl_tm->process_vertex(vertex_list[p->vi[0]]);				

			glMultiTexCoord2fvARB(GL_TEXTURE0_ARB, (float*)tvertex_list[p->ti[1]]);
			if(state_lightmaps_active == STATE_LIGHTMAPS_ON)
				glMultiTexCoord2fARB(GL_TEXTURE1_ARB, p->lm_u[1], p->lm_v[1]);
			ogl_tm->process_vertex(vertex_list[p->vi[1]]);				

			glMultiTexCoord2fvARB(GL_TEXTURE0_ARB, (float*)tvertex_list[p->ti[2]]);
			if(state_lightmaps_active == STATE_LIGHTMAPS_ON)
				glMultiTexCoord2fARB(GL_TEXTURE1_ARB, p->lm_u[2], p->lm_v[2]);
			ogl_tm->process_vertex(vertex_list[p->vi[2]]);				
					
	glEnd();		
}


// --------------------------------------------------------------------------------- 
// draw all the polygons in the polygon list p_list, first time around create a
// OpenGL display list for the polygon list, then use that to draw the polygon list
// as the size, shape of each polygon will never change.
// --------------------------------------------------------------------------------- 
void COpenGLRend::draw_polygons(CPolygonList *p_list)
{
	if(state_lightmaps_active == STATE_LIGHTMAPS_OFF) {
		int num_polys = p_list->count();
		for(int i=0;i<num_polys;i++) {
			CPoly *p = p_list->get(i);
			first_pass_rend(p);
		}
		return;
	}

	if(p_list->has_display_list == 1 ) { // if display list created use it
		glCallList(p_list->display_list_id);
		g_poly_count+=p_list->count();
	} else {
		p_list->display_list_id = glGenLists(1); // create display list otherwise

		glNewList(p_list->display_list_id, GL_COMPILE);		
			glEnable(GL_TEXTURE_2D);
			int num_polys = p_list->count();
			for(int i=0;i<num_polys;i++) {
				CPoly *p = p_list->get(i);
				first_pass_rend(p);
			}
		glEndList();

		p_list->has_display_list = 1;
	}
}
 
// --------------------------------------------------------------------------------- 
// render each of the active particle systems
// --------------------------------------------------------------------------------- 
void COpenGLRend::render_particles()
{
//	COGLTextureManager *ogl_tm = (COGLTextureManager *) texture_manager;

	for(int i=0;i<MAX_PART_SYSTEMS;i++) {
		if(particle_systems[i] != NULL) {		

			if(((COGLParticleSystem*)particle_systems[i])->update_system(500) == -1) {				
				delete ((COGLParticleSystem*)particle_systems[i]);
				particle_systems[i] = NULL;		
			} else {
				((COGLParticleSystem*)particle_systems[i])->render();	
			}	
		}
	}
}

#ifdef DRAW_NODE_POINTS
// --------------------------------------------------------------------------------- 
// actually render the visibility points and lines
// --------------------------------------------------------------------------------- 
void COpenGLRend::debug_draw_node_points(CBSPNode *np)
{
	glDisable(GL_TEXTURE_2D);
	glPointSize(5.0f);

	glBegin(GL_POINTS);
		glColor4f(1.0,0,0,1);
		Vert3DList point_list = np->pointset->get_set();
		for(Vert3DIterator itor = point_list.begin(); itor != point_list.end();) {
			Vert3D v = (*itor++);			
			glColor4f(1.0,0,0,1);
			glVertex3f(v.x, v.y, v.z);
		}
	glEnd();
	
	glDepthMask(GL_TRUE);

	for(p = 0;p<sandes;p++) {
		glColor4f(0.0,0.0,1.0,1.0);
		glBegin(GL_LINE_STRIP);
			glVertex3fv((float*)&starts[p]);
			glVertex3fv((float*)&endss[p]);
		glEnd();
	}
}
#endif

// --------------------------------------------------------------------------------- 
// This is the main drawing function, it calculates the leaf the viewer is in and
// renders each leaf that is visible using the calculated PVS
// --------------------------------------------------------------------------------- 
void COpenGLRend::draw_world(void)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	if(state_old_style_rend == STATE_OLD_STYLE_REND_OFF) {
		// use PVS
		int node_counter=0;
		CBSPNode *cur_node = bsp_tree->which_node(&loc, node_counter);
		draw_polygons(cur_node->polygons);

		// draw all the visible nodes
		for(int p=0;p<cur_node->num_vis_nodes;p++) {		
			draw_polygons(node_list[cur_node->vis_node_list[p]]->polygons);
		}
	} else {
		// redundant drawing mechanism which renders every node (ignores PVS)
		draw_bsp_tree(bsp_tree->get_bsp(), loc);
	}

#ifdef DRAW_NODE_POINTS
	debug_draw_node_points(cur_node);
#endif

	// Disable everything now
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_FALSE);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	// render the particles
	if(state_particles_active == STATE_PARTICLES_ON)
		render_particles();
}

// --------------------------------------------------------------------------------- 
// This code renders a .HRZ model stored in CObject.shape at the location and 
// orientation specified in the object structure
// --------------------------------------------------------------------------------- 
void COpenGLRend::render_hrz_shape(CObject *obj)
{
	// This is some simple code to make the textures rotate on all .HRZ models
	static float tx0rot = 0;
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	float tranx = (sin(to_radians(tx0rot)));
	float trany = (cos(to_radians(tx0rot)));
	glTranslatef(tranx, trany, 0);
	tx0rot+=0.5f;

	glMatrixMode(GL_MODELVIEW);

	COGLTextureManager *ogl_tm = (COGLTextureManager *) texture_manager;

	CVert3D **vlist = obj->shape->get_vertexlist();
	CPoly   **plist = obj->shape->get_polygons();
	CVert3D **nlist = obj->shape->get_normal_list();
	CVert3D **tlist = obj->shape->get_texture_vertexlist();
	Material *mlist = obj->shape->get_materials();
			
	CVert3D pos = obj->location;
	CVert3D ori = obj->get_orientation();

	glPushMatrix();

		glTranslatef(pos.x, pos.y, pos.z);
		glRotatef(ori.y, 0.0, 1.0, 0.0);
		glRotatef(ori.x, 1.0, 0.0, 0.0);
			
		glEnable(GL_TEXTURE_2D);
				
		int num_polys = obj->shape->get_num_polygons();
				
		for(int j=0;j<num_polys;j++) {
			CPoly *p = plist[j];					
					
			glActiveTextureARB(GL_TEXTURE0_ARB);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			ogl_tm->bind_texture(mlist[p->material_id].name); // base texture

			glBegin(GL_TRIANGLES);
				
				for(int k=0;k<3;k++) {
					glTexCoord2f(tlist[p->ti[k]]->x,tlist[p->ti[k]]->y); 
					glVertex3f(vlist[p->vi[k]]->x, vlist[p->vi[k]]->y, vlist[p->vi[k]]->z);
				}
	
			glEnd();

		}		
	
	glPopMatrix();
	g_poly_count+= num_polys;
	glMatrixMode(GL_TEXTURE); // reset the texture matrix
	glLoadIdentity();
}

// --------------------------------------------------------------------------------- 
// Decides if object is .MD2 or .HRZ and calls the appropriate functions(s)
// --------------------------------------------------------------------------------- 
void COpenGLRend::render_object(CObject *obj)
{	
	if(obj->active != 1) // ignore if not active
		return;

	if(obj->shape != NULL) { // if a .HRZ file
		render_hrz_shape(obj);
		return;
	}

	if(obj->md2_model != NULL) { // its an .MD2 model
		COGLTextureManager *ogl_tm = (COGLTextureManager *) texture_manager;
		render_md2_model(obj->md2_model, obj->location, obj->orientation, ogl_tm);
		CBot *b = (CBot*) obj; // assume its a bot and render weapon as well
		render_md2_model(b->weapon, obj->location, obj->orientation, ogl_tm);
	}
}


// --------------------------------------------------------------------------------- 
// This using renders all the sprites that have been added to the rendering unit
// also renders the cross hair (dot) in the centre of the screen
// --------------------------------------------------------------------------------- 
void COpenGLRend::render_sprites()
{
	COGLTextureManager *ogl_tm = (COGLTextureManager *) texture_manager;

	glMatrixMode( GL_PROJECTION ); 
	glLoadIdentity();
	glOrtho(0,screen_width,0,screen_height,1,-1);
	glMatrixMode( GL_MODELVIEW ); 
	glLoadIdentity();

	// render crosshair
	glPointSize(5);
	glBegin(GL_POINTS);
		glColor3f(1,1,1);
		glVertex3f(screen_width/2,screen_height/2,0);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	// enable alpha testing for transparancy effect
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);

	for(CSpriteIterator itor = sprites.begin(); itor != sprites.end(); itor++) {
		CSprite *sprite = (*itor);

		if(sprite->render) { // only render if render = 1
			int startx = sprite->pos.x;
			int endx = sprite->pos.x+sprite->width;

			int starty = sprite->pos.y;
			int endy = sprite->pos.y+sprite->height;

			ogl_tm->bind_sprite_texture(sprite->texture_id); // sprite texture id

			glBegin(GL_POLYGON);
				glTexCoord2f(0,0);
				glVertex2i(startx,starty);
				glTexCoord2f(1,0);
				glVertex2i(endx,starty);
				glTexCoord2f(1,1);
				glVertex2i(endx,endy);
				glTexCoord2f(0,1);
				glVertex2i(startx,endy);
			glEnd();
		}
	}
}

// --------------------------------------------------------------------------------- 
// This function deals with all the things that are necessary to draw one frame
// --------------------------------------------------------------------------------- 
int COpenGLRend::draw_scene(void)
{
	g_poly_count = 0;

	// Clear all the OpenGL buffers
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Create the project and modelview matrices relative to the users location
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, screen_width/screen_height, 1, 30000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// rotate the world according to the users orientation
	glRotatef(-ori.x, 1.0, 0.0, 0.0);
	glRotatef(-ori.y, 0.0, 1.0, 0.0);
	glRotatef(-ori.z, 0.0, 0.0, 1.0);	
	glTranslatef(-loc.x, -loc.y, -loc.z);

	// render the world and objects
	if(state_bsp_render == STATE_RENDER_BSP_ON) {
		draw_world();
		render_objects();
	}

	// display number of polygons rendered
	if(state_debug_console)
		cons_printf(2, "Rendered %d polys\n", g_poly_count);

	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);

	// render all the added sprites
	render_sprites();

	// render the debug console if set to no
	if(state_debug_console == STATE_DEBUG_CONSOLE_ON)
		draw_console();

	// render the drop down console 
	if(state_drop_down_console == STATE_DROP_DOWN_ACTIVE)
		console_update();

	// flush all the OpenGL commands and swap the double buffers
	glFlush();
	SwapBuffers(g_hdc);

	return 1;
}

// --------------------------------------------------------------------------------- 
// if the console is open set variable signifing to close it otherwise read in the
// contents that need to be displayed on the drop down console and start the process
// of opening it
// --------------------------------------------------------------------------------- 
void COpenGLRend::console_open()
{
	if(is_console_open == 0) {
		CConsole::console_read_file();		
		start_opening_console = 1;
	} else {
		start_closing_console = 1;
	}			
}

// --------------------------------------------------------------------------------- 
// deals with all the processes required to display the drop down console
// --------------------------------------------------------------------------------- 
void COpenGLRend::console_update()
{
	// Move the console down from the top of the screen
	if(start_opening_console) {
		console_open_amount+=20;
		console_tex0_move = 90.f;
		console_tex1_move = 0.f;
		if(console_open_amount >= (int)screen_height) {
			console_open_amount = screen_height;
			start_opening_console = 0;
			is_console_open = 1;
		}
	}

	// Move the console up tp the top of the screen
	if(start_closing_console) {
		console_open_amount-=20;
		if(console_open_amount <= 0) {
			console_open_amount = 0;
			is_console_open = 0;
			start_closing_console = 0;
		}
	}

	// if the console needs to be drawn
	if(start_opening_console || is_console_open) {

		glMatrixMode(GL_PROJECTION);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);
		glMatrixMode( GL_PROJECTION ); 
		glLoadIdentity();
		glOrtho(0,screen_width,0,screen_height,1,-1); // render using orthagonal viewing NOT perspective

		// set up nice scrolling effect for console backgrounds
		COGLTextureManager *ogl_tm = (COGLTextureManager *) texture_manager;

		glActiveTextureARB(GL_TEXTURE0_ARB);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();		
		glTranslatef(0, console_tex0_move, 0);
		console_tex0_move+=0.001f;
		ogl_tm->bind_texture("CONSOLE_TEX0");
		glActiveTextureARB(GL_TEXTURE1_ARB);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glTranslatef(console_tex1_move, 0, 0);
		console_tex1_move+=0.001f;		
		ogl_tm->bind_texture("CONSOLE_TEX1");
		glActiveTextureARB(GL_TEXTURE0_ARB);

		// render the actual console
		glMatrixMode( GL_MODELVIEW ); 
		glLoadIdentity();
		// translate the polygons giving the effect of the console scrolling down
		glTranslatef(0.0,(float)(screen_height + (screen_height/2) - console_open_amount), 0.0);

		glBegin(GL_POLYGON);
			glColor4f(0,0,0,1.0);
	
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0,0);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0,0);
			glVertex2i(0,0);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1,0);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1,0);
			glVertex2i(screen_width,0);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1,1);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1,1);
			glVertex2i(screen_width,screen_height/2);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0,1);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0,1);
			glVertex2i(0,screen_height/2);

		glEnd();

		// disable the scrolling texture effects
		glActiveTextureARB(GL_TEXTURE1_ARB);
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();

		// draw red bar along the bottom of the console
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_POLYGON);
			glColor4f(93.0/256,10.0/256.0,10.0/256.0,1.0);
			glVertex2i(0,0);
			glVertex2i(screen_width,0);
			glVertex2i(screen_width,3);
			glVertex2i(0,3);
		glEnd();

		// draw the console text, also allow for the text to be scrolled up and down
		int x = 5, y = 20;
		if(CConsole::scroll_pos > 0)
			CConsole::scroll_pos = 0;
		if(CConsole::scroll_pos < -CConsole::number_console_lines)
			CConsole::scroll_pos = -CConsole::number_console_lines;

		for(int k=CConsole::number_console_lines+CConsole::scroll_pos;k>=0+CConsole::scroll_pos;k--) {			
			y+=8; // increase based on width/height of font
			if(k >= 0 && k <= CConsole::number_console_lines) {
				int length = strlen(CConsole::console_txt[k]);
				for(int i=0;i<length;i++) {
					glRasterPos2i(x+(8*i),y);
					if(CConsole::console_txt[k][i] != '\n') // do not draw newline character
						glDrawPixels(8,8,GL_RGBA, GL_UNSIGNED_BYTE, console_font->letters[CConsole::console_txt[k][i] ] );
				}
			}
		}

		// draw a ']' indicating input point
		y = 10;
		glRasterPos2i(x,y);
		glDrawPixels(8,8,GL_RGBA, GL_UNSIGNED_BYTE, console_font->letters[']'] );

		// render each letter of the current input
		x = 20;
		for(u_int i=0;i<strlen(CConsole::console_input);i++) {
			glRasterPos2i(x+(8*i),y);
			glDrawPixels(8,8,GL_RGBA, GL_UNSIGNED_BYTE, console_font->letters[CConsole::console_input[i]] );
		}
	}	
}

// --------------------------------------------------------------------------------- 
// This is called by the game unit each frame, it is the outside access point that
// sets up and renders a complete frame.
// --------------------------------------------------------------------------------- 
void COpenGLRend::update(CObject *viewer)
{
	if(viewer != NULL) {
		this->viewer = viewer;
		loc = viewer->get_location();
		ori = viewer->get_orientation();
	}

	draw_scene();

	Message *msg = get_next_message();
	if(msg != NULL) {
		process_message(msg);
	}
}

// --------------------------------------------------------------------------------- 
// add a non-specific particle system to the rendering unit, converting it to an
// OpenGL particle system
// --------------------------------------------------------------------------------- 
int COpenGLRend::add_particle_system(CParticleSystem *ps)
{
	COGLTextureManager *ogl_tm = (COGLTextureManager *) texture_manager;

	if(num_particle_systems == -1)
		return -1; // particle system out of action;

	int count= 0; 
	for(int j=0;j<MAX_PART_SYSTEMS;j++) {
		if(particle_systems[j] != NULL) count++;
	}
	
	if(state_debug_console)
		cons_printf(13, "Number of active psystems = %d\n", count);

	if(count >= MAX_PART_SYSTEMS)
		return -1;

	// check all the spaces for placing particle systems in, when one is
	// found that is free fill it with this new system
	for(int i=0;i<MAX_PART_SYSTEMS;i++) {
		if(particle_systems[i] == NULL) {						
			// copy particle system into OpenGL particle system
			particle_systems[i] = new COGLParticleSystem(ps, ogl_tm);	
			particle_systems[i]->reset_system();		
			return 1;
		}
	}
	cprt("No space for particle system %f, %f, %f\n", ps->system_origin.x, ps->system_origin.y, ps->system_origin.z);

//	particle_systems[num_particle_systems++] = new COGLParticleSystem(ps, ogl_tm);
	return 1;
}

// --------------------------------------------------------------------------------- 
// add an object that needs to be rendered, only the reference to the object is
// stored not a copy of the object structure so that its variables can be altered 
// outside of the renderer and the renderer will also notice the changes
// --------------------------------------------------------------------------------- 
void COpenGLRend::add_object(CObject *obj)
{
	objects.push_back(obj);
	num_objects++;

	// load the model textures if needed
	if(obj->shape != NULL) {
		COGLTextureManager *ogl_tm = (COGLTextureManager *) texture_manager;
		for(int i=0;i<obj->shape->get_num_materials();i++) {
			Material *mat = obj->shape->get_material(i);				
			ogl_tm->add_texture(new CBasicTexture(mat->filename, mat->name));
		}		
	}	
}

// --------------------------------------------------------------------------------- 
// add a sprite to the renderer
// --------------------------------------------------------------------------------- 
void COpenGLRend::add_sprite(CSprite *sprite)
{
	COGLTextureManager *ogl_tm = (COGLTextureManager *) texture_manager;

	sprites.push_back(sprite);

	// store the new sprite texture
	sprite->texture_id = ogl_tm->add_sprite_texture(new CInvisTexture(sprite->tex_filename, sprite->texname, sprite->tex_invis));
}


// --------------------------------------------------------------------------------- 
// This renderer has various states that change its behavious, these can be altered
// outside of the rendering unit, by calls to this function
// --------------------------------------------------------------------------------- 
void COpenGLRend::set_state_var(int state_var, int value)
{
	switch(state_var) {
	case STATE_FILL_TYPE: // draw filled/wireframe polygons
		if(value == STATE_FILL_POLY)
			glPolygonMode(GL_FRONT, GL_FILL);
		if(value == STATE_FILL_LINE)
			glPolygonMode(GL_FRONT, GL_LINE);
		break;
	case STATE_CLIPPING: // collision detection enabled
		if(value == STATE_CLIP_ON)
			state_clipping = STATE_CLIP_ON;
		if(value == STATE_CLIP_OFF)
			state_clipping = STATE_CLIP_OFF;
		break;
	case STATE_PARTICLES_ACTIVE: // particle systems enabled
		if(value == STATE_PARTICLES_ON)
			state_particles_active = 1;
		if(value == STATE_PARTICLES_OFF)
			state_particles_active = 0;
		break;
	case STATE_DEBUG_CONSOLE: // debug console enabled
		if(value == STATE_DEBUG_CONSOLE_ON)
			state_debug_console = STATE_DEBUG_CONSOLE_ON;
		if(value == STATE_DEBUG_CONSOLE_OFF)
			state_debug_console = STATE_DEBUG_CONSOLE_OFF;
		break;
	case STATE_RENDER_BSP: // renderer enabled
		if(value == STATE_RENDER_BSP_ON)
			state_bsp_render = STATE_RENDER_BSP_ON;
		if(value == STATE_RENDER_BSP_OFF)
			state_bsp_render = STATE_RENDER_BSP_OFF;
		break;
	case STATE_DROP_DOWN_CONSOLE: // drop down console renderable
		if(value == STATE_DROP_DOWN_ACTIVE)
			state_drop_down_console = STATE_DROP_DOWN_ACTIVE;
		if(value == STATE_DROP_DOWN_DISABLED)
			state_drop_down_console = STATE_DROP_DOWN_DISABLED;
		break;
	case STATE_LIGHTMAPS_ACTIVE: // render the lightmaps?
		if(value == STATE_LIGHTMAPS_ON)
			state_lightmaps_active = STATE_LIGHTMAPS_ON;
		if(value == STATE_LIGHTMAPS_OFF)
			state_lightmaps_active = STATE_LIGHTMAPS_OFF;
		break;
	case STATE_OLD_STYLE_REND: // PVS or recursive rendering
		if(value == STATE_OLD_STYLE_REND_ON) 
			state_old_style_rend = STATE_OLD_STYLE_REND_ON;
		if(value == STATE_OLD_STYLE_REND_OFF) 
			state_old_style_rend = STATE_OLD_STYLE_REND_OFF;
		break;
	}
}

// END OF FILE --------------------------------------------------------------------------