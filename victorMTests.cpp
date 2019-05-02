//creator: Victor Merino
//created by: 3/2/2019
#include <iostream>
#include <GL/glx.h>
#include "fonts.h"
#include "victorM.h"
using namespace std;

//Bullets bullets;
BList bullets;

void testBulletFunction()
{
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	timeCopy(&bullets.bulletTimer, &bt);

	Bullet b;
	bullets.Add();//.barr[0] = b;

	shootBullet(&bullets, &bt, Front);

	if (bullets.Count() != 1) {
		cout << "bullets: " << bullets.Count() << endl;
	}
}

int main(void)
{
	testBulletFunction();
	cout << "Tests completed!" << endl;
	return 0;
}

