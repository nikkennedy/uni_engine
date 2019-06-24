/*	--------------------------------------------------------------------------------- 
	Filename      - image.cpp
	Last Modified - 20/04/03
	Author        - Nicholas Kennedy
	Project       - Horizon (3rd Year BSC Computer Science Project "Interactive Game 
			        system with Intelligence")
	Description   - Implementation of CBitMap and CFont and COGLFont
	                CBitMap deals with loading a .BMP file and providing methods 
					that return the bitmap data in various ways
					CFont is the superclass of all implementation of fonts.
					COGLFont is the OpenGL implementation of font handling
	--------------------------------------------------------------------------------- */

#include <windows.h>
#include <stdio.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include <image.h>
#include <types.h>
#include <debug.h>

// --------------------------------------------------------------------------------- 
// open up the .BMP file filename, and store the raw image data 
// --------------------------------------------------------------------------------- 
int CBitMap::read_image(char *filename) 
{
	// windows function call that deals with loading a .BMP file
	image_data = (HBITMAP) LoadImage(NULL, filename, 
		IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);

	if(image_data == NULL) {
		char strbuffer[200];
		sprintf(strbuffer, "Could not allocate memory for bitmap (%s)\n", filename);
        MessageBox(NULL, strbuffer, "Error", MB_OK); 
		dbp("can not read image file %s\n", filename);
		exit(1);
	}

	// just need the image data, i.e. red, green, blue values
	GetObject(image_data, sizeof(BITMAP), &bitmap);

	// store height and width
	height = bitmap.bmHeight;
	width = bitmap.bmWidth;

	return 1;
}

// --------------------------------------------------------------------------------- 
// parse the bitmap data, and return an allocated array with red, green and blue 
// values correctly ordered.
// --------------------------------------------------------------------------------- 
u_char *CBitMap::get_RGB_data()
{
	u_char *p = (u_char *)bitmap.bmBits;
	u_char r,g,b;
	long j = 0, x, y;

	// no need to recalculate the values 
	if(rgb_data != NULL) return rgb_data;

	// allocate memory for bitmap
	rgb_data = (u_char *) malloc(sizeof(unsigned char) * height * width * 3);

	if(rgb_data == NULL) {
        MessageBox(NULL, "Coult not allocate memory for bitmap.", "Error", MB_OK); 
		dbp("Unable to allocate memory for rgb image\n");	
		exit(1);
	}

	for(x=0;x<width;x++) {
		for(y=0;y<height;y++) {
			b = *p++;	g = *p++;	r = *p++;
			rgb_data[j++] = r;
			rgb_data[j++] = g;
			rgb_data[j++] = b;
		}
	}

	return rgb_data;
}

// --------------------------------------------------------------------------------- 
// parses the bitmap and returns red, green, blue and alpha values. the alpha value 
// is equal to 0 when the red component is equal to that of invisible otherwise 
// the alpha value equals 255 (1.0f)
// --------------------------------------------------------------------------------- 
u_char *CBitMap::get_RGBA_data(int invisible)
{
	u_char *p = (u_char *)bitmap.bmBits;
	u_char r,g,b,a;
	long j = 0, x, y;

	// no need to recalculate the values 
	if(rgba_data != NULL) return rgba_data;

	rgba_data = (u_char *) malloc(sizeof(unsigned char) * height * width * 4);

	if(rgba_data == NULL) {
        MessageBox(NULL, "Coult not allocate memory for bitmap.", "Error", MB_OK); 
		dbp("Unable to allocate memory for rgba image\n");	
		exit(1);
	}

	for(x=0;x<width;x++) {
		for(y=0;y<height;y++) {
			b = *p++;	g = *p++;	r = *p++;	a = 255;
			if(invisible && r == rgba_data[0]) { a = 0; }
			rgba_data[j++] = r;
			rgba_data[j++] = g;
			rgba_data[j++] = b;
			rgba_data[j++] = a;
		}
	}

	return rgba_data;
}

// --------------------------------------------------------------------------------- 
// red, green and blue components are equal to color.x, color.y and color.z
// alpha value is equal to average of the bitmaps r, g, b components
// --------------------------------------------------------------------------------- 
u_char *CBitMap::get_alpha_data(CVert3D color)
{
	u_char *p = (u_char *)bitmap.bmBits;
	u_char r,g,b;
	long j = 0, x, y;

	// no need to recalculate the values 
	if(rgba_data != NULL) return rgba_data;

	rgba_data = (u_char *) malloc(sizeof(unsigned char) * height * width * 4);

	if(rgba_data == NULL) {
        MessageBox(NULL, "Coult not allocate memory for bitmap.", "Error", MB_OK); 
		dbp("Unable to allocate memory for rgba image\n");	
		exit(1);
	}

	for(x=0;x<width;x++) {
		for(y=0;y<height;y++) {
			b = *p++;	g = *p++;	r = *p++;	
			
			rgba_data[j++] = (int) color.x;
			rgba_data[j++] = (int) color.y;
			rgba_data[j++] = (int) color.z;
			rgba_data[j++] = (r + g + b) / 3;
		}
	}

	return rgba_data;

}

// --------------------------------------------------------------------------------- 
// This grabs RGBA data alpha is set to 255 for all colors except those which have
// rgb triple less than those specified in invis_color
// --------------------------------------------------------------------------------- 
u_char *CBitMap::get_RGBA_data(CVert3D invis_color)
{
	u_char *p = (u_char *)bitmap.bmBits;
	u_char r,g,b,a;
	long j = 0, x, y;

	// no need to recalculate the values 
	if(rgba_data != NULL) return rgba_data;

	rgba_data = (u_char *) malloc(sizeof(unsigned char) * height * width * 4);

	if(rgba_data == NULL) {
        MessageBox(NULL, "Coult not allocate memory for bitmap.", "Error", MB_OK); 
		dbp("Unable to allocate memory for rgba image\n");	
		exit(1);
	}

	for(x=0;x<width;x++) {
		for(y=0;y<height;y++) {
			b = *p++;	g = *p++;	r = *p++;	a = 255;
			
			if(r <= (int)invis_color.x && g <= (int) invis_color.y && b <= (int)invis_color.z) {
				a = 0; 
			}

			rgba_data[j++] = r;
			rgba_data[j++] = g;
			rgba_data[j++] = b;
			rgba_data[j++] = a;
		}
	}

	return rgba_data;
}

// --------------------------------------------------------------------------------- 
// destructor, free up memory
// --------------------------------------------------------------------------------- 
CBitMap::~CBitMap()
{
	if(rgba_data != NULL)
		free(rgba_data);
	if(rgb_data != NULL)
		free(rgb_data);
}


// --------------------------------------------------------------------------------- 
// non-rending API specific font constructor
// --------------------------------------------------------------------------------- 
CFont::CFont(char *filename)
{
	// read in the specified font .BMP file
	if(!read_image(filename)) return;
}

// --------------------------------------------------------------------------------- 
// OpenGL API font constructor
// --------------------------------------------------------------------------------- 
COGLFont::COGLFont(char *filename) : CFont(filename) 
{
	get_RGBA_data(USE_INVISIBLE);
	create_font();
}

// --------------------------------------------------------------------------------- 
// This takes the 8x8 letter in the 128x128 texture representing the whole font
// at start position x,y and stores it in the letters array at index letternum
// --------------------------------------------------------------------------------- 
void COGLFont::grab_letter(int letternum, int x, int y)
{
	u_char *p = rgba_data;
	u_char letter[8][8][4];

	for(int i=0;i<128;i++) {
		for(int j=0;j<128;j++) {
			if(j < (x+1) * 8 && j >= (x*8) && i < (y+1) * 8 && i >= (y*8)) {
				letter[i%8][j%8][0] = *p++;
				letter[i%8][j%8][1] = *p++;
				letter[i%8][j%8][2] = *p++;
				letter[i%8][j%8][3] = *p++;
			} else {
				*p++; *p++; *p++; *p++; // skip the rest
			}
		}
	}
	memcpy(&letters[letternum], letter, 8*8*4);
}

// --------------------------------------------------------------------------------- 
// grabs all the relevant letters needed from the 128x128 font image
// --------------------------------------------------------------------------------- 
void COGLFont::create_font(void)
{
	int i;

	for(i=' ';i<='/';i++) {
		grab_letter(i, i-' ', 5);
	}

	for(i='0';i<='?';i++) {
		grab_letter(i, i-'0', 4);
	}

	for(i='@';i<='O';i++) {
		grab_letter(i, i-'@', 3);
	}

	for(i='P';i<='_';i++) {
		grab_letter(i, i-'P', 2);
	}

	for(i='`';i<='o';i++) {
		grab_letter(i, i-'`', 1);
	}

	for(i='p';i<='~';i++) {
		grab_letter(i, i-'p', 0);
	}	
	
}

// --------------------------------------------------------------------------------- 
// draws the string str at location x,y
// --------------------------------------------------------------------------------- 
void COGLFont::draw_string(char *str, float x, float y)
{
	int length = strlen(str);
	
	// draw text
	for(int i=0;i<length;i++) {
		glRasterPos2f(x+(8*i),y);
		glDrawPixels(8,8,GL_RGBA, GL_UNSIGNED_BYTE, letters[str[i]]);
	}

}

// END OF FILE ----------------------------------------------------------------------