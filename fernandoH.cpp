// Program: fernandoH.cpp
// CMPS3350
// Author: Fernando

#include <iostream>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include "fonts.h"
#include <math.h>
#include "fernandoH.h"

/*-------------------------------------------------------------myFunctions---*/
int preventPlayerProgress(int objectX)
{
	return objectX - 15;

}
/*----------------------------------------------------------myFunctions-END--*/


/*----------------------------------------------------------------Cover------*/
Cover::Cover()
{
	pos[0] = 1040;
	pos[1] = 340;
}

Cover::Cover(int x, int y) 
{
	pos[0] = x;
	pos[1] = y;
}
void Cover::drawCover(int xpos, int ypos, GLuint coverID)
{
	// Change the size of the sprite.
	static int wid = 80;
	float fx = (float)xpos;
	float fy = (float)ypos;

	//glClear(GL_COLOR_BUFFER_BIT);
	//glPushMatrix();
	glTranslatef(fx,fy,0);
	glBindTexture(GL_TEXTURE_2D, coverID);


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

int Cover::getXpos()
{
	return pos[0];

}

int Cover::getYpos()
{
	return pos[1];
}

void Cover::slideCoverBackward() 
{
	pos[0] -= vel;
}
void Cover::slideCoverForward() 
{
	pos[0] += vel;
}
/*----------------------------------------------------------------Cover-END--*/

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
void Platform::drawPlatform(int xpos, int ypos, GLuint platID)
{
	// Change the size of the sprite.
	static int wid = 50;
	float fx = (float)xpos;
	float fy = (float)ypos;

	//glClear(GL_COLOR_BUFFER_BIT);
	//glPushMatrix();
	glTranslatef(fx,fy,0);
	glBindTexture(GL_TEXTURE_2D, platID);


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

/*----------------------------------------------------------------Wall---*/
Wall::Wall()
{
	//pos[0] = 740;
	pos[0] = 1040;
	pos[1] = 300;
}

Wall::Wall(int x, int y) 
{
	pos[0] = x;
	pos[1] = y;
}
void Wall::drawWall(int xpos, int ypos, GLuint crateID)
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

int Wall::getXpos()
{
	return pos[0];

}

int Wall::getYpos()
{
	return pos[1];
}

void Wall::slideWallBackward() 
{
	pos[0] -= vel;
}
void Wall::slideWallForward() 
{
	pos[0] += vel;
}
/*------------------------------------------------------------Wall-END---*/

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