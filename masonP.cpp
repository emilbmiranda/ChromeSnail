//3350
//program: masonP.cpp
//author:  Mason Pawsey 
//date:    Spring 2019
//
// Display my picture and write my name as a part of the credits 
#include <iostream>
#include "fonts.h"
#include <GL/glx.h>
#include <math.h>

void masonP(Rect x, int y)
{
  ggprint8b(&x, y, 0x00ff0000, "Mason Pawsey");
}

void showMasonPicture(int x, int y, GLuint textid)
{
	glColor3ub(255,255,255);
	static float angle = 0.0f;
	static int wid = 30;
	//wid += sin(angle) * 10;
	float fx = (float)x;
	float fy = (float)y;
	//float a = 0;
	//a += sin(angle) * 10.0f;
	//fx += a;
	//angle += 0.2f;
	glPushMatrix();
	glTranslatef(fx,fy,0);
	//glRotatef(a, 0, 0, 1.0);
	glBindTexture(GL_TEXTURE_2D, textid);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
	glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
	glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);

	glEnd();
	glPopMatrix();
}
