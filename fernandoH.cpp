// Program: fernandoH.cpp
// CMPS3350
// Author: Fernando

#include <iostream>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include "fonts.h"
#include <math.h>
#include "fernandoH.h"

/*----------------------------------------------------------------Platform---*/
Platform::Platform()
{
	//pos[0] = 740;
	pos[0] = 1040;
	pos[1] = 300;
}

Platform::Platform(int x, int y) 
{
	pos[0] = x;
	pos[1] = y;
}
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

void Platform::slidePlatformBackward() 
{
	pos[0] -= vel;
}
void Platform::slidePlatformForward() 
{
	pos[0] += vel;
}
/*------------------------------------------------------------Platform-END---*/

/*------------------------------------------------------------Credits-Page---*/
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
/*--------------------------------------------------------Credits-Page-END---*/