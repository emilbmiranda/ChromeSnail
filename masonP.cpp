

// Course:   CMPS 3350 Software Engineering
// File: 	 masonP.cpp
// Auhtour:  Mason Pawsey 
// Date:     Spring 2019


// Import required libraries

#include <iostream>
#include "fonts.h"
#include <GL/glx.h>
#include <math.h>

using namespace std;

/*

This function writes my name to the credits screen

*/


void masonP(Rect x, int y)
{
	ggprint8b(&x, y, 0x00ff0000, "Mason Pawsey");
}


/*

This function brings my picutre into the credits screen

*/

void showMasonPicture(int x, int y, GLuint textid)
{
	glColor3ub(255,255,255);
	

	// Manipulate the image position
	static float angle = 0.0f;
	static int wid = 50;
	wid += sin(angle) * 2;
	float fx = (float)x;
	float fy = (float)y;
	float a = 0;
	a += sin(angle) * 5.0f;
	fx += a;
	angle += 0.2f;
	

	glPushMatrix();
	glTranslatef(fx, fy, 0);
	glRotatef(a, 0, 0, 1.0);
	glBindTexture(GL_TEXTURE_2D, textid);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2i(-wid,-wid);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2i(-wid, wid);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2i(wid, wid);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2i(wid, -wid);

	glEnd();
	glPopMatrix();
}

/*

This function renders the helicopter sprite in game

*/

void renderHelicopter(int x, int y, GLuint helicopterID)
{
	// Handle sprite coordinates
	float fx = (float)x;
	float fy = (float)y;
	static int wid = 100;
	glTranslatef(fx, fy, 0);
	glBindTexture(GL_TEXTURE_2D, helicopterID);

	// This removes the black bacground from the image
	// to give us a transparent sprite
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);


	// Add the sprite layer to the screen
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2i(-wid, -wid);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2i(-wid, wid);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2i(wid, wid);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2i(wid, -wid);
	
	glEnd();
	// Remove focus from the helicopter sprite to 
	// allow the screen to render the rest of the layers
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}