// Creator: Victor Merino
// Created Date: 2/14/2019
//
#include <GL/glx.h>
#include <iostream>
#include <math.h>
#include <ctime>
#include <cstring>
#include "fonts.h"
#include "victorM.h"
using namespace std;

Bullet::Bullet()
{
	int size = *(&pos + 1) - pos;
	for (int i = 0; i < size ; i++) {
		pos[i] = 0;
		vel[i] = 0;
		color[i] = 0;
	}
}

Bullets::Bullets()
{
	barr = new Bullet();
	nbullets = 0;
}

void showVictorText(Rect r) 
{
	ggprint8b(&r, 16, 0xff8866, "Victor Merino");
}

void showVictorPicture(int x, int y, GLuint textid) 
{
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

void drawBullets(Bullets *bullets)
{
	for (int i=0; i< bullets->nbullets; i++) {
		Bullet *b = &bullets->barr[i];
		//Log("draw bullet...\n");
		glColor3f(1.0, 1.0, 1.0);
		glPushMatrix();
		glTranslatef(400, 350, 0);
		glBegin(GL_POINTS);
		glVertex2f(b->pos[0],      b->pos[1]);
		glVertex2f(b->pos[0]-1.0f, b->pos[1]);
		glVertex2f(b->pos[0]+1.0f, b->pos[1]);
		glVertex2f(b->pos[0],      b->pos[1]-1.0f);
		glVertex2f(b->pos[0],      b->pos[1]+1.0f);
		glColor3f(0.8, 0.8, 0.8);
		glVertex2f(b->pos[0]-1.0f, b->pos[1]-1.0f);
		glVertex2f(b->pos[0]-1.0f, b->pos[1]+1.0f);
		glVertex2f(b->pos[0]+1.0f, b->pos[1]-1.0f);
		glVertex2f(b->pos[0]+1.0f, b->pos[1]+1.0f);
		glEnd();
		glPopMatrix();
	}
}

void shootBullet(Bullets *bullets, timespec *bt, BulletDirection dir)
{
	//a little time between each bullet
	//shoot a bullet...
	if (bullets->nbullets < MAX_BULLETS) {
		Bullet *b = &bullets->barr[bullets->nbullets];
		timeCopy(&b->time, bt);
		double xdir = .5;
		double ydir = .5;
		b->pos[0] += xdir*20.0f;
		b->pos[1] += ydir*20.0f;

		if (dir == Up) {
			b->vel[1] = ydir*6.0f;
		} else if (dir == Back) {
			b->vel[0] = -1.0f * (xdir*6.0f);
		} else if (dir == Down) {
			b->vel[1] = -1.0f * (ydir*6.0f);
		} else if (dir == FrontDiag) {
			b->vel[0] = xdir*6.0f;
			b->vel[1] = ydir*6.0f;
		} else {
			b->vel[0] = xdir*6.0f;
		}

		#ifdef PROFILE_VICTOR
		cout << "bullet #: " << bullets->nbullets << endl;
		cout << "bullet direction: x = " <<  b->vel[0]
			<< " y = " << b->vel[1] << endl;
		cout << "bullet position: x = " << b->pos[0]
			<< " y = " << b->pos[1] << endl;
		#endif
		b->color[0] = 1.0f;
		b->color[1] = 1.0f;
		b->color[2] = 1.0f;
		++bullets->nbullets;
	}
}

void updateBulletPosition(Bullets *bullets, int xres, int yres)
{
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	int i=0;
	while (i < bullets->nbullets) {
		Bullet *b = &bullets->barr[i];
		//How long has bullet been alive?
		double ts = timeDiff(&b->time, &bt);
		if (ts > 2.5) {
			//time to delete the bullet.
			memcpy (&bullets->barr[i], &bullets->barr[bullets->nbullets-1],
				sizeof(Bullet));
			bullets->nbullets--;
			bullets->barr[i].vel[0] = 0;
			bullets->barr[i].vel[1] = 0;
			bullets->barr[i].pos[0] = 0;
			bullets->barr[i].pos[1] = 0;
			//do not increment i.
			continue;
		}
		// TODO: check for out of bound
		if(bullets->barr[i].pos[1] >= (double)xres) {
			// delete the bullet.
			memcpy (&bullets->barr[i], &bullets->barr[bullets->nbullets-1],
				sizeof(Bullet));
			bullets->nbullets--;
			bullets->barr[i].vel[0] = 0;
			bullets->barr[i].vel[1] = 0;
			bullets->barr[i].pos[0] = 0;
			bullets->barr[i].pos[1] = 0;
			//do not increment i.
			continue;
		}

		//move the bullet
		b->pos[0] += b->vel[0];
		b->pos[1] += b->vel[1];
		//Check for collision with window edges
		if (b->pos[0] < 0.0) {
			b->pos[0] += (float)xres;
		}
		else if (b->pos[0] > (float)xres) {
			b->pos[0] -= (float)xres;
		}
		else if (b->pos[1] < 0.0) {
			b->pos[1] += (float)yres;
		}
		else if (b->pos[1] > (float)yres) {
			b->pos[1] -= (float)yres;
		}
		i++;
	}
}

double timeDiff(struct timespec *start, struct timespec *end)
{
	//return the difference in two times.
	return (double)(end->tv_sec - start->tv_sec ) +
		(double)(end->tv_nsec - start->tv_nsec) * oobillion;
}

void timeCopy(struct timespec *dest, struct timespec *source) {
	memcpy(dest, source, sizeof(struct timespec));
}