#ifndef _FERNANDO_H_
#define _FERNANDO_H_

#include <X11/Xlib.h>
#include <GL/glx.h>


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


#endif