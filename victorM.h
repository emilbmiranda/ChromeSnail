#ifndef _VICTORM_H_
#define _VICTORM_H_

#include <time.h>
#include <math.h>

typedef double Vec[3];

typedef struct t_mouse 
{
	int x,y;
	int lastx,lasty;
	int lbuttondown;
} Mouse;

#define rnd() (((double)rand())/(double)RAND_MAX)
#define PI 3.141592653589793

const int MAX_BULLETS = 100;
const double oobillion = 1.0 / 1e9;
const int bulletXOffset = 400;
const int bulletYOffset = 300;

enum BulletDirection 
{
	Front = 0,
	Up = 1,
	Back = 2,
	Down = 3,
	FrontDiag = 4,
	BackDiag = 5
};

class Bullet 
{
public:
	Vec pos;
	Vec vel;
	float color[3];
	struct timespec time;
	Bullet();
	Bullet *next;
};

class BList {
private:
	Bullet *head;
	Bullet *tail;
	int nbullets;
public:
	struct timespec bulletTimer;
	Bullet* Add();
	void Remove(int index);
	Bullet* Get(int index);
	int Count();
	BList();
};

void drawBullets(BList *bullets);
void shootBullet(BList *bullets, timespec *t, BulletDirection dir);
void updateBulletPosition(BList *bullets, int xres, int yres);
void showVictorText(Rect r);
void showVictorPicture(int x, int y, GLuint textid);
double timeDiff(struct timespec *start, struct timespec *end);
void timeCopy(struct timespec *dest, struct timespec *source);
void checkBulletHelicopterCollision(BList *bullets, int x, int y);
void helicopterHit();

#endif