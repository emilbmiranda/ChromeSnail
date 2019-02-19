// Program: fernandoH.cpp
// CMPS3350
// Author: Fernando
#include <iostream>
#include <GL/glx.h>
#include "fonts.h"
#include <math.h>


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
