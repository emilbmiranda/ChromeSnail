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

BList::BList()
{
	head = NULL;
	tail = NULL;
	nbullets = 0;
}

int BList::Count()
{
	return nbullets;
}

Bullet* BList::Get(int index)
{
	if (!head)
		return NULL;

	Bullet *output = head;
	for (int i = 0; i < index && i < nbullets; i++) {
		output = output->next;
	}
	return output;
}

Bullet* BList::Add()
{
	Bullet *temp = new Bullet();
	temp->next = NULL;

	if (head == NULL) {
		tail = temp;
		head = temp;
	} else {
		tail->next = temp;
		tail = temp;
	}
	nbullets++;
	return temp;
}

void BList::Remove(int index)
{
	if(!head)
		return;

	Bullet *temp = head;
	// if list is 1 item only
	if (nbullets == 1) {
		head = NULL;
		tail = NULL;
	} else if (index == 0) {
		// if removing head
		head = head->next;
		temp = NULL;
	} else {
		// move to node right before the delete node
		for (int i = 0; i < (index -1); i++) {
			temp = temp->next;
		}

		if (temp->next == tail) {
			tail = temp;
			temp->next = NULL;
		} else {
			Bullet *removingNode = temp->next;
			temp->next = temp->next->next;
			removingNode->next = NULL;
		}
	}
	nbullets--;
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

void drawBullets(BList *bullets)
{
	for (int i = 0; i < bullets->Count(); i++) {
		Bullet *b = bullets->Get(i);
		if (!b)
			break;
		glColor3f(1.0, 1.0, 1.0);
		glPushMatrix();
		glTranslatef(bulletXOffset, bulletYOffset, 0);
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

void shootBullet(BList *bullets, timespec *bt, BulletDirection dir)
{
	//a little time between each bullet
	//shoot a bullet...
	if (bullets->Count() < MAX_BULLETS) {
		#ifdef PROFILE_VICTOR
		cout << "create bullet..." << endl;
		#endif

		Bullet *b = bullets->Add();
		
		#ifdef PROFILE_VICTOR
		cout << "get newly created bullet..." << endl;
		#endif

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
		} else if (dir == BackDiag) {
			b->vel[0] = -1.0f * (xdir*6.0f);
			b->vel[1] = ydir*6.0f;
		} else {
			b->vel[0] = xdir*6.0f;
		}

		#ifdef PROFILE_VICTOR
		cout << "bullet #: " << bullets->Count() << endl;
		cout << "bullet direction: x = " <<  b->vel[0]
			<< " y = " << b->vel[1] << endl;
		cout << "bullet position: x = " << b->pos[0]
			<< " y = " << b->pos[1] << endl;
		#endif
		b->color[0] = 1.0f;
		b->color[1] = 1.0f;
		b->color[2] = 1.0f;
	}
}

void updateBulletPosition(BList *bullets, int xres, int yres)
{
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	int i=0;
	while (i < bullets->Count()) {
		Bullet *b = bullets->Get(i);
		if (!b)
			break;
		
		// check for out of bound and remove bullet
		if(b->pos[0] >= (double)xres 
			|| b->pos[0] <= 0.0
			|| b->pos[1] >= (double)yres
			|| b->pos[1] <= 0.0) {
			// delete the bullet.
			bullets->Remove(i);
			continue;
		}

		//move the bullet
		b->pos[0] += b->vel[0];
		b->pos[1] += b->vel[1];
		i++;
	}
}

void checkBulletHelicopterCollision(BList *bullets, int x, int y)
{
	for (int i = 0; i < bullets->Count(); i++) {
		Bullet *b = bullets->Get(i);
		if (b->pos[1]+bulletYOffset >= y) {
			int currentPos = b->pos[0]+bulletXOffset;
			int tempX = x - 80;
			if (currentPos > tempX && currentPos < (tempX+100)) {
				#ifdef PROFILE_VICTOR
				cout << "x pos: " << x << " x+200: " << x+200 << endl;
				cout << "collided b#: " << currentPos << endl;
				#endif
				bullets->Remove(i);
				if(helicopterHit() == 0) {
					helicopterHealthReset();
				}
				playerScoreIncrease();
			}
		}
	}
}

double timeDiff(struct timespec *start, struct timespec *end)
{
	//return the difference in two times.
	return (double)(end->tv_sec - start->tv_sec ) +
		(double)(end->tv_nsec - start->tv_nsec) * oobillion;
}

void timeCopy(struct timespec *dest, struct timespec *source) 
{
	memcpy(dest, source, sizeof(struct timespec));
}