// Creator: Victor Merino
// Created Date: 2/14/2019
//
#include <GL/glx.h>
#include <math.h>
#include "fonts.h"

void showVictorText(Rect r, int y) {
	ggprint8b(&r, y, 0xff8866, "Victor Merino");
}

void showVictorPicture(int x, int y, GLuint textid) {
	glColor3ub(255, 255, 255);
	static float angle = 0.0f;
	static int wid = 30;
	float fx = (float)x;
	float fy = (float)y;
	float a = 0;
	a += sin(angle) * 10.0f;
	fx += a;
	fy += a;
	angle += 0.2f;
	glPushMatrix();
	glTranslatef(fx, fy, 0);
	glBindTexture(GL_TEXTURE_2D, textid);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid, -wid);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
		glTexCoord2f(1.0f, 0.0f); glVertex2i(wid, wid);
		glTexCoord2f(1.0f, 1.0f); glVertex2i(wid, -wid);
	glEnd();
	glPopMatrix();
}