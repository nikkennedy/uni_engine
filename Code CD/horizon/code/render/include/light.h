#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <types.h>
#include <vert3d.h>

class CLight {
public:
	CVert3D pos;

	CLight() {}
	CLight(float x, float y, float z) { pos = CVert3D(x, y, z); }

	virtual void begin_alpha_diffuse_intensity() = 0;
	virtual void process_alpha_diffuse_intensity(CVert3D, CVert3D) = 0;
	virtual void end_alpha_diffuse_intensity() = 0;
	
	virtual void begin_diffuse() = 0;
	virtual void process_diffuse_vertex(CVert3D vertex, CVert3D tvert) = 0;
	virtual void end_diffuse() = 0;
};

class CSpotLight : public CLight
{
public:
	u_char *attenuation_map_image;
	u_int  attenuation_map_id;
	
	u_char *cube_px_image; // the cube map image data
	u_char *cube_nx_image;
	u_char *cube_py_image;
	u_char *cube_ny_image;
	u_char *cube_pz_image;
	u_char *cube_nz_image;
	u_int  cube_map_texture_id;
	
	float   angle;
	CVert3D color;
	CVert3D orientation;

	CSpotLight();

	void begin_alpha_diffuse_intensity();
	void process_alpha_diffuse_intensity(CVert3D, CVert3D);
	void end_alpha_diffuse_intensity();

	void begin_diffuse();
	void process_diffuse_vertex(CVert3D vertex, CVert3D tvert);
	void end_diffuse();
	
};

#endif