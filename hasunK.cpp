//3350
//program: hasunK.cpp
//author:  Hasun Khan 
//date:    Spring 2019
//
//Display my picture and write my name as a part of the credits 
#include <GL/glx.h>
#include "fonts.h"
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];


class Texture {
public:
	float xc[2];
	float yc[2];
};

class Global {
public:
	bool GoEvent;
	Texture tex;
}g;

//Prints my name to the Screen
void printHasunName(Rect x, int y)
{
	ggprint8b(&x, y, 0x00ff5900, "Hasun Khan");
}

//Displays an image to the screen
void showHasunPicture(int x, int y, GLuint textid)
{
	glColor3ub(255,255,255);
	static float angle = 0.0f;
	static int w = 30; 
	w +=sin(angle)*10;
	float fx = (float)x;
	float fy = (float)y;
	float a = 0; 
	//a += sin(angle)*10.0f;
	fx += a;
	angle += 0.2f;

	glPushMatrix(); 
	glTranslatef(fx,fy,0);
	glRotatef(a, 0, 0,1.0);
	glBindTexture(GL_TEXTURE_2D, textid);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,1.0f); 
		glVertex2i(-w,-w);
		glTexCoord2f(0.0f,0.0f); 
		glVertex2i(-w,w);
		glTexCoord2f(1.0f,0.0f); 
		glVertex2i(w,w);
		glTexCoord2f(1.0f,1.0f); 
		glVertex2i(w,-w);
	glEnd();
	glPopMatrix();
}

/*
void initBG(int xres, int yres, GLuint *backgroundTexture, unsigned char *data, float xc[2], float yc[2])
{
	glGenTextures(1, backgroundTexture);
	int bxres = xres;
	int byres = yres;
	glBindTexture(GL_TEXTURE_2D, *backgroundTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, bxres, byres, 0,
		GL_RGB, GL_UNSIGNED_BYTE, data);
	xc[0] = 0.0;
	xc[1] = 0.25;
	yc[0] = 0.0;
	yc[1] = 1.0;
}
*/
/*
void renderBackground(int xres, int yres, GLuint backgroundTexture, float xc[2], float yc[2])
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, backgroundTexture);
	glBegin(GL_QUADS);
		glTexCoord2f(xc[0], yc[1]); 
		glVertex2i(0, 0);
		glTexCoord2f(xc[0], yc[0]); 
		glVertex2i(0, yres);
		glTexCoord2f(xc[1], yc[0]); 
		glVertex2i(xres, yres);
		glTexCoord2f(xc[1], yc[1]); 
		glVertex2i(xres, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}
*/

