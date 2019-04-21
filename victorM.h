#ifndef _VICTORM_H_
#define _VICTORM_H_

#include <time.h>

typedef double Vec[3];

typedef struct t_mouse {
	int x,y;
	int lastx,lasty;
	int lbuttondown;
} Mouse;

#define rnd() (((double)rand())/(double)RAND_MAX)
#define PI 3.141592653589793

const int MAX_BULLETS = 11;
const double oobillion = 1.0 / 1e9;

enum BulletDirection{
	Front = 0,
	Up = 1,
	Back = 2,
	Down = 3,
	FrontDiag = 4,
	BackDiag = 5
};

class Bullet {
public:
	Vec pos;
	Vec vel;
	float color[3];
	struct timespec time;
	Bullet();
};

class Bullets {
public:
	Bullet *barr;
	int nbullets;
	struct timespec bulletTimer;
	Bullets();
};

void drawBullets(Bullets *bullets);
void shootBullet(Bullets *bullets, timespec *t, BulletDirection dir);
void updateBulletPosition(Bullets *bullets, int xres, int yres);
void showVictorText(Rect r);
void showVictorPicture(int x, int y, GLuint textid);
double timeDiff(struct timespec *start, struct timespec *end);
void timeCopy(struct timespec *dest, struct timespec *source);

#endif