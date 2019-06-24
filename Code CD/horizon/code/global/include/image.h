/*	--------------------------------------------------------------------------------- 
	Filename      - image.h
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Definition of CBitMap and CFont and COGLFont
	                CBitMap deals with loading a .BMP file and providing methods 
					that return the bitmap data in various ways
					CFont is the superclass of all implementation of fonts.
					COGLFont is the OpenGL implementation of font handling
	--------------------------------------------------------------------------------- */

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <windows.h>
#include <types.h>

#define IGNORE_INVISIBLE 0
#define USE_INVISIBLE    1

// --------------------------------------------------------------------------------- 
// CBitMap
// --------------------------------------------------------------------------------- 
class CBitMap {
protected:	
	HBITMAP image_data;
	BITMAP bitmap;
	int width;
	int height;
	unsigned char *rgba_data;
	unsigned char *rgb_data;
	Color invisible_color; // set this if you want an invisible color

public:
	CBitMap() 
	{ 
		rgba_data = NULL; 
		rgb_data= NULL;
	}
	
	~CBitMap();
	
	int read_image(char *filename);
	u_char *get_RGBA_data(int invisible);
	u_char *get_RGB_data();
	u_char *get_RGBA_data(CVert3D invis_color);
	u_char *get_alpha_data(CVert3D color);

	int get_height() 
	{ 
		return height; 
	}
	int get_width() 
	{ 
		return width; 
	}
};

// --------------------------------------------------------------------------------- 
// CFont which is a sub class of CBitMap
// --------------------------------------------------------------------------------- 
class CFont : public CBitMap {
public:
	u_char letters[255][8][8][4];
	virtual void create_font() = 0;
	virtual void grab_letter(int, int, int) = 0;
	CFont(char *filename);
	virtual void draw_string(char *str, float x, float y) = 0;
};

// --------------------------------------------------------------------------------- 
// OpenGL Specific Definition of CFont
// --------------------------------------------------------------------------------- 
class COGLFont : public CFont {
public:
	void create_font();
	void grab_letter(int, int, int);
	COGLFont(char *filename);
	void draw_string(char *str, float x, float y);
};

#endif

// END OF FILE ---------------------------------------------------------------------