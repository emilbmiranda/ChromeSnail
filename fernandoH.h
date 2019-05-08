#ifndef _FERNANDO_H_
#define _FERNANDO_H_

#include <X11/Xlib.h>
#include <GL/glx.h>

struct Wall {
	float pos[2];
	float vel = {4.00};

	Wall(); 
	Wall(int, int);

	void drawWall(int, int, GLuint);
	int getXpos();
	int getYpos();
	void slideWallBackward();
	void slideWallForward();
};

struct Platform {
	float pos[2];
	float vel = {4.00};

	Platform(); 
	Platform(int, int);

	void drawPlatform(int, int, GLuint);
	int getXpos();
	int getYpos();
	void slidePlatformBackward();
	void slidePlatformForward();
};

struct Cover{
	float pos[2];
	float vel = {4.00};

	Cover(); 
	Cover(int, int);

	void drawCover(int, int, GLuint);
	int getXpos();
	int getYpos();
	void slideCoverBackward();
	void slideCoverForward();
};

#endif