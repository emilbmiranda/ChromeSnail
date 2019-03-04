//creator: Victor Merino
//created by: 3/2/2019
#include <iostream>
#include <GL/glx.h>
#include "fonts.h"
#include "victorM.h"
using namespace std;

Bullets bullets;

void testBulletFunction()
{
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	timeCopy(&bullets.bulletTimer, &bt);

	Bullet b;
	bullets.barr[0] = b;

	shootBullet(&bullets, &bt);

	if (bullets.nbullets != 1) {
		cout << "bullets: " << bullets.nbullets << endl;
	}
}

int main(void)
{
	testBulletFunction();
	return 0;
}

