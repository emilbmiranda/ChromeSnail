#ifndef _EMILM_H_
#define _EMILM_H_

#include <GL/glx.h>

//#define SQL_UNIT_TEST

extern void start_menu(int xres, int yres, GLuint startMenuTexture);
extern void show_logo(int xres, int yres, GLuint logoTexture);
extern void show_keys(int xres, int yres, GLuint keyTexture);
extern void leaderboard(int xres, int yres, GLuint leaderboardTexture);
extern void leaderboard_title(int xres, int yres, GLuint leaderboardTitleTexture);
extern void leaderboard_box(int xres, int yres, GLuint leaderboardBoxTexture);
int init_connection();
#ifdef SQL_UNIT_TEST
void create_table();
void insert();
#endif
extern void generate_leaderboard(int xres, int yres);
int callback(void *data, int argc, char **argv, char **azColName);
void print_leaderboard(int xres, int yres);

#endif