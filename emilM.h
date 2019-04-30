#ifndef _EMILM_H_
#define _EMILM_H_

#include <X11/Xlib.h>
#include <GL/glx.h>

extern void start_menu(int xres, int yres, GLuint startMenuTexture);
extern void show_logo(int xres, int yres, GLuint logoTexture);
extern void show_keys(int xres, int yres, GLuint keyTexture);

#endif