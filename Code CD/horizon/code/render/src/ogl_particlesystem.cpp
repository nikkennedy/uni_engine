/*	--------------------------------------------------------------------------------- 
	Filename      - ogl_particlesystem.cpp
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Implementation of COGLParticleSystem
				    Constains all the OpenGL-renderer specific code for rendering a 
					particle system
	--------------------------------------------------------------------------------- */

#include <types.h>
#include <ogl.h>
#include <particle.h>
#include <particlesystem.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glext.h>
#include <glh_extensions.h>
#include <invistexture.h>
#include <alphatexture.h>
#include <ogl_particlesystem.h>

// --------------------------------------------------------------------------------- 
// destructor
// --------------------------------------------------------------------------------- 
COGLParticleSystem::~COGLParticleSystem()
{
	if(particles != NULL) {
		delete[] particles;
		particles = NULL;
	}
	// remove all the particles
//	for(int i=0;i<this->num_particles;i++) {
//		delete &particles[i];
//	}
}

// --------------------------------------------------------------------------------- 
// Copy constructor, used to create an OpenGL particle system from a plain non-
// specific CParticleSystem, requires the OGL TextureManager to allow for the 
// particle texture to be added.
// --------------------------------------------------------------------------------- 
COGLParticleSystem::COGLParticleSystem(CParticleSystem *ps, COGLTextureManager *tm)
{
	// now u've got the texturemanager its time to add particles texture
	strcpy(this->texturename, ps->texturename);
	strcpy(this->filename, ps->filename);	

	this->tm = tm;
	this->type = ps->type;
	this->type_color = ps->type_color;

	if(type == 1) { // Invisible Texture Type
		if(tm->bind_texture(texturename) == -1)
			tm->add_texture(new CInvisTexture(filename, texturename, type_color));
	}
	if(type == 2) { // Alpha Texture Type
		if(tm->bind_texture(texturename) == -1)
			tm->add_texture(new CAlphaTexture(filename, texturename, type_color));
	}

	// copy all the values
	this->acceleration = ps->acceleration;
	this->bigtosmall = ps->bigtosmall;
	this->agebased = ps->agebased;
	this->death_time = ps->death_time;
	this->dst_blend = ps->dst_blend;
	this->height = ps->height;
	this->max_age = ps->max_age;
	this->max_width = ps->max_width;
	this->num_particles = ps->num_particles;
	this->particles = new CParticle[num_particles];	
	this->src_blend = ps->src_blend;
	this->system_origin = ps->system_origin;
	this->velocityax = ps->velocityax;
	this->velocityay = ps->velocityay;
	this->velocityaz = ps->velocityaz;
	this->velocityx = ps->velocityx;
	this->velocityy = ps->velocityy;
	this->velocityz = ps->velocityz;
	this->velocitysx = ps->velocitysx;
	this->velocitysy = ps->velocitysy;
	this->velocitysz = ps->velocitysz;
	this->depth_test = ps->depth_test;
}

// --------------------------------------------------------------------------------- 
// Provide the function to render the particle system using OpenGL, the method for
// displaying the particles is BillBoarding and requires the transpose of the 
// modelview matrix to make create polygons that always face the users.
// --------------------------------------------------------------------------------- 
void COGLParticleSystem::render()
{
	CParticle *p;

	matrix4x4 mat;
	matrix4x4 transpose;
	
	glGetFloatv(GL_MODELVIEW_MATRIX, (float*)mat);
	matrix_transpose(mat, transpose);
	CVert3D x = CVert3D(transpose[0][0], transpose[0][1], transpose[0][2]);
	CVert3D y = CVert3D(transpose[1][0], transpose[1][1], transpose[1][2]);
	glPushMatrix();

	glColor4f(1.0,1.0,1.0,0.0);

	glBlendFunc(src_blend, dst_blend);
	glEnable(GL_BLEND);
	
	if(!depth_test)
		glDepthMask(GL_FALSE);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	tm->bind_texture(texturename);

	for(int i=0;i<num_particles;i++) {
		p = &particles[i];

		CVert3D a = p->position + (-x - y) * p->size;
		CVert3D b = p->position + ( x - y) * p->size;
		CVert3D c = p->position + ( x + y) * p->size;
		CVert3D d = p->position + (-x + y) * p->size;


		glBegin(GL_QUADS);
			glTexCoord2f(0,0);
			glVertex3fv((float*)&a);
			glTexCoord2f(1,0);
			glVertex3fv((float*)&b);
			glTexCoord2f(1,1);
			glVertex3fv((float*)&c);
			glTexCoord2f(0,1);
			glVertex3fv((float*)&d);
		glEnd();
	}

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
	if(!depth_test)
		glDepthMask(GL_TRUE);

	glPopMatrix();
}

// END OF FILE ------------------------------------------------------------------------------