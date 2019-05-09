#ifndef _EMILM_H_
#define _EMILM_H_

#include <GL/glx.h>
#include <string>

using namespace std;

//#define SQL_UNIT_TEST

extern void start_menu(int xres, int yres, GLuint startMenuTexture);
extern void show_logo(int xres, int yres, GLuint logoTexture);
extern void show_keys(int xres, int yres, GLuint lettersTexture[]);
extern void leaderboard(int xres, int yres, GLuint leaderboardTexture);
extern void leaderboard_title(int xres, int yres, GLuint leaderboardTitleTexture);
int init_connection();
#ifdef SQL_UNIT_TEST
void create_table();
void insert();
#endif
extern void generate_leaderboard();
int callback(void *data, int argc, char **argv, char **azColName);
int callback2(void *data, int argc, char **argv, char **azColName);
void print_leaderboard(int xres, int yres, GLuint numbersTexture[],
    GLuint lettersTexture[]);
void leaderboard_name(int xres, int yres, int wid, string name, 
    GLuint lettersTexture[]);
void leaderboard_time(int xres, int yres, int wid, string time, 
    GLuint numbersTexture[]);
void leaderboard_score(int xres, int yres, int wid, string score, 
    GLuint numbersTexture[]);
void render_letter(char letter, GLuint lettersTexture[]);
void render_number(char number, GLuint numbersTexture[]);
void render_number(int number, GLuint numbersTexture[]);
extern void start_time();
extern void print_time(int yres, GLuint numbersTexture[]);
extern void print_score(int gameScore, int xres, int yres, 
    GLuint numbersTexture[]);
extern void game_over(int xres, int yres, GLuint gameOverTexture);
extern void game_over_text(int xres, int yres, int playerScore, 
    string finalTime, 
    GLuint lettersTexture[], GLuint numbersTexture[]);
extern char store_initials(XEvent *e);
extern void print_initials(int xres, int yres, GLuint lettersTexture[], 
    char initial, bool first);
extern string final_time();
extern void insert_into_database(char firstInitial, char secondInitial, 
    string score, string time);
extern void get_ranking(char firstInitial, char secondInitial, 
    string playerScore, string final_time);
extern void ranking_text(int xres, int yres, GLuint lettersTexture[]);
extern void print_ranking(int xres, int yres, GLuint numbersTexture[]);
extern void print_game_over_input(int xres, int yres, GLuint lettersTexture[]);

#endif
