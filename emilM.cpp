/*Created by: Emil Miranda
* Created on: February 14, 2019
 */

#include <math.h>
#include "fonts.h"
#include "emilM.h"
//#include <sqlite3.h>
#include <iostream>

using namespace std;

//sqlite3 *db;
int result;
char *ErrMsg;
bool sql_init_flag = 1;

#define SQL_UNIT_TEST

void showEmil(Rect r, int y)
{
	ggprint8b(&r, y, 0xffff66, "Emil Miranda");
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
	glTexCoord2f(0.0f, 1.0f);
	glVertex2i(-wid,-wid);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2i(-wid, wid);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2i( wid, wid);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2i( wid,-wid);
	glEnd();
	glPopMatrix();
}

void create_menu_button(int gl_xres, int gl_yres)
{
	Rect r;
	r.bot = gl_yres - 20;
	r.left = gl_xres - 50;
	r.center = 0;
	r.height = 200;
	r.width = 100;
	ggprint12(&r, 100, 0xffffff, "Menu");
}

void start_menu(int xres, int yres, GLuint startMenuTexture)
{
	glClear(GL_COLOR_BUFFER_BIT);
	static int wid = xres/2;
	float fx = (float)xres;
	float fy = (float)yres;
	glPushMatrix();
	glTranslatef(fx/2,fy/2,0);
	glBindTexture(GL_TEXTURE_2D, startMenuTexture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2i(-wid,-wid);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2i(-wid, wid);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2i( wid, wid);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2i( wid,-wid);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}

void show_logo(int xres, int yres, GLuint logoTexture)
{
	static int wid = xres/4.5;
	float fx = (float)xres;
	float fy = (float)yres+300;
	glPushMatrix();
	glTranslatef(fx/2,fy/2,0);
	glBindTexture(GL_TEXTURE_2D, logoTexture);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2i(-wid,-wid);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2i(-wid, wid);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2i( wid, wid);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2i( wid,-wid);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}

void show_keys(int xres, int yres, GLuint keysTexture)
{
	static int wid = xres/5;
	float fx = (float)xres;
	float fy = (float)yres-300;
	glPushMatrix();
	glTranslatef(fx/2,fy/2,0);
	glBindTexture(GL_TEXTURE_2D, keysTexture);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2i(-wid,-wid);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2i(-wid, wid);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2i( wid, wid);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2i( wid,-wid);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}

void leaderboard(int xres, int yres, GLuint leaderboardTexture)
{
	glClear(GL_COLOR_BUFFER_BIT);
	static int wid = xres/2;
	float fx = (float)xres;
	float fy = (float)yres;
	glPushMatrix();
	glTranslatef(fx/2,fy/2,0);
	glBindTexture(GL_TEXTURE_2D, leaderboardTexture);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2i(-wid,-wid);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2i(-wid, wid);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2i( wid, wid);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2i( wid,-wid);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
	if (sql_init_flag) {
		//show_leaderboard();
		sql_init_flag = 0;
	}
}

void leaderboard_title(int xres, int yres, GLuint leaderboardTitleTexture)
{
	static int wid = xres/3.5;
	float fx = (float)xres;
	float fy = (float)yres+400;
	glPushMatrix();
	glTranslatef(fx/2,fy/2,0);
	glBindTexture(GL_TEXTURE_2D, leaderboardTitleTexture);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(255,255,255,255);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2i(-wid,-wid);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2i(-wid, wid);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2i( wid, wid);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2i( wid,-wid);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}

// int init_connection()
// {
// 	int connection = sqlite3_open("ChromeSnail.db",&db);
// 	#ifdef SQL_UNIT_TEST
// 	if (connection) {
// 		cout << "Connection Unsuccessful:" <<  sqlite3_errmsg(db) << endl;
// 		return 0;
// 	} else {
// 		cout << "Connection Successful" << endl;
// 		return 1;
// 	}
// 	#endif
// }

// void create_table()
// {
// 	if (init_connection()) {
// 		const char *query = "CREATE TABLE IF NOT EXISTS Leaderboard (" 
// 			"GameId INTEGER PRIMARY KEY AUTOINCREMENT," 
// 			"Name VARCHAR(50) NOT NULL," 
// 			"Time VARCHAR(50) NOT NULL);";
// 		result = sqlite3_exec(db, query, 0, 0, &ErrMsg);
// 		if (result != SQLITE_OK) {
// 			cout << "SQL Error:" << ErrMsg << endl;
// 			sqlite3_free(ErrMsg);
// 		} else {
// 			cout << "Table created successfully" << endl;
// 		}
// 	}
// }

// void insert()
// {
// 	if (init_connection()) {

// 	}
// 	sqlite3_close(db);
// }

// void show_leaderboard()
// {
//  	if (init_connection()) {
//  		create_table();
//  	}
//  	sqlite3_close(db);
// }