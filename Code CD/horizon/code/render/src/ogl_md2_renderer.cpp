/*	--------------------------------------------------------------------------------- 
	Filename      - ogl_md2_renderer.cpp
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Provides the method used to render an .MD2 model using OpenGL
	--------------------------------------------------------------------------------- */

#include <ogl.h>
#include <md2.h>
#include <float.h>

#include <ogl_texturemanager.h>

// --------------------------------------------------------------------------------- 
// render the model at position pos, rotate according to ori
// --------------------------------------------------------------------------------- 
void COpenGLRend::render_md2_model(CMD2 *model, CVert3D pos, CVert3D ori, CTextureManager *in_tm)
{	
	COGLTextureManager *tm = (COGLTextureManager*) in_tm;

	int start = (int)model->cur_frame;
	int end   = start+1;

	if(model->cur_frame > model->max_frame) {
		start = model->max_frame; end = model->min_frame;
	}

	float inter = model->cur_frame-start;

	frame_t *fs = &model->frames[start];
	frame_t *fe = &model->frames[end];

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

		glTranslatef(pos.x, pos.y, pos.z);
		glRotatef(ori.y, 0.0, 1.0, 0.0);
		glRotatef(-90, 1.0, 0.0, 0.0);
		glRotatef(90, 0.0, 0.0, 1.0);	
		glScalef(0.15f, 0.15f, 0.15f);

		tm->bind_texture(model->skinname);
		glEnable(GL_TEXTURE_2D);

	    glFrontFace( GL_CW ); // This way round for MD2 Models
		int count = 0;
		int type = 0;
		while(type != -1) {
			type = model->command_verts_type[count];

			if(type == -1)
				break;

			if(type > 0) {
				glBegin(GL_TRIANGLE_STRIP);
				for(int j=0;j<type;j++) {
					int v0 = model->command_verts[count][j].vertexIndex;

					CVert3D sv0 = fs->verts[v0];
					CVert3D ev0 = fe->verts[v0]; 
					CVert3D p0 = sv0;			

					if(inter != 0) {
						p0 = p0 + ((ev0 - sv0) * inter);
					}
					glTexCoord2f(model->command_verts[count][j].s, model->command_verts[count][j].t);
					glVertex3fv((float*)&p0);	
				}
				glEnd();
			}
			if(type < 0) {
				glBegin(GL_TRIANGLE_FAN);
				for(int j=0;j<abs(type);j++) {
					int v0 = model->command_verts[count][j].vertexIndex;

					CVert3D sv0 = fs->verts[v0];
					CVert3D ev0 = fe->verts[v0]; 
					CVert3D p0 = sv0;			

					if(inter != 0) {
						p0 = p0 + ((ev0 - sv0) * inter);
					}
					glTexCoord2f(model->command_verts[count][j].s, model->command_verts[count][j].t);
					glVertex3fv((float*)&p0);	
				}
				glEnd();
			}

			count++;
		}
		
	glPopMatrix();
    glFrontFace( GL_CCW ); // Renable correct culling for world models
}

// END OF FILE -----------------------------------------------------------------------