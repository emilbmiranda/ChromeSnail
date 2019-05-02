//
//program: background.cpp
//author:  Gordon Griesel
//date:    2017 - 2018
//modified by: Hasun Khan
//date:		Spring 2019
//
//The position of the background QUAD does not change.
//Just the texture coordinates change.
//In this example, only the x coordinates change.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>

//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];

//macros
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(v, x, y, z) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
                      (c)[1]=(a)[1]-(b)[1]; \
                      (c)[2]=(a)[2]-(b)[2]

class Image {
public:
	int width, height;
	unsigned char *data;
	~Image() { delete [] data; }
	Image(const char *fname) {
		if (fname[0] == '\0')
			return;
		//printf("fname **%s**\n", fname);
		char name[40];
		strcpy(name, fname);
		int slen = strlen(name);
		name[slen-4] = '\0';
		//printf("name **%s**\n", name);
		char ppmname[80];
		sprintf(ppmname,"%s.ppm", name);
		//printf("ppmname **%s**\n", ppmname);
		char ts[100];
		//system("convert eball.jpg eball.ppm");
		sprintf(ts, "convert %s %s", fname, ppmname);
		system(ts);
		//sprintf(ts, "%s", name);
		FILE *fpi = fopen(ppmname, "r");
		if (fpi) {
			char line[200];
			fgets(line, 200, fpi);
			fgets(line, 200, fpi);
			//skip comments and blank lines
			while (line[0] == '#' || strlen(line) < 2)
				fgets(line, 200, fpi);
			sscanf(line, "%i %i", &width, &height);
			fgets(line, 200, fpi);
			//get pixel data
			int n = width * height * 3;			
			data = new unsigned char[n];			
			for (int i=0; i<n; i++)
				data[i] = fgetc(fpi);
			fclose(fpi);
		} else {
			printf("ERROR opening image: %s\n",ppmname);
			exit(0);
		}
		unlink(ppmname);
	}
};

class Sprite {
public:
	int onoff;
	int frame;
	double delay;
	Vec pos;
	Image *image;
	GLuint tex;
	struct timespec time;
	Sprite() {
		onoff = 0;
		frame = 0;
		image = NULL;
		delay = 0.1;
	}
};


//Setup timers
class Timers {
public:
	double physicsRate;
	double oobillion;
	struct timespec timeStart, timeEnd, timeCurrent;
	struct timespec walkTime;
	Timers() {
		physicsRate = 1.0 / 30.0;
		oobillion = 1.0 / 1e9;
	}
	double timeDiff(struct timespec *start, struct timespec *end) {
		return (double)(end->tv_sec - start->tv_sec ) +
				(double)(end->tv_nsec - start->tv_nsec) * oobillion;
	}
	void timeCopy(struct timespec *dest, struct timespec *source) {
		memcpy(dest, source, sizeof(struct timespec));
	}
	void recordTime(struct timespec *t) {
		clock_gettime(CLOCK_REALTIME, t);
	}
} timers;


Image img[2] = {"Mission_1.png","go.gif"};

class Texture {
public:
	Image *backImage;
	GLuint backTexture;
	float xc[2];
	float yc[2];
	Image *goImage;
	GLuint goTexture; 
};

class Global {
public:
	int xres, yres;
	bool GoEvent;
	Sprite exp;
	Texture tex;
	Global() {
		GoEvent = false; 
		exp.onoff= 0;
		exp.frame = 0;
		exp.image = NULL;
		exp.delay = 0.02; 
		xres=640, yres=480;
	}
}g;

class X11_wrapper {
private:
	Display *dpy;
	Window win;
	GLXContext glc;
public:
	X11_wrapper() {
		GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
		//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
		setup_screen_res(640, 480);
		dpy = XOpenDisplay(NULL);
		if(dpy == NULL) {
			printf("\n\tcannot connect to X server\n\n");
			exit(EXIT_FAILURE);
		}
		Window root = DefaultRootWindow(dpy);
		XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
		if(vi == NULL) {
			printf("\n\tno appropriate visual found\n\n");
			exit(EXIT_FAILURE);
		} 
		Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
		XSetWindowAttributes swa;
		swa.colormap = cmap;
		swa.event_mask =
			ExposureMask | KeyPressMask | KeyReleaseMask | PointerMotionMask |
			ButtonPressMask | ButtonReleaseMask |
			StructureNotifyMask | SubstructureNotifyMask;
		win = XCreateWindow(dpy, root, 0, 0, g.xres, g.yres, 0,
								vi->depth, InputOutput, vi->visual,
								CWColormap | CWEventMask, &swa);
		set_title();
		glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
		glXMakeCurrent(dpy, win, glc);
	}
	void cleanupXWindows() {
		XDestroyWindow(dpy, win);
		XCloseDisplay(dpy);
	}
	void setup_screen_res(const int w, const int h) {
		g.xres = w;
		g.yres = h;
	}
	void reshape_window(int width, int height) {
		//window has been resized.
		setup_screen_res(width, height);
		glViewport(0, 0, (GLint)width, (GLint)height);
		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
		glOrtho(0, g.xres, 0, g.yres, -1, 1);
		set_title();
	}
	void set_title() {
		//Set the window title bar.
		XMapWindow(dpy, win);
		XStoreName(dpy, win, "scrolling background (seamless)");
	}
	bool getXPending() {
		return XPending(dpy);
	}
	XEvent getXNextEvent() {
		XEvent e;
		XNextEvent(dpy, &e);
		return e;
	}
	void swapBuffers() {
		glXSwapBuffers(dpy, win);
	}
	void check_resize(XEvent *e) {
		//The ConfigureNotify is sent by the
		//server if the window is resized.
		if (e->type != ConfigureNotify)
			return;
		XConfigureEvent xce = e->xconfigure;
		if (xce.width != g.xres || xce.height != g.yres) {
			//Window size did change.
			reshape_window(xce.width, xce.height);
		}
	}
} x11;

void init_opengl(void);
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics(void);
void render(void);


//===========================================================================
//===========================================================================
int main()
{
	init_opengl();
	int done=0;
	while (!done) {
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.check_resize(&e);
			check_mouse(&e);
			done = check_keys(&e);
		}
		physics();
		render();
		x11.swapBuffers();
	}
	return 0;
}

unsigned char *buildAlphaData(Image *img)
{
	//add 4th component to RGB stream...
	int i;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	unsigned char a,b,c;
	//use the first pixel in the image as the transparent color.
	unsigned char t0 = *(data+0);
	unsigned char t1 = *(data+1);
	unsigned char t2 = *(data+2);
	for (i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		*(ptr+3) = 1;
		if (a==t0 && b==t1 && c==t2)
			*(ptr+3) = 0;
		//-----------------------------------------------
		ptr += 4;
		data += 3;
	}
	return newdata;
}




void init_opengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, g.xres, g.yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, g.xres, 0, g.yres, -1, 1);
	//Clear the screen
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT);
	//Do this to allow texture maps
	glEnable(GL_TEXTURE_2D);
	//
	//load the images file into a ppm structure.
	//
	g.tex.backImage = &img[0];
	//create opengl texture elements
	glGenTextures(1, &g.tex.backTexture);
	int w = g.tex.backImage->width;
	int h = g.tex.backImage->height;
	glBindTexture(GL_TEXTURE_2D, g.tex.backTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
							GL_RGB, GL_UNSIGNED_BYTE, g.tex.backImage->data);
	g.tex.xc[0] = 0.0;
	g.tex.xc[1] = 0.25;
	g.tex.yc[0] = 0.0;
	g.tex.yc[1] = 1.0;

	//create opengl texture elements
	w = img[1].width;
	h = img[1].height;
	glGenTextures(1, &g.exp.tex);
	//-------------------------------------------------------------------------
	
	g.tex.goImage = &img[1];
	//create opengl texture elements
	glGenTextures(1, &g.tex.goTexture);
	w = g.tex.goImage->width;
	h = g.tex.goImage->height;
	//this is similar to a sprite graphic
	glBindTexture(GL_TEXTURE_2D, g.exp.tex);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//must build a new set of data...
	unsigned char *xData = buildAlphaData(&img[1]);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, xData);
	free(xData);

}

void check_mouse(XEvent *e)
{
	//Did the mouse move?
	//Was a mouse button clicked?
	static int savex = 0;
	static int savey = 0;
	//
	if (e->type == ButtonRelease) {
			g.GoEvent = false;		
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
		}
		if (e->xbutton.button==3) {
			//Right button is down
		}
	}
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		savex = e->xbutton.x;
		savey = e->xbutton.y;
	}
}

int check_keys(XEvent *e)
{
	//Was there input from the keyboard?
	if (e->type == KeyPress) {
		int key = XLookupKeysym(&e->xkey, 0);
		if (key == XK_Escape) {
			return 1;
		}
		if (key == XK_g) { 
			g.GoEvent = true; 
		}
		if (key == XK_s) { 
			g.GoEvent = false;
			g.exp.pos[0] = 200.0; 
			g.exp.pos[1] = -60.0;
			timers.recordTime(&g.exp.time);
			g.exp.onoff ^= 1;
		}
	}
	return 0;
}

void physics()
{
	//move the background
	if(g.GoEvent == True) { 
		g.tex.xc[0] += 0.001;
		g.tex.xc[1] += 0.001;
	}
	if (g.exp.onoff) { 
		g.exp.pos[0] -= 2.0 * (0.05/g.exp.delay); 
	}
	if (g.exp.onoff) { 
		//explosion is happening 
		timers.recordTime(&timers.timeCurrent); 
		double timeSpan = timers.timeDiff(&g.exp.time, &timers.timeCurrent);
		
		if (timeSpan > g.exp.delay) {
			++g.exp.frame;
			if (g.exp.frame >= 14) {
				g.exp.onoff = 0;
				g.exp.frame = 0;
			} 
			else { 
				timers.recordTime(&g.exp.time); 
			}
		} 
	} 
} 

void render()
{
	float cx = g.xres/2.0;
	float cy = g.yres/2.0;
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, g.tex.backTexture);
	glBegin(GL_QUADS);
		glTexCoord2f(g.tex.xc[0], g.tex.yc[1]); glVertex2i(0, 0);
		glTexCoord2f(g.tex.xc[0], g.tex.yc[0]); glVertex2i(0, g.yres);
		glTexCoord2f(g.tex.xc[1], g.tex.yc[0]); glVertex2i(g.xres, g.yres);
		glTexCoord2f(g.tex.xc[1], g.tex.yc[1]); glVertex2i(g.xres, 0);
	glEnd();
	/*if (g.exp.onoff) {
		float h = 80.0;
		float w = 80.0;
		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, g.exp.tex);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glColor4ub(255,255,255,255); //255 originally
		glTranslated(g.exp.pos[0], g.exp.pos[1], g.exp.pos[2]);
		int ix = g.exp.frame % 16;
		int iy = g.exp.frame / 16;
		float tx = (float)ix / 8.0;
		float ty = (float)iy / 5.0;
		glBegin(GL_QUADS);
			glTexCoord2f(tx,     ty+0.2); glVertex2i(cx-w, cy-h);
			glTexCoord2f(tx,     ty);     glVertex2i(cx-w, cy+h);
			glTexCoord2f(tx+0.2, ty);     glVertex2i(cx+w, cy+h);
			glTexCoord2f(tx+0.2, ty+0.2); glVertex2i(cx+w, cy-h);
		glEnd();
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_ALPHA_TEST);

	}*/
}














