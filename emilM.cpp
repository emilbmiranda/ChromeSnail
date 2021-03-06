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
#include <chrono>
#include <sstream>
#include <fstream>

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
vector<char> ranking_vector;
auto startTime = chrono::system_clock::now();
const char *ranking;

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
			"Score INTEGER NOT NULL," 
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
			"(Name, Score, Time)"
			"Values ('EM', 20, '1:00')";
		const char *hasun = "INSERT INTO Leaderboard"
			"(Name, Score, Time)"
			"Values ('HK', 15, '0:50')";
		const char *mason = "INSERT INTO Leaderboard"
			"(Name, Score, Time)"
			"Values ('MP', 21, '1:05')";
		const char *fernando = "INSERT INTO Leaderboard"
			"(Name, Score, Time)"
			"Values ('FH',23, '1:30')";
		const char *victor = "INSERT INTO Leaderboard"
			"(Name, Score, Time)"
			"Values ('VM', 21, '2:00')";
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
			exit(0);
		}
#endif
		const char *showLeaderboard = "SELECT ROW_NUMBER() OVER("
			"ORDER BY Score DESC, Time) as Row,"
			"Name,Score,Time FROM Leaderboard "
			"LIMIT 3;";
		result = sqlite3_exec(db, showLeaderboard, callback, 
				(void*) data, &ErrMsg);
		if (result != SQLITE_OK) {
#ifdef SQL_UNIT_TEST
			cout << "SQL Error:" << ErrMsg << endl;
			sqlite3_free(ErrMsg);
#endif
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
	char* argv_score = (char*)calloc(30, sizeof(char));
	strcpy(argv_score, argv[2]);
	string score(argv_score, 2);
	char* argv_time= (char*)calloc(30, sizeof(char));
	strcpy(argv_time, argv[3]);
	string time(argv_time, 10);
	leaderboard_vector.insert(leaderboard_vector.end(), name);
	leaderboard_vector.insert(leaderboard_vector.end(), score);
	leaderboard_vector.insert(leaderboard_vector.end(), time);
	free(argv_name);
	free(argv_score);
	free(argv_time);
	return SQLITE_OK;
}

void print_leaderboard(int xres, int yres, GLuint numbersTexture[],
		GLuint lettersTexture[]) 
{
	static int wid = 40;
	float fx = (float)xres/2-225;
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
		leaderboard_name(fx+50, fy, wid, leaderboard_vector[i*3-3], 
				lettersTexture);
		leaderboard_score(fx+170, fy, wid, leaderboard_vector[i*3-2], 
				numbersTexture);
		leaderboard_time(fx+275, fy, wid, leaderboard_vector[i*3-1], 
				numbersTexture);	
		fy -= 100;
	}
}

void leaderboard_name(int xres, int yres, int width, string name, GLuint lettersTexture[])
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

void leaderboard_time(int xres, int yres, int width, string time, GLuint numbersTexture[])
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

void leaderboard_score(int xres, int yres, int width, string score, GLuint numbersTexture[])
{
	static int wid = width;
	float fx = (float)xres;
	float fy = (float)yres;
	for (int i = 0; i < 2; i++) {
		fx += 40;
		glPushMatrix();
		glTranslatef(fx,fy,0);
		char char_score = (char)score[i];
		render_number(char_score, numbersTexture);
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

void render_number(int number, GLuint numbersTexture[])
{ 
	switch (number) {
		case(0):
			glBindTexture(GL_TEXTURE_2D, numbersTexture[0]);
			break;
		case(1):
			glBindTexture(GL_TEXTURE_2D, numbersTexture[1]);
			break;
		case(2):
			glBindTexture(GL_TEXTURE_2D, numbersTexture[2]);
			break;
		case(3):
			glBindTexture(GL_TEXTURE_2D, numbersTexture[3]);
			break;
		case(4):
			glBindTexture(GL_TEXTURE_2D, numbersTexture[4]);
			break;
		case(5):
			glBindTexture(GL_TEXTURE_2D, numbersTexture[5]);
			break;
		case(6):
			glBindTexture(GL_TEXTURE_2D, numbersTexture[6]);
			break;
		case(7):
			glBindTexture(GL_TEXTURE_2D, numbersTexture[7]);
			break;
		case(8):
			glBindTexture(GL_TEXTURE_2D, numbersTexture[8]);
			break;
		case(9):
			glBindTexture(GL_TEXTURE_2D, numbersTexture[9]);
			break;
		default:
			glBindTexture(GL_TEXTURE_2D, numbersTexture[10]);
	}
}

void start_time()
{
	startTime = chrono::system_clock::now();
}

void print_time(int yres, GLuint numbersTexture[])
{
	auto currentTime = chrono::system_clock::now();
	double time_minutes = chrono::duration_cast<chrono::minutes>
		(currentTime-startTime).count();
	double time_seconds = chrono::duration_cast<chrono::seconds>
		(currentTime-startTime).count();
	static int wid = 40;
	float fx = 60;
	float fy = (float)yres - 50;
	int tens = (int)fmod(time_seconds,60.0)/10;
	int ones = (int)fmod(time_seconds,60.0)%10;
	//declared array as followed due to getting narrowing warning
	//when initiliazing with {}
	int time_array[4];
	time_array[0] = time_minutes;
	time_array[1] = -1;
	time_array[2] = tens;
	time_array[3] = ones;
	for (int i = 0; i < 4; i++) {
		glPushMatrix();
		glTranslatef(fx,fy,0);
		render_number(time_array[i], numbersTexture);
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
		fx += 30;
	}
}

void print_score(int gameScore, int xres, int yres, GLuint numbersTexture[])
{
	float fx = (float)xres/2 - 30;
	float fy = (float)yres - 50;
	int tens = (int)gameScore/10;
	int ones = (int)gameScore%10;
	int wid = 60;
	//declared array as followed due to getting narrowing warning
	//when initiliazing with {}
	int score_array[2];
	score_array[0] = tens;
	score_array[1] = ones;
	for (int i = 0; i < 2; i++) {
		glPushMatrix();
		glTranslatef(fx,fy,0);
		render_number(score_array[i], numbersTexture);
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
		fx += 60;
	} 
}

void game_over(int xres, int yres, GLuint leaderboardTexture)
{
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

void game_over_text(int xres, int yres, int playerScore, string finalTime, 
		GLuint lettersTexture[], GLuint numbersTexture[]) 
{
	static int wid = 20;
	float fx = (float)xres/2-200;
	float fy = (float)yres-25;
	const char *gameOver1stLine = "Thank you for playing.";
	int size = strlen(gameOver1stLine);
	for (int i = 0; i < size; i++) {
		char letter = gameOver1stLine[i];
		render_letter(letter, lettersTexture);
		glPushMatrix();
		glTranslatef(fx,fy,0);
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
		fx += 20;
	}
	string gameOver2ndLine = "You scored " + to_string(playerScore) + " in " + finalTime;
	size = strlen(gameOver2ndLine.c_str());
	fx = (float)xres/2-220;
	for (int i = 0; i < size; i++) {
		char letter = gameOver2ndLine[i];
		glPushMatrix();
		glTranslatef(fx,fy-50,0);
		if (letter == '0' || letter == '1' || letter == '2' ||
				letter == '3' || letter == '4' || letter == '5' ||
				letter == '6' || letter == '7' || letter == '8' ||
				letter == '9' || letter == ':') {
			render_number(letter, numbersTexture);
		} else {
			render_letter(letter, lettersTexture);
		}
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
		fx += 20;
	}
	const char *gameOver3rdLine = "Please type your "
		"initials";
	size = strlen(gameOver3rdLine);
	fx = (float)xres/2-240;
	for (int i = 0; i < size; i++) {
		char letter = gameOver3rdLine[i];
		glPushMatrix();
		glTranslatef(fx,fy-100,0);
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
		fx += 20;
	}
}

char store_initials(XEvent *e)
{
	char initial;
	int key = XLookupKeysym(&e->xkey, 0);
	switch (key) {
		case(XK_A):
		case(XK_a):
			initial = 'A';
			break;
		case(XK_B):
		case(XK_b):
			initial = 'B';
			break;
		case(XK_C):
		case(XK_c):
			initial = 'C';
			break;
		case(XK_D):
		case(XK_d):
			initial = 'D';
			break;
		case(XK_E):
		case(XK_e):
			initial = 'E';
			break;
		case(XK_F):
		case(XK_f):
			initial = 'F';
			break;
		case(XK_G):
		case(XK_g):
			initial = 'G';
			break;
		case(XK_H):
		case(XK_h):
			initial = 'H';
			break;
		case(XK_I):
		case(XK_i):
			initial = 'I';
			break;
		case(XK_L):
		case(XK_l):
			initial = 'L';
			break;
		case(XK_M):
		case(XK_m):
			initial = 'M';
			break;
		case(XK_N):
		case(XK_n):
			initial = 'N';
			break;
		case(XK_O):
		case(XK_o):
			initial = 'O';
			break;
		case(XK_P):
		case(XK_p):
			initial = 'P';
			break;
		case(XK_Q):
		case(XK_q):
			initial = 'Q';
			break;
		case(XK_R):
		case(XK_r):
			initial = 'R';
			break;
		case(XK_S):
		case(XK_s):
			initial = 'S';
			break;
		case(XK_T):
		case(XK_t):
			initial = 'T';
			break;
		case(XK_U):
		case(XK_u):
			initial = 'U';
			break;
		case(XK_V):
		case(XK_v):
			initial = 'V';
			break;
		case(XK_W):
		case(XK_w):
			initial = 'W';
			break;
		case(XK_X):
		case(XK_x):
			initial = 'X';
			break;
		case(XK_Y):
		case(XK_y):
			initial = 'Y';
			break;	
		default:
			initial = 'Z';
	}	 
	return initial;
}

void print_initials(int xres, int yres, GLuint lettersTexture[], char initial, bool first) {
	static int wid = 60;
	float fx = (first) ? (float)xres/2-60 : (float)xres/2+60;
	float fy = (float)yres/2;
	glPushMatrix();
	glTranslatef(fx,fy,0);
	render_letter(initial, lettersTexture);
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

string final_time() 
{
	string time;
	auto currentTime = chrono::system_clock::now();
	chrono::duration<double> finalTime = currentTime - startTime;
	int minutes = (int)finalTime.count() / 60;
	int seconds = (int)finalTime.count() % 60;
	string seconds_string = (seconds < 10) 
		? "0" + to_string(seconds) 
		: to_string(seconds);
	time = to_string(minutes) + ":" + seconds_string;
	return time;
}

void insert_into_database(char firstInitial, char secondInitial, 
		string score, string time) 
{
	string first, second;
	stringstream ss,ss1;
	ss << firstInitial;
	ss >> first;
	ss1 << secondInitial;
	ss1 >> second;
	if (init_connection()) {
		string query = "INSERT INTO Leaderboard"
			"(Name, Score, Time)"
			"Values ('" + first + second + "', " + score +
			", '" + time + "')";
		result = sqlite3_exec(db, query.c_str(), callback, 
				(void*) data, &ErrMsg);
		//#ifdef SQL_UNIT_TEST
		if (result != SQLITE_OK) {
			cout << "SQL Error:" << ErrMsg << endl;
			sqlite3_free(ErrMsg);
		} else {
			cout << "Insert statement successful" << endl;
		}
		//#endif
	}
}

void get_ranking(char firstInitial, char secondInitial, 
		string playerScore, string final_time)
{
	string first, second;
	stringstream ss,ss1;
	ss << firstInitial;
	ss >> first;
	ss1 << secondInitial;
	ss1 >> second;
	if (init_connection()) {
		string query = "select name,score,time, 1+(select count(*) "
			"from leaderboard l2 where l2.score > l1.score) as rank "
			"from leaderboard l1 where name = '" + first + second + 
			"' and score = '" + playerScore + "' and time = '" +
			final_time + "';";
		result = sqlite3_exec(db, query.c_str(), callback2, 
				(void*) data, &ErrMsg);
		//#ifdef SQL_UNIT_TEST
		if (result != SQLITE_OK) {
			cout << "SQL Error get_ranking:" << ErrMsg << endl;
			sqlite3_free(ErrMsg);
		} else {
			cout << "Retrieval statement successful" << endl;
		}
		//#endif
	}
}

void ranking_text(int xres, int yres, GLuint lettersTexture[])
{
	static int wid = 20;
	float fx = (float)xres/2-200;
	float fy = (float)yres/2+100;
	string rankStatement = "You are ranked number";
	int size = rankStatement.length();
	for (int i = 0; i < size; i++) {
		char letter = rankStatement[i];
		render_letter(letter, lettersTexture);
		glPushMatrix();
		glTranslatef(fx,fy,0);
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
		fx += 20;
	}
}

int callback2(void *data, int argc, char **argv, char **azColName)
{
	(void) data;
	(void) argc;
	(void) azColName;
	char* argv_rank= (char*)calloc(30, sizeof(char));
	strcpy(argv_rank, argv[3]);
	int size = strlen(argv_rank);
	for (int i = 0; i < size; i++) {
		ranking_vector.insert(ranking_vector.end(),argv_rank[i]);
	}
	free(argv_rank);
	return SQLITE_OK;
}

void print_ranking(int xres, int yres, GLuint numbersTexture[])
{
	static int wid = 100;
	int size = ranking_vector.size();
	float fx;
	float fy = (float)yres/2;
	if (size == 2) {
		fx = (float)xres/2-50;
	} else if (size == 3) {
		fx = (float)xres/2-75;
	} else if (size == 4) {
		fx = (float)xres/2-100;
	} else {
		fx = (float)xres/2;
	}
	for (int i = 0; i < size; i++) {
		char number = ranking_vector[i];
		render_number(number, numbersTexture);
		glPushMatrix();
		glTranslatef(fx,fy,0);
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
		fx += 100;
	}
}

extern void print_game_over_input(int xres, int yres, GLuint lettersTexture[]) 
{
	static int wid = 20;
	float fx = (float)xres/2-150;
	float fy = (float)yres/2-200;
	int size;
	string gameOver2ndLine = "ESC - Quit Game";
	size = strlen(gameOver2ndLine.c_str());
	fx = (float)xres/2-150;
	for (int i = 0; i < size; i++) {
		char letter = gameOver2ndLine[i];
		glPushMatrix();
		glTranslatef(fx,fy-50,0);
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
		fx += 20;
	}	
}

void menu(int xres, int yres, GLuint menuTexture) 
{
	static int wid = xres/2;
	float fx = (float)xres/2;
	float fy = (float)yres/2;
	glPushMatrix();
	glTranslatef(fx,fy,0);
	glBindTexture(GL_TEXTURE_2D, menuTexture);
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

void menu_text(int xres, int yres, GLuint lettersTexture[]) 
{
	int wid = 50;	
	float fx = (float)xres/2-300;
	float fy = (float)yres-50;
	const char *menu1stLine = "Game Controls";
	int size = strlen(menu1stLine);
	for (int i = 0; i < size; i++) {
		char letter = menu1stLine[i];
		render_letter(letter, lettersTexture);
		glPushMatrix();
		glTranslatef(fx,fy,0);
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
		fx += 50;
	}
	wid = 20;
	fy -= 50;
	string game_control = "left arrow - move character left";
	size = strlen(game_control.c_str());
	fx = (float)xres/2-300;
	fy -= 50;
	for (int i = 0; i < size; i++) {
		char letter = game_control[i];
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
		fx += 20;
	}
	game_control = "right arrow - move character right";
	size = strlen(game_control.c_str());
	fx = (float)xres/2-300;
	fy -= 50;
	for (int i = 0; i < size; i++) {
		char letter = game_control[i];
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
		fx += 20;
	}
	game_control = "r - shoot up";
	size = strlen(game_control.c_str());
	fx = (float)xres/2-300;
	fy -= 50;
	for (int i = 0; i < size; i++) {
		char letter = game_control[i];
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
		fx += 20;
	}
	game_control = "g - shoot right";
	size = strlen(game_control.c_str());
	fx = (float)xres/2-300;
	fy -= 50;
	for (int i = 0; i < size; i++) {
		char letter = game_control[i];
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
		fx += 20;
	}
	game_control = "v - shoot down";
	size = strlen(game_control.c_str());
	fx = (float)xres/2-300;
	fy -= 50;
	for (int i = 0; i < size; i++) {
		char letter = game_control[i];
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
		fx += 20;
	}
	game_control = "d - shoot left";
	size = strlen(game_control.c_str());
	fx = (float)xres/2-300;
	fy -= 50;
	for (int i = 0; i < size; i++) {
		char letter = game_control[i];
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
		fx += 20;
	}

	game_control = "r and g - shoot diag right";
	size = strlen(game_control.c_str());
	fx = (float)xres/2-300;
	fy -= 50;
	for (int i = 0; i < size; i++) {
		char letter = game_control[i];
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
		fx += 20;
	}
}

void print_menu(int xres, int yres, GLuint lettersTexture[]) 
{
	float fx = (float)xres-280;
	float fy = (float)yres-550;
	int wid = 20;
	string menu = "CTRL-L - Menu";
	int size = strlen(menu.c_str());
	for (int i = 0; i < size; i++) {
		glPushMatrix();
		glTranslatef(fx,fy,0);
		render_letter(menu[i], lettersTexture);
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
		fx += 20;
	} 
}

int show_cpu_usage()
{
	ifstream fileStat("/proc/stat");
	string line;
	string data[11];
	bool first_line = 1;
	while (getline(fileStat,line) && first_line) {
		first_line = false;
		string stats;
		int position = 0;
		stringstream ss(line);
		while (ss >> stats) {
			data[position] = stats;
			position++;
		}
	}
	int cpu_time = stoll(data[1]) + stoll(data[2]) + stoll(data[3]) +
		stoll(data[4]) + stoll(data[5]) + stoll(data[6]) + 
		stoll(data[7]) + stoll(data[8]);	
	int cpu_idle = stoll(data[4]) + stoll(data[5]);
	int cpu_usage = cpu_time - cpu_idle;
	double cpu_percentage = (double)cpu_usage/cpu_time * 100;
	return cpu_percentage;
}

void print_cpu_usage(int cpu_usage, int xres, int yres, GLuint lettersTexture[],
		GLuint numbersTexture[]) {
	static int wid = 15;
	float fx = (float)xres-280;
	float fy = (float)yres-50;
	string usageStatement = "CPU Usage - ";
	int size = usageStatement.length();
	for (int i = 0; i < size; i++) {
		char letter = usageStatement[i];
		render_letter(letter, lettersTexture);
		glPushMatrix();
		glTranslatef(fx,fy,0);
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
		fx += 15;
	}
	string usage = to_string(cpu_usage);
	size = usage.length();
	for (int i = 0; i < size; i++) {
		char number= usage[i];
		render_number(number, numbersTexture);
		glPushMatrix();
		glTranslatef(fx,fy,0);
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
		fx += 15;
	}	
}
