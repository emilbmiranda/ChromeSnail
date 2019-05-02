// Program: fernandoH.cpp
// CMPS3350
// Author: Fernando

#include <iostream>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include "fonts.h"
#include <math.h>
#include "fernandoH.h"

typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];

#define MakeVector(v, x, y, z) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)


void showFernandoPicture(int x, int y, GLuint textid) 
{
	glColor3ub(255,255,255);
	static float angle = 0.0f;
	// wid determines thumbnail size.
	static int wid = 40;
	float fx = (float)x;
	float fy = (float)y;
	float a = 0;
	float b = 0;
	a += sin(angle) * 20.0f;
	b += cos(angle) * 20.0f;
	fx += a;
	fy += b;
	angle += 0.2f;
	glPushMatrix();
	glTranslatef(fx,fy,0);
	glBindTexture(GL_TEXTURE_2D, textid);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid,wid);
		glTexCoord2f(1.0f, 1.0f); glVertex2i(wid,wid);
		glTexCoord2f(1.0f, 1.0f); glVertex2i(wid,-wid);
	glEnd();
	glPopMatrix();
}

void showFHText(Rect x)
{
	ggprint8b(&x, 16, 0xff3333, "Fernando Herrera");
}

Platform::Platform()
{
	pos[0] = 540;
	pos[1] = 140;
}

Platform::Platform(int x, int y) 
{
	pos[0] = x;
	pos[1] = y;
}
//void Platform::drawPlatform()
void Platform::drawPlatform(int xpos, int ypos, GLuint crateID)
{
	// Change the size of the sprite.
	static int wid = 50;
	float fx = (float)xpos;
	float fy = (float)ypos;

	//glClear(GL_COLOR_BUFFER_BIT);
	//glPushMatrix();
	glTranslatef(fx,fy,0);
	glBindTexture(GL_TEXTURE_2D, crateID);

	// Transparent sprite needed?


		//	(0,140)			(180,140)
		//	(0,0)			(180,0)
		/*
		glVertex2f(0.0f,140.0f);
		glVertex2f(180.0f,140.0f);
		glVertex2f(180.0f,0.0f);
		glVertex2f(0.0f,0.0f);
		*/

	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2i(-wid, -wid);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2i(-wid, wid);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2i(wid, wid);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2i(wid, -wid);
	glEnd();
	// This is DEFINITELY needed.
	glBindTexture(GL_TEXTURE_2D, 0);
}

int Platform::getXpos()
{
	return pos[0];

}

int Platform::getYpos()
{
	return pos[1];
}