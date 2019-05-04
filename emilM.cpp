/*Created by: Emil Miranda
* Created on: February 14, 2019
 */

#include <math.h>
#include "fonts.h"
#include "emilM.h"
#include "sqlite3.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

sqlite3 *db;
int result;
char *ErrMsg;
const char *data = "Callback function called";
bool sql_init_flag = 1;
Rect leaderboard_rect;
int leaderboardY = 550;
int leaderboardX = 300;
vector<string> leaderboard_vector;

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

void show_keys(int xres, int yres, GLuint lettersTexture[])
{
	static int wid = 40;
	float fx = (float)xres/2-250;
	float fy = (float)yres/2-25;
	const char *playGame = "P - Play Game";
	int size = strlen(playGame);
	for (int i = 0; i < size; i++) {
		char letter = playGame[i];
		glPushMatrix();
		glTranslatef(fx,fy,0);
		render_letter(letter, lettersTexture);
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
		fx += 40;
	}
	const char *leaderboard = "L - Leaderboard";
	size = strlen(leaderboard);
	fy -= 50;
	fx = (float)xres/2-250;
	for (int i = 0; i < size; i++) {
		char letter = leaderboard[i];
		glPushMatrix();
		glTranslatef(fx,fy,0);
		render_letter(letter, lettersTexture);
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
		fx += 40;
	}
	const char *credits = "C - Credits";
	size = strlen(credits);
	fy -= 50;
	fx = (float)xres/2-250;
	for (int i = 0; i < size; i++) {
		char letter = credits[i];
		glPushMatrix();
		glTranslatef(fx,fy,0);
		render_letter(letter, lettersTexture);
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
		fx += 40;
	}
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

int init_connection()
{
	int connection = sqlite3_open("ChromeSnail.db",&db);
	if (connection) {
		#ifdef SQL_UNIT_TEST
		cout << "Connection Unsuccessful:" <<  sqlite3_errmsg(db) << endl;
		#endif
		return 0;
	} else {
		#ifdef SQL_UNIT_TEST
		cout << "Connection Successful" << endl;
		#endif
		return 1;
	}
}

#ifdef SQL_UNIT_TEST
void create_table()
{
	if (init_connection()) {
		const char *query = "CREATE TABLE IF NOT EXISTS Leaderboard (" 
			"GameId INTEGER PRIMARY KEY AUTOINCREMENT," 
			"Name VARCHAR(2) NOT NULL," 
			"Time TIME NOT NULL);";
		result = sqlite3_exec(db, query, 0, 0, &ErrMsg);
		if (result != SQLITE_OK) {
			cout << "SQL Error:" << ErrMsg << endl;
			sqlite3_free(ErrMsg);
		} else {
			cout << "Table created successfully" << endl;
		}
	}
}
#endif

#ifdef SQL_UNIT_TEST
void insert()
{
	if (init_connection()) {
		const char *emil = "INSERT INTO Leaderboard"
			"(Name, Time)"
			"Values ('EM','1:00')";
		const char *hasun = "INSERT INTO Leaderboard"
			"(Name, Time)"
			"Values ('HK','0:50')";
		const char *mason = "INSERT INTO Leaderboard"
			"(Name, Time)"
			"Values ('MP','1:05')";
		const char *fernando = "INSERT INTO Leaderboard"
			"(Name, Time)"
			"Values ('FH','1:30')";
		const char *victor = "INSERT INTO Leaderboard"
			"(Name, Time)"
			"Values ('VM','2:00')";
		result = sqlite3_exec(db, emil, 0, 0, &ErrMsg);
		if (result != SQLITE_OK) {
			cout << "SQL Error:" << ErrMsg << endl;
			sqlite3_free(ErrMsg);
		} else {
			cout << "Emil added successfully" << endl;
		}
		result = sqlite3_exec(db, hasun, 0, 0, &ErrMsg);
		if (result != SQLITE_OK) {
			cout << "SQL Error:" << ErrMsg << endl;
			sqlite3_free(ErrMsg);
		} else {
			cout << "Hasun added successfully" << endl;
		}
		result = sqlite3_exec(db, mason, 0, 0, &ErrMsg);
		if (result != SQLITE_OK) {
			cout << "SQL Error:" << ErrMsg << endl;
			sqlite3_free(ErrMsg);
		} else {
			cout << "Mason added successfully" << endl;
		}
		result = sqlite3_exec(db, fernando, 0, 0, &ErrMsg);
		if (result != SQLITE_OK) {
			cout << "SQL Error:" << ErrMsg << endl;
			sqlite3_free(ErrMsg);
		} else {
			cout << "Fernando added successfully" << endl;
		}
		result = sqlite3_exec(db, victor, 0, 0, &ErrMsg);
		if (result != SQLITE_OK) {
			cout << "SQL Error:" << ErrMsg << endl;
			sqlite3_free(ErrMsg);
		} else {
			cout << "Victor added successfully" << endl;
		}
	}
	sqlite3_close(db);
}
#endif

void generate_leaderboard()
{
 	if (init_connection()) {
		#ifdef SQL_UNIT_TEST
		if (sql_init_flag) {
 			create_table();
			insert();
			sql_init_flag = 0;
		}
		#endif
		const char *showLeaderboard = "SELECT * from Leaderboard "
			"ORDER BY Time LIMIT 3";
		result = sqlite3_exec(db, showLeaderboard, callback, 
			(void*) data, &ErrMsg);
		if (result != SQLITE_OK) {
			#ifdef SQL_UNIT_TEST
			cout << "SQL Error:" << ErrMsg << endl;
			#endif
			sqlite3_free(ErrMsg);
		} else {
			#ifdef SQL_UNIT_TEST
			cout << "SELECT statement successful" << endl;
			#endif
		}
 	}
 	sqlite3_close(db);
}

int callback(void *data, int argc, char **argv, char **azColName)
{
	(void) data;
	(void) argc;
	(void) azColName;
	char* argv_name= (char*)calloc(30, sizeof(char));
	strcpy(argv_name, argv[1]);
	int name_size = strlen(argv[1]);
	string name(argv_name, name_size);
	char* argv_time= (char*)calloc(30, sizeof(char));
	strcpy(argv_time, argv[2]);
	string time(argv_time, 10);
	leaderboard_vector.insert(leaderboard_vector.end(), name);
	leaderboard_vector.insert(leaderboard_vector.end(), time);
	return SQLITE_OK;
}

void print_leaderboard(int xres, int yres, GLuint numbersTexture[],
	GLuint lettersTexture[]) 
{
	static int wid = 40;
	float fx = (float)xres/2-175;
	float fy = (float)yres/2+50;
	for (int i = 1; i <= 3; i++) { 
		glPushMatrix();
		glTranslatef(fx,fy,0);
		glBindTexture(GL_TEXTURE_2D, numbersTexture[i]);
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
		print_name(fx+50, fy, wid, leaderboard_vector[i*2-2], lettersTexture);
		print_time(fx+175, fy, wid, leaderboard_vector[i*2-1], numbersTexture);	
		fy -= 100;
	}
}

void print_name(int xres, int yres, int width, string name, GLuint lettersTexture[])
{
	int size = name.length();
	static int wid = width;
	float fx = (float)xres;
	float fy = (float)yres;
	for (int i = 0; i < size; i++) {
		fx += 40;
		glPushMatrix();
		glTranslatef(fx,fy,0);
		char letter = (char)name[i];
		render_letter(letter, lettersTexture);
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
}

void print_time(int xres, int yres, int width, string time, GLuint numbersTexture[])
{
	static int wid = width;
	float fx = (float)xres;
	float fy = (float)yres;
	for (int i = 0; i < 4; i++) {
		fx += 40;
		glPushMatrix();
		glTranslatef(fx,fy,0);
		char char_time = (char)time[i];
		render_number(char_time, numbersTexture);
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
}

void render_letter(char letter, GLuint lettersTexture[])
{
	switch (letter) {
		case('A'):
		case('a'):
			glBindTexture(GL_TEXTURE_2D, lettersTexture[0]);
			break;
		case('B'):
		case('b'):
			glBindTexture(GL_TEXTURE_2D, lettersTexture[1]);
			break;
		case('C'):
		case('c'):
			glBindTexture(GL_TEXTURE_2D, lettersTexture[2]);
			break;
		case('D'):
		case('d'):
			glBindTexture(GL_TEXTURE_2D, lettersTexture[3]);
			break;
		case('E'):
		case('e'):
			glBindTexture(GL_TEXTURE_2D, lettersTexture[4]);
			break;
		case('F'):
		case('f'):
			glBindTexture(GL_TEXTURE_2D, lettersTexture[5]);
			break;
		case('G'):
		case('g'):
			glBindTexture(GL_TEXTURE_2D, lettersTexture[6]);
			break;
		case('H'):
		case('h'):
			glBindTexture(GL_TEXTURE_2D, lettersTexture[7]);
			break;
		case('I'):
		case('i'):
			glBindTexture(GL_TEXTURE_2D, lettersTexture[8]);
			break;
		case('J'):
		case('j'):
			glBindTexture(GL_TEXTURE_2D, lettersTexture[9]);
			break;
		case('K'):
		case('k'):
			glBindTexture(GL_TEXTURE_2D, lettersTexture[10]);
			break;
		case('L'):
		case('l'):
			glBindTexture(GL_TEXTURE_2D, lettersTexture[11]);
			break;
		case('M'):
		case('m'):
			glBindTexture(GL_TEXTURE_2D, lettersTexture[12]);
			break;
		case('N'):
		case('n'):
			glBindTexture(GL_TEXTURE_2D, lettersTexture[13]);
			break;
		case('O'):
		case('o'):
			glBindTexture(GL_TEXTURE_2D, lettersTexture[14]);
			break;
		case('P'):
		case('p'):
			glBindTexture(GL_TEXTURE_2D, lettersTexture[15]);
			break;
		case('Q'):
		case('q'):
			glBindTexture(GL_TEXTURE_2D, lettersTexture[16]);
			break;
		case('R'):
		case('r'):
			glBindTexture(GL_TEXTURE_2D, lettersTexture[17]);
			break;
		case('S'):
		case('s'):
			glBindTexture(GL_TEXTURE_2D, lettersTexture[18]);
			break;
		case('T'):
		case('t'):
			glBindTexture(GL_TEXTURE_2D, lettersTexture[19]);
			break;
		case('U'):
		case('u'):
			glBindTexture(GL_TEXTURE_2D, lettersTexture[20]);
			break;
		case('V'):
		case('v'):
			glBindTexture(GL_TEXTURE_2D, lettersTexture[21]);
			break;
		case('W'):
		case('w'):
			glBindTexture(GL_TEXTURE_2D, lettersTexture[22]);
			break;
		case('X'):
		case('x'):
			glBindTexture(GL_TEXTURE_2D, lettersTexture[23]);
			break;
		case('Y'):
		case('y'):
			glBindTexture(GL_TEXTURE_2D, lettersTexture[24]);
			break;
		case('Z'):
		case('z'):
			glBindTexture(GL_TEXTURE_2D, lettersTexture[25]);
			break;
		case('-'):
			glBindTexture(GL_TEXTURE_2D, lettersTexture[26]);
			break;
		default:
			glBindTexture(GL_TEXTURE_2D, lettersTexture[27]);
	}
}

void render_number(char number, GLuint numbersTexture[])
{ 
	switch (number) {
		case('0'):
			glBindTexture(GL_TEXTURE_2D, numbersTexture[0]);
			break;
		case('1'):
			glBindTexture(GL_TEXTURE_2D, numbersTexture[1]);
			break;
		case('2'):
			glBindTexture(GL_TEXTURE_2D, numbersTexture[2]);
			break;
		case('3'):
			glBindTexture(GL_TEXTURE_2D, numbersTexture[3]);
			break;
		case('4'):
			glBindTexture(GL_TEXTURE_2D, numbersTexture[4]);
			break;
		case('5'):
			glBindTexture(GL_TEXTURE_2D, numbersTexture[5]);
			break;
		case('6'):
			glBindTexture(GL_TEXTURE_2D, numbersTexture[6]);
			break;
		case('7'):
			glBindTexture(GL_TEXTURE_2D, numbersTexture[7]);
			break;
		case('8'):
			glBindTexture(GL_TEXTURE_2D, numbersTexture[8]);
			break;
		case('9'):
			glBindTexture(GL_TEXTURE_2D, numbersTexture[9]);
			break;
		case(':'):
			glBindTexture(GL_TEXTURE_2D, numbersTexture[10]);
			break;
		default:
			glBindTexture(GL_TEXTURE_2D, numbersTexture[10]);
	}
}