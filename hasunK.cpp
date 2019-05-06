//3350
//program: hasunK.cpp
//author:  Hasun Khan 
//date:    Spring 2019
//
//Display my picture and write my name as a part of the credits 
#include <GL/glx.h>
#include "fonts.h"
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

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

class Image {
public:
	int width, height;
	unsigned char *data;
	~Image() { delete [] data; }
	Image(const char *fname) {
		if (fname[0] == '\0')
			return;
		//printf("fname **%s**\n", fname);
		int ppmFlag = 0;
		char name[40];
		strcpy(name, fname);
		int slen = strlen(name);
		char ppmname[80];
		if (strncmp(name+(slen-4), ".ppm", 4) == 0)
			ppmFlag = 1;
		if (ppmFlag) {
			strcpy(ppmname, name);
		} else {
			name[slen-4] = '\0';
			//printf("name **%s**\n", name);
			sprintf(ppmname,"%s.ppm", name);
			//printf("ppmname **%s**\n", ppmname);
			char ts[100];
			//system("convert eball.jpg eball.ppm");
			sprintf(ts, "convert %s %s", fname, ppmname);
			system(ts);
		}
		//sprintf(ts, "%s", name);
		//printf("read ppm **%s**\n", ppmname); fflush(stdout);
		FILE *fpi = fopen(ppmname, "r");
		if (fpi) {
			char line[200];
			fgets(line, 200, fpi);
			fgets(line, 200, fpi);
			//skip comments and blank lines
			while (line[0] == '#' || strlen(line) < 2)
				fgets(line, 200, fpi);
			sscanf(line, "%i %i", &width, &height);
			fgets(line, 200, fpi);
			//get pixel data
			int n = width * height * 3;
			data = new unsigned char[n];
			for (int i=0; i<n; i++)
				data[i] = fgetc(fpi);
			fclose(fpi);
		} else {
			printf("ERROR opening image: %s\n",ppmname);
			exit(0);
		}
		if (!ppmFlag)
			unlink(ppmname);
	}
};


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
///*
void initBG(int xres, int yres, GLuint &backgroundTexture, Image *bgImage, float xc[2], float yc[2])
{
	glGenTextures(1, &backgroundTexture);
	int bxres = xres;
	int byres = yres;
	glBindTexture(GL_TEXTURE_2D, backgroundTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, bxres, byres, 0,
		GL_RGB, GL_UNSIGNED_BYTE, bgImage->data);
	xc[0] = 0.0;
	xc[1] = 0.25;
	yc[0] = 0.0;
	yc[1] = 1.0;
}
//*/
void renderBackground(int xres, int yres, GLuint backgroundTexture, float xc[2], float yc[2])
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, backgroundTexture);
	glBegin(GL_QUADS);
		glTexCoord2f(xc[0], yc[1]); 
		glVertex2i(0, 0);
		glTexCoord2f(xc[0], yc[0]); 
		glVertex2i(0, yres);
		glTexCoord2f(xc[1], yc[0]); 
		glVertex2i(xres, yres);
		glTexCoord2f(xc[1], yc[1]); 
		glVertex2i(xres, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}


