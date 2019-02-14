/*Created by: Emil Miranda
* Created on: February 14, 2019
 */

//Text and font headers from fonts.h from asteroids framework
#include <iostream>
#include <GL/glx.h>
#include <math.h>
#include "fonts.h"

void showEmil(Rect r, int y)
{
    ggprint8b(&r, 16, 0xffff66, "Emil Miranda");
}

void showEmilPicture(int x, int y, GLuint textid)
{
    glColor3ub(255,255,255);
    static float angle = 0.0f;
    static int wid = 30;
    wid += sin(angle) * 10;
    float fx = (float)x;
    float fy = (float)y;
    float a = 0;
    a += sin(angle) * 10.0f;
    fx += a;
    angle += 0.2f;
    glPushMatrix();
    glTranslatef(fx,fy,0);
    glRotatef(a, 0, 0, 1.0);
    glBindTexture(GL_TEXTURE_2D, textid);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
    glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
    glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);

    glEnd();
    glPopMatrix();
}

