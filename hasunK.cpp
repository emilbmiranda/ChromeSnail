//3350
//program: hasunK.cpp
//author:  Hasun Khan 
//date:    Spring 2019
//
//Display my picture and write my name as a part of the credits 
#include <GL/glx.h>
#include "fonts.h"
#include <math.h>

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
	float a = 1; 
	//a += sin(angle)*10.0f;
	//fx += a;
	angle += 0.2f;
	glPushMatrix(); 
	glTranslatef(fx,fy,0);
	glRotatef(a, 0, 0,1.0);
	glBindTexture(GL_TEXTURE_2D, textid);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f,1.0f); glVertex2i(-w,-w);
		glTexCoord2f(1.0f,0.0f); glVertex2i(-w,w);
		glTexCoord2f(1.0f,0.0f); glVertex2i(w,w);
		glTexCoord2f(1.0f,1.0f); glVertex2i(w,-w);
	glEnd();
	glPopMatrix();
}
/*
void initBG(int xres, int yres, GLuint backgroundTexture, Image *bgImage){
	glGenTextures(1, &backgroundTexture);
	int bxres = xres;
	int byres = yres;
	glBindTexture(GL_TEXTURE_2D, backgroundTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, bxres, byres, 0,
	//	GL_RGB, GL_UNSIGNED_BYTE, bgImage->data);
	g.tex.xc[0] = 0.0;
	g.tex.xc[1] = 0.25;
	g.tex.yc[0] = 0.0;
	g.tex.yc[1] = 1.0;
}
*/
void renderBackground(int xres, int yres, GLuint backgroundTexture)
{

	//static int wid = xres/5;
	float fx = (float)xres;
	float fy = (float)yres-300;
	glPushMatrix();
	glTranslatef(fx/2,fy/2,0);
	glBindTexture(GL_TEXTURE_2D, backgroundTexture);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); 
		glVertex2i(0, 0);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex2i(0, yres);
		glTexCoord2f(1.0f, 0.0f); 
		glVertex2i(xres, yres);
		glTexCoord2f(1.0f, 1.0f); 
		glVertex2i(xres, 0);
	glEnd();

/*
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2i(-wid,-wid);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2i(-wid, wid);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2i( wid, wid);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2i( wid,-wid);
	glEnd();
*/
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();


	/*
	static int wid = xres/5;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, backgroundTexture);
	glBegin(GL_QUADS);
		glTexCoord2f(g.tex.xc[0], g.tex.yc[1]); glVertex2i(0, 0);
		glTexCoord2f(g.tex.xc[0], g.tex.yc[0]); glVertex2i(0, yres);
		glTexCoord2f(g.tex.xc[1], g.tex.yc[0]); glVertex2i(xres, yres);
		glTexCoord2f(g.tex.xc[1], g.tex.yc[1]); glVertex2i(xres, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
*/
}
/*
	int bxres = Global::getInstance().xres;
	int byres = Global::getInstance().yres;
	static int wid = bxres/5;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, Global::getInstance().backgroundTexture);
	glBegin(GL_QUADS);
		glTexCoord2f(Global::getInstance().xc[0], Global::getInstance().yc[1]); glVertex2i(0, 0);
		glTexCoord2f(Global::getInstance().xc[0], Global::getInstance().yc[0]); glVertex2i(0, byres);
		glTexCoord2f(Global::getInstance().xc[1], Global::getInstance().yc[0]); glVertex2i(bxres, byres);
		glTexCoord2f(Global::getInstance().xc[1], Global::getInstance().yc[1]); glVertex2i(bxres, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

*/

