//3350
//program: hasunK.cpp
//author:  Hasun Khan 
//date:    Spring 2019
//
//1. Display my picture and write my name as a part of the credits 
//2. Manage the movement engine of the main character 
//3. Render the main character
//4. Render the background 
//5. Develop audio engine
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
	int xres, yres;
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
	float a = 0; 
	//a += sin(angle)*10.0f;
	fx += a;
	angle += 0.2f;

	glPushMatrix(); 
	glTranslatef(fx,fy,0);
	glRotatef(a, 0, 0,1.0);
	glBindTexture(GL_TEXTURE_2D, textid);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,1.0f); 
		glVertex2i(-w,-w);
		glTexCoord2f(0.0f,0.0f); 
		glVertex2i(-w,w);
		glTexCoord2f(1.0f,0.0f); 
		glVertex2i(w,w);
		glTexCoord2f(1.0f,1.0f); 
		glVertex2i(w,-w);
	glEnd();
	glPopMatrix();
}


int getXres()
{
	return g.xres;
}


void setXres(int x) 
{
	g.xres = x; 
}


int getYres()
{
	return g.yres;
}


void setYres (int y) 
{
	g.yres = y; 
}


//function used to render main character 
void renderWalker(GLuint walkerID, int walkFrame, bool leftPressed)
{
	// cx and cy are used for positioning h and w for height and width
    float cx = g.xres/2.0;
    float cy = 100;
	float h = 50.0;
	float w = h *0.5;

	    glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glBindTexture(GL_TEXTURE_2D, walkerID);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor4ub(255,255,255,255);

        int ix = walkFrame % 8;
        int iy = 0;
        if (walkFrame >= 8) {
            iy = 1;
		}
        float fx = (float)ix / 8.0;
        float fy = (float)iy / 2.0;

        glBegin(GL_QUADS);
        if (leftPressed == true) {
            glTexCoord2f(fx+.125, fy+.5);
            glVertex2i(cx-w, cy-h);
            glTexCoord2f(fx+.125, fy);
            glVertex2i(cx-w, cy+h);
            glTexCoord2f(fx, fy);
            glVertex2i(cx+w, cy+h);
            glTexCoord2f(fx, fy+.5);
            glVertex2i(cx+w, cy-h);
        } else {
            glTexCoord2f(fx, fy+.5);
            glVertex2i(cx-w, cy-h);
            glTexCoord2f(fx, fy);
            glVertex2i(cx-w, cy+h);
            glTexCoord2f(fx+.125, fy);
            glVertex2i(cx+w, cy+h);
            glTexCoord2f(fx+.125, fy+.5);
            glVertex2i(cx+w, cy-h);
        }
        glEnd();
        glPopMatrix();
}


//Velocity based movement 
float moveWalker(int direction, float position, float velocity)
{
    if ((position < -145.0 && velocity < 0.0) || (position >= getXres()+140.0 &&
		velocity > 0.0))
    {
		velocity = -velocity;
    }

	//right movement 
	if (direction == 1){  
		return position += velocity;
    }
	//left movement
	if (direction == 2){ 
		return position -= velocity;
    }
	if (direction == 3){
		return position += velocity;
	}
	return velocity; 
}


/*
void initBG(int xres, int yres, GLuint *backgroundTexture, 
unsigned char *data, float xc[2], float yc[2])
{
	glGenTextures(1, backgroundTexture);
	int bxres = xres;
	int byres = yres;
	glBindTexture(GL_TEXTURE_2D, *backgroundTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, bxres, byres, 0,
		GL_RGB, GL_UNSIGNED_BYTE, data);
	xc[0] = 0.0;
	xc[1] = 0.25;
	yc[0] = 0.0;
	yc[1] = 1.0;
}


void renderBackground(float xc[2], float yc[2], GLuint backgroundTexture, )
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
*/

/*

	//Get started right here.
#ifdef USE_OPENAL_SOUND
	alutInit(0, NULL);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: alutInit()\n");
		return 0;
	}
	//Clear error state.
	alGetError();
	//
	//Setup the listener.
	//Forward and up vectors are used.
	float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);
	//
	//Buffer holds the sound information.
	ALuint alBuffer;
	alBuffer = alutCreateBufferFromFile("./test.wav");
	//
	//Source refers to the sound.
	ALuint alSource;
	//Generate a source, and store it in a buffer.
	alGenSources(1, &alSource);
	alSourcei(alSource, AL_BUFFER, alBuffer);
	//Set volume and pitch to normal, no looping of sound.
	alSourcef(alSource, AL_GAIN, 1.0f);
	alSourcef(alSource, AL_PITCH, 1.0f);
	alSourcei(alSource, AL_LOOPING, AL_FALSE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return 0;
	}
	for (int i=0; i<4; i++) {
		alSourcePlay(alSource);
		usleep(250000);
	}
	//Cleanup.
	//First delete the source.
	alDeleteSources(1, &alSource);
	//Delete the buffer.
	alDeleteBuffers(1, &alBuffer);
	//Close out OpenAL itself.
	//Get active context.
	ALCcontext *Context = alcGetCurrentContext();
	//Get device for active context.
	ALCdevice *Device = alcGetContextsDevice(Context);
	//Disable context.
	alcMakeContextCurrent(NULL);
	//Release context(s).
	alcDestroyContext(Context);
	//Close device.
	alcCloseDevice(Device);
#endif //USE_OPENAL_SOUND
	return 0;
}
*/


