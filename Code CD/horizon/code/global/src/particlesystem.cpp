/*	--------------------------------------------------------------------------------- 
	Filename      - particlesystem.cpp
	Last Modified - 09/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Implementation of CParticleSystem
				    Constains all the non-renderer specific code for controlling a 
					particle system
	--------------------------------------------------------------------------------- */

#include <types.h>
#include <ogl.h>
#include <particle.h>
#include <particlesystem.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <stdio.h>
#include <stdlib.h>

// --------------------------------------------------------------------------------- 
// useful function to grab a random number
// --------------------------------------------------------------------------------- 
#define RandFloat ((float)(rand()%1000)/1000.0f)

// --------------------------------------------------------------------------------- 
// constructor
// --------------------------------------------------------------------------------- 
CParticleSystem::CParticleSystem()
{	
   /* Seed the random-number generator with current time so that
    * the numbers will be different every time we run.
    */
   srand( (unsigned)time( NULL ) );
}

// --------------------------------------------------------------------------------- 
// constructor that creates a particle system 
// --------------------------------------------------------------------------------- 
CParticleSystem::CParticleSystem(int num_particles, CVert3D origin, char *pt_filename)
{
	particles = NULL;
//	particles = new CParticle[num_particles];
	system_origin = origin;

	this->num_particles = num_particles;
	
	max_age      = 100;
	max_width    = 0;
	height       = 0;
	acceleration = CVert3D(0,0,0);
	death_time   = -255;
	depth_test   = 1;

	CParse *pt_parser = new CParse();
	pt_parser->open_file(pt_filename);

	pt_parser->read_line();

	while(!pt_parser->cmp("#END")) {
		if(!pt_parser->cmp("#")) {
			set_option(pt_parser);
		}
		pt_parser->read_line();
	}
	delete pt_parser;
}

// --------------------------------------------------------------------------------- 
// destructor
// --------------------------------------------------------------------------------- 
CParticleSystem::~CParticleSystem()
{
	if(particles != NULL)
		delete[] particles;
}

// --------------------------------------------------------------------------------- 
// provides means to set all the various attributes of a particle system
// --------------------------------------------------------------------------------- 
void CParticleSystem::set_option(CParse *p)
{
	char option[80];
	char equals[200];
	float f1, f2, f3;

	sscanf(p->line, "%s %s %f %f %f", option, equals, &f1, &f2, &f3);

	if(strncmp(option, "TYPE", 4) == 0) {
		p->read_string(option);
		if(strncmp(option, "bigtosmall", 10) == 0) {
			bigtosmall = 1;
			agebased = 0;
		} 
		if(strncmp(option, "smalltobig", 10)==0) {
			bigtosmall = 0;
			agebased = 0;
		}
		if(strncmp(option, "agebased", 8)==0) {
			bigtosmall = -1;
			agebased = 1;
		}
	} else if(strncmp(option, "TEXTURE", 7) == 0) {
		char type_str[20];
		int i1, i2, i3;
		sscanf(p->line, "%s %s %s %d %d %d %s", option, texturename, type_str, &i1, &i2, &i3, filename);
		type_color = CVert3D(i1, i2, i3);
		if(strncmp(type_str, "INVIS", 5) == 0) {
			type = 1;
		}
		if(strncmp(type_str, "ALPHA", 5) == 0) {
			type = 2;
		}
	} else if(strncmp(option, "HEIGHT", 6) == 0) {
		height = (int) f1;
	} else if(strncmp(option, "VELOCITY", 8) == 0) {
		velocityx = f1 / 1000.0f;
		velocityy = f2 / 1000.0f;
		velocityz = f3 / 1000.0f;
	} else if(strncmp(option, "WIDTH", 5) == 0) {
		max_width = f1;
	} else if(strncmp(option, "MAXAGE", 6) == 0) {
		max_age = (int)f1;
	} else if(strncmp(option, "DEATHTIME", 9) == 0) {
		death_time = (int)f1;
	} else if(strncmp(option, "ADDVELOCITY", 11) == 0) {
		velocityax = f1 / 1000.0f;
		velocityay = f2 / 1000.0f;
		velocityaz = f3 / 1000.0f;
	} else if(strncmp(option, "ACCELERATION", 12) == 0) {
		acceleration = CVert3D(f1 / 1000.0f, f2 / 1000.0f, f3 / 1000.0f);
	} else if(strncmp(option, "SUBVELOCITY", 11) == 0) {
		velocitysx = f1 / 1000;
		velocitysy = f2 / 1000;
		velocitysz = f3 / 1000;
	} else if(strncmp(option, "SRCBLEND", 8) == 0) {
		p->read_string(option);
		if(strncmp(option, "GL_ONE", 6) == 0) {
			src_blend = GL_ONE;
		} else
		if(strncmp(option, "GL_DST_COLOR", 12) == 0) {
			src_blend = GL_DST_COLOR;
		} else
		if(strncmp(option, "GL_ZERO", 7) == 0) {
			src_blend = GL_ZERO;
		} 
		if(strncmp(option, "GL_SRC_ALPHA", 12) == 0) {
			src_blend = GL_SRC_ALPHA;
		} 
		if(strncmp(option, "GL_ONE_MINUS_SRC_ALPHA", 22) == 0) {
			src_blend = GL_ONE_MINUS_SRC_ALPHA;
		} 

	} else if(strncmp(option, "DSTBLEND", 8) == 0) {
		p->read_string(option);
		if(strncmp(option, "GL_ONE", 6) == 0) {
			dst_blend = GL_ONE;
		} else
		if(strncmp(option, "GL_DST_COLOR", 12) == 0) {
			dst_blend = GL_DST_COLOR;
		} else
		if(strncmp(option, "GL_ZERO", 7) == 0) {
			dst_blend = GL_ZERO;
		} 
		if(strncmp(option, "GL_SRC_ALPHA", 12) == 0) {
			dst_blend = GL_SRC_ALPHA;
		} 
		if(strncmp(option, "GL_ONE_MINUS_SRC_ALPHA", 22) == 0) {
			dst_blend = GL_ONE_MINUS_SRC_ALPHA;
		} 

	} else if(strncmp(option, "DEPTHTEST", 9)==0) {
		if(strncmp(equals, "ON", 2)==0)
			depth_test = 1;
		if(strncmp(equals, "OFF", 3)==0)
			depth_test = 0;
	}
}

// --------------------------------------------------------------------------------- 
// reset all the particle systems particles attributes after it has died
// --------------------------------------------------------------------------------- 
void CParticleSystem::reset_system()
{
	for(int i=0;i<num_particles;i++) {
		set_particle_defaults(i);
	}
}

// --------------------------------------------------------------------------------- 
// reset an individual particles attributes
// --------------------------------------------------------------------------------- 
void CParticleSystem::set_particle_defaults(int i)
{
	CParticle *p = &particles[i];

	p->alive  = 1;
	p->age     = 0;
	p->max_age = (int)(RandFloat * (float)max_age);	

	p->velocity = CVert3D(velocityax + (RandFloat*velocityx) - velocitysx, 
		                  velocityay + (RandFloat*velocityy) - velocitysy, 
						  velocityaz + (RandFloat*velocityz) - velocitysz);

	while(p->velocity.x == 0 && p->velocity.y==0 && p->velocity.z==0) {
		p->velocity = CVert3D(velocityax + (RandFloat*velocityx) - velocitysx, 
			                  velocityay + (RandFloat*velocityy) - velocitysy, 
							  velocityaz + (RandFloat*velocityz) - velocitysz);
	}

	p->acceleration = acceleration;// * RandFloat;	
	p->position = system_origin + (p->velocity * (float)i) * RandFloat;



}

// --------------------------------------------------------------------------------- 
// Life of a particle, it moves and changes shape
// --------------------------------------------------------------------------------- 
int CParticleSystem::update_system(float time_passed)
{
	CParticle *p;

	if(death_time != -255) {
		if(death_time-- <= 0)
			return -1;
	}

	for(int i=0;i<num_particles;i++) {

		p = &particles[i];

		if(p->age++ >= p->max_age) {
			set_particle_defaults(i);
		}
		
		p->velocity.x += (p->acceleration.x);
		p->velocity.y += (p->acceleration.y);
		p->velocity.z += (p->acceleration.z);

		p->position = p->position + (p->velocity * time_passed);

		if(p->position.y > system_origin.y + height) {
			set_particle_defaults(i);
		}

		
		if(bigtosmall == 1)
			p->size = max_width - (max_width / ((float)height)*(p->position.y-system_origin.y));
		if(bigtosmall == 0)
			p->size = max_width / ((float)height)*(p->position.y-system_origin.y);


		if(bigtosmall == 0 || bigtosmall == 1) {
			if(p->acceleration.y < 0) {
				if(p->size >= max_width) {
					set_particle_defaults(i);//p->size = max_width;
				}

				if(p->size <= 0) {
					set_particle_defaults(i);//p->size = 0;
					
				}
			}
		}

		if(agebased == 1) {
			float div2 = max_width / max_age;
			p->size = div2 * p->age;
		}

	}
	
	return 1;
}

// END OF FILE --------------------------------------------------------------------------
