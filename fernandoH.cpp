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
	static int wid = 30;
	float fx = (float)x;
	float fy = (float)y;
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
	ggprint8b(&x, 16, 0x00ff0000, "Fernando Herrera");
}
