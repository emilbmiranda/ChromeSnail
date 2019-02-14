//3350
//program: hasunK.cpp
//author:  Hasun Khan 
//date:    Spring 2019
//
// Display my picture and write my name as a part of the credits 
#include <iostream>
#include <GL/glx.h>
#include <fonts.h>
#include <math.h>



//Prints my name to the Screen
void printHasunName(Rect x, int y) {
	ggprint8b(8x, 16, 0x00ff0000, "Hasun Khan");
}


//Displays an image to the screen
void showHasunPicture(int x, int y, Gluint textid) {
 
	glColor3ub(1,1,1);
	static float angle = 0.0f;
	static int w = 30; 
	w +=sin(angle)*10;
	float fx = (float)x;
	float fy = (float)y;
	float a = 0; 
	a += sin(angle)*10.0f;
	fx += a;
	angle += 0.2f;
	glPushMatrix(); 
	glTranslatef(fx,fy,0);
	glRotatef(a, 0, 0,1.0);
	glBindTexture(GL_TEXTURE_2D, textid);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,1.0f); glVertex2i(-w,-w);
		glTexCoord2f(0.0f,0.0f); glVertex2i(-w,w);
		glTexCoord2f(1.0f,0.0f); glVertex2i(w,w);
		glTexCoord2f(1.0f,1.0f); glVertex2i(w,-w);
	glEnd();
	glPopMatrix();
} 



