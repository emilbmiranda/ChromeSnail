//3350
//program: walk2.cpp
//author:  Gordon Griesel
//date:    summer 2017
//         spring 2018
//
//modified by: mason pawsey, Victor Merino, Fernando Herrera, Emil Miranda, Hasun Khan
//modified date: Spring 2019
//
//Walk cycle using a sprite sheet.
//images courtesy: http://games.ucla.edu/resource/walk-cycles/
//
//This program includes:
//  multiple sprite-sheet animations
//  a level tiling system
//  parallax scrolling of backgrounds
//
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
//#include "ppm.h"
#include "fonts.h"
#include "victorM.h"
#include "fernandoH.h"
#include "emilM.h"
#include "masonP.h"
#include <dirent.h>

using namespace std;

//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];
time_t start = time(0);
//macros
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(v, x, y, z) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
                             (c)[1]=(a)[1]-(b)[1]; \
(c)[2]=(a)[2]-(b)[2]
//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define ALPHA 1
#define MASON 0
#define FERNANDO 1
#define HASUN 2
#define VICTOR 3
#define EMIL 4
#define NUMBERS_ARRAY 11
#define LETTERS_ARRAY 28

//function prototypes
void initOpengl();
void checkMouse(XEvent *e);
int checkKeys(XEvent *e);
void setHelicopterPos(float pos);
float lastKnownHelicopterPos();
void init();
void physics();
void render();

//-----------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------

class Image;

class Walker {
    public:
        Vec pos = {0.00, 525.00};
        Vec vel = {6.00};
        int dir = 0;
} walker;

class Helicopter {
    public:
        Vec pos = {100.00, 525.00};
        Vec vel = {4.00};
} helicopter;

class Bomb {
    public:
        Vec pos = {100.00, 570.00};
        Vec vel = {4.00, 3.00};
} bomb;

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

//Bullets bullets;
BList bullets;




// Global is using the singleton pattern
class Global {
   public:
        bool Forward = false; 
        bool Backward = false;
        unsigned char keys[65536];
        int xres, yres;
        int movie, movieStep;
        int walk;
        int walkFrame;
        int showCredits;
        int displayHelicopter;
        int showStartMenu;
        int dropBomb = 0;
        int showCrate;
        int showLeaderboard;
        int playerScore = 0;
        int helicopterHealth = 5;
        int startGame;
        int health = 99;
        int done = 0;
        double delay;
        float xc[2];
        float yc[2];
        Image *walkImage;
        GLuint walkTexture;
        GLuint creditPicsTexture[5];
        GLuint helicopterTexture;
        GLuint bombTexture;
        GLuint startMenuTexture;
        GLuint logoTexture;
        GLuint keysTexture;
        GLuint leaderboardTexture;
        GLuint leaderboardTitleTexture;
        GLuint leaderboardBoxTexture;
        GLuint numbersTexture[NUMBERS_ARRAY];
        GLuint lettersTexture[LETTERS_ARRAY];
        GLuint timeTexture;
        // Fernando: Need to create a GLuint object for the crate texture.
        GLuint crateTexture;
        //Platform plat1(540,140);
        Platform plat1;
        GLuint backgroundTexture;
        Vec box[20];
        Sprite exp;
        Sprite exp44;
        Vec ball_pos;
        Vec ball_vel;
        //camera is centered at (0,0) lower-left of screen. 
        Flt camera[2];
        ~Global() {
            logClose();
        }

        static Global& getInstance() 
        {
            static Global _gInstance;
            return _gInstance;
        }

    private:
        // constructor is private
        Global() {
            logOpen();
            camera[0] = camera[1] = 0.0;
            movie=0;
            movieStep=2;
            xres=800;
            yres=600;
            walk=0;
            walkFrame=0;
            walkImage=NULL;
            MakeVector(ball_pos, 520.0, 0, 0);
            MakeVector(ball_vel, 0, 0, 0);
            delay = 0.1;
            exp.onoff=0;
            exp.frame=0;
            exp.image=NULL;
            exp.delay = 0.02;
            exp44.onoff=0;
            exp44.frame=0;
            exp44.image=NULL;
            exp44.delay = 0.022;
            showCredits = 0;
            displayHelicopter = 1;
            showStartMenu = 1;
            showCrate = 1;
            startGame = 0;
            for (int i=0; i<20; i++) {
                box[i][0] = rnd() * xres;
                box[i][1] = rnd() * (yres-220) + 220.0;
                box[i][2] = 0.0;
            }
            for (int i = 0; i < 5; i++) {
                creditPicsTexture[i] = 0;
            }
            memset(keys, 0, 65536);
        };
        // copy constructor private
        Global(Global const&){};
        // assignment operator is private
        Global& operator=(Global const&);
};

int helicopterHit()
{
    return --Global::getInstance().helicopterHealth;
}

void helicopterHealthReset()
{
    Global::getInstance().helicopterHealth = 5;
}

void playerScoreIncrease()
{
    Global::getInstance().playerScore++;
}

class Level {
    public:
        unsigned char arr[16][80];
        int nrows, ncols;
        int tilesize[2];
        Flt ftsz[2];
        Flt tile_base;
        Level() {
            //Log("Level constructor\n");
            tilesize[0] = 32;
            tilesize[1] = 32;
            ftsz[0] = (Flt)tilesize[0];
            ftsz[1] = (Flt)tilesize[1];
            tile_base = 220.0;
            //read level
            FILE *fpi = fopen("level1.txt","r");
            if (fpi) {
                nrows=0;
                char line[100];
                while (fgets(line, 100, fpi) != NULL) {
                    removeCrLf(line);
                    int slen = strlen(line);
                    ncols = slen;
                    //Log("line: %s\n", line);
                    for (int j=0; j<slen; j++) {
                        arr[nrows][j] = line[j];
                    }
                    ++nrows;
                }
                fclose(fpi);
                //printf("nrows of background data: %i\n", nrows);
            }
            for (int i=0; i<nrows; i++) {
                for (int j=0; j<ncols; j++) {
                    printf("%c", arr[i][j]);
                }
                printf("\n");
            }
        }
        void removeCrLf(char *str) {
            //remove carriage return and linefeed from a Cstring
            char *p = str;
            while (*p) {
                if (*p == 10 || *p == 13) {
                    *p = '\0';
                    break;
                }
                ++p;
            }
        }
} lev;

//X Windows variables
class X11_wrapper {
    private:
        Display *dpy;
        Window win;
    public:
        ~X11_wrapper() {
            XDestroyWindow(dpy, win);
            XCloseDisplay(dpy);
        }
        void setTitle() {
            //Set the window title bar.
            XMapWindow(dpy, win);
            XStoreName(dpy, win, "Chrome Snail");
        }
        void setupScreenRes(const int w, const int h) {
            Global::getInstance().xres = w;
            Global::getInstance().yres = h;
        }
        X11_wrapper() {
            GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
            //GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
            XSetWindowAttributes swa;
            setupScreenRes(Global::getInstance().xres, Global::getInstance().yres);
            dpy = XOpenDisplay(NULL);
            if (dpy == NULL) {
                printf("\n\tcannot connect to X server\n\n");
                exit(EXIT_FAILURE);
            }
            Window root = DefaultRootWindow(dpy);
            XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
            if (vi == NULL) {
                printf("\n\tno appropriate visual found\n\n");
                exit(EXIT_FAILURE);
            }
            Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
            swa.colormap = cmap;
            swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
                ButtonPressMask | ButtonReleaseMask |
                StructureNotifyMask | SubstructureNotifyMask;
            win = XCreateWindow(dpy, root, 0, 0, 
                    Global::getInstance().xres, 
                    Global::getInstance().yres, 0,
                    vi->depth, InputOutput, vi->visual,
                    CWColormap | CWEventMask, &swa);
            GLXContext glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
            glXMakeCurrent(dpy, win, glc);
            setTitle();
        }
        void reshapeWindow(int width, int height) {
            //window has been resized.
            setupScreenRes(width, height);
            glViewport(0, 0, (GLint)width, (GLint)height);
            glMatrixMode(GL_PROJECTION); glLoadIdentity();
            glMatrixMode(GL_MODELVIEW); glLoadIdentity();
            glOrtho(0, Global::getInstance().xres, 
                    0, Global::getInstance().yres, -1, 1);
            setTitle();
        }
        void checkResize(XEvent *e) {
            //The ConfigureNotify is sent by the
            //server if the window is resized.
            if (e->type != ConfigureNotify)
                return;
            XConfigureEvent xce = e->xconfigure;
            if (xce.width != Global::getInstance().xres 
                    || xce.height != Global::getInstance().yres) {
                //Window size did change.
                reshapeWindow(xce.width, xce.height);
            }
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
} x11;

class Image {
    public:
        int width, height;
        unsigned char *data;
        ~Image() { delete [] data; }
        Image(const char *fname) {
            if (fname[0] == '\0')
                return;
            //printf("fname **%s**\n", fname);
            int ppmFlag = 0;
            char name[40];
            strcpy(name, fname);
            int slen = strlen(name);
            char ppmname[80];
            if (strncmp(name+(slen-4), ".ppm", 4) == 0)
                ppmFlag = 1;
            if (ppmFlag) {
                strcpy(ppmname, name);
            } else {
                name[slen-4] = '\0';
                //printf("name **%s**\n", name);
                sprintf(ppmname,"%s.ppm", name);
                //printf("ppmname **%s**\n", ppmname);
                char ts[100];
                //system("convert eball.jpg eball.ppm");
                sprintf(ts, "convert %s %s", fname, ppmname);
                system(ts);
            }
            //sprintf(ts, "%s", name);
            //printf("read ppm **%s**\n", ppmname); fflush(stdout);
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
            if (!ppmFlag)
                unlink(ppmname);
        }
};
Image img[9] = {
    "./images/walk.gif",
    "./images/exp.png",
    "./images/exp44.png",
    "./images/masonp.jpg",
    "./images/fh.jpg",
    "./images/hasunK.jpg",
    "./images/VictorM.jpg",
    "./images/EmilM.jpeg" ,
    "./images/mission_3.png"};

Image helicopter_image = "./images/helicopter.gif";
Image bomb_image = "./images/bomb.gif";
Image start_menu_image = "./images/StartMenu.jpg";
Image logo_image = "./images/Logo.gif";
Image keys_image = "./images/Keys.gif"; 
// Fernando: Create Image object that references .jpg
Image crate_image = "./images/wall.gif";
Image leaderboard_image = "./images/Leaderboard.gif";
Image leaderboard_title_image = "./images/LeaderboardTitle.gif";
Sprite numbers_spritesheet;
Image letters_image[LETTERS_ARRAY] = {"./images/A.gif", "./images/B.gif",
    "./images/C.gif", "./images/D.gif",
    "./images/E.gif", "./images/F.gif",
    "./images/G.gif", "./images/H.gif",
    "./images/I.gif", "./images/J.gif",
    "./images/K.gif", "./images/L.gif",
    "./images/M.gif", "./images/N.gif",
    "./images/O.gif", "./images/P.gif",
    "./images/Q.gif", "./images/R.gif",
    "./images/S.gif", "./images/T.gif",
    "./images/U.gif", "./images/V.gif",
    "./images/W.gif", "./images/X.gif",
    "./images/Y.gif", "./images/Z.gif",
    "./images/hypen.gif", "./images/space.gif"};
Image numbers_image[NUMBERS_ARRAY] = {"./images/0.gif", "./images/1.gif",
    "./images/2.gif", "./images/3.gif",
    "./images/4.gif", "./images/5.gif",
    "./images/6.gif", "./images/7.gif",
    "./images/8.gif", "./images/9.gif",
    "./images/colon.gif"};
Image time_image = "./images/Time.gif";
Image *backImage = &img[8];
void show_credits(Rect x, int y);

int main(void)
{
    initOpengl();
    init();
    while (!Global::getInstance().done) {
        while (x11.getXPending()) {
            XEvent e = x11.getXNextEvent();
            x11.checkResize(&e);
            checkMouse(&e);
            Global::getInstance().done = checkKeys(&e);
        }
        physics();
        render();
        x11.swapBuffers();
    }
    cout << "Game finished" << endl;
    cleanup_fonts();
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

void initOpengl(void)
{
    //OpenGL initialization
    glViewport(0, 0, 
            Global::getInstance().xres, Global::getInstance().yres);
    //Initialize matrices
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //This sets 2D mode (no perspective)
    glOrtho(0, Global::getInstance().xres, 
            0, Global::getInstance().yres, -1, 1);
    //
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_FOG);
    glDisable(GL_CULL_FACE);
    //
    //Clear the screen
    glClearColor(1.0, 1.0, 1.0, 1.0);
    //glClear(GL_COLOR_BUFFER_BIT);
    //Do this to allow fonts
    glEnable(GL_TEXTURE_2D);
    initialize_fonts();
    //
    //load the images file into a ppm structure.
    //
    int w = img[0].width;
    int h = img[0].height;
    //
    //
    //create opengl texture elements
    glGenTextures(1, &Global::getInstance().walkTexture);
    //silhouette
    //this is similar to a sprite graphic
    //
    glBindTexture(GL_TEXTURE_2D, Global::getInstance().walkTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //
    //must build a new set of data...
    unsigned char *walkData = buildAlphaData(&img[0]);	
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, walkData);
    free(walkData);
    //-------------------------------------------------------------------------
    //create opengl texture elements
    w = img[1].width;
    h = img[1].height;
    glGenTextures(1, &Global::getInstance().exp.tex);
    //-------------------------------------------------------------------------
    //this is similar to a sprite graphic
    glBindTexture(GL_TEXTURE_2D, Global::getInstance().exp.tex);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //must build a new set of data...
    unsigned char *xData = buildAlphaData(&img[1]);	
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, xData);
    free(xData);
    //-------------------------------------------------------------------------
    w = img[2].width;
    h = img[2].height;
    //create opengl texture elements
    glGenTextures(1, &Global::getInstance().exp44.tex);
    //-------------------------------------------------------------------------
    //this is similar to a sprite graphic
    glBindTexture(GL_TEXTURE_2D, Global::getInstance().exp44.tex);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //must build a new set of data...
    xData = buildAlphaData(&img[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, xData);
    free(xData);

    // Created picture image array
    for(int i = 3; i < 8; i++){
        glGenTextures(1, &Global::getInstance().creditPicsTexture[i - 3]);
        w = img[i].width;
        h = img[i].height;
        glBindTexture(GL_TEXTURE_2D, 
                Global::getInstance().creditPicsTexture[i - 3]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        xData = buildAlphaData(&img[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, xData);
        free(xData);
    }

    // Helicopter texture
    glGenTextures(1, &Global::getInstance().helicopterTexture);
    //-------------------------------------------------------------------------
    //helicopter

    int w_helicopter = helicopter_image.width;
    int h_helicopter = helicopter_image.height;
    //
    glBindTexture(GL_TEXTURE_2D, Global::getInstance().helicopterTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w_helicopter, h_helicopter, 0,
            GL_RGB, GL_UNSIGNED_BYTE, helicopter_image.data);
    //-------------------------------------------------------------------------
    unsigned char *heliData = buildAlphaData(&helicopter_image);	
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w_helicopter, h_helicopter, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, heliData);
    free(heliData);
    // bomb texture
    glGenTextures(1, &Global::getInstance().bombTexture);
    //-------------------------------------------------------------------------
    //bomb

    int w_bomb = bomb_image.width;
    int h_bomb = bomb_image.height;
    //
    glBindTexture(GL_TEXTURE_2D, Global::getInstance().bombTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w_bomb, h_bomb, 0,
            GL_RGB, GL_UNSIGNED_BYTE, bomb_image.data);
    //-------------------------------------------------------------------------
    unsigned char *bombData = buildAlphaData(&bomb_image);	
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w_bomb, h_bomb, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, bombData);
    free(bombData);

    // Background initialization 
    glGenTextures(1, &Global::getInstance().backgroundTexture);
    int bw = backImage->width;
    int bh = backImage->height;
    glBindTexture(GL_TEXTURE_2D, Global::getInstance().backgroundTexture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, bw, bh, 0,
            GL_RGB, GL_UNSIGNED_BYTE, backImage->data);
    Global::getInstance().xc[0] = 0.0;
    Global::getInstance().xc[1] = 0.25;
    Global::getInstance().yc[0] = 0.0;
    Global::getInstance().yc[1] = 1.0;	



    // Fernando: Get a crate texture object for reasons.
    glGenTextures(1, &Global::getInstance().crateTexture);
    //-Crate texture----------------------------------------------------------

    int crate_h = crate_image.height;
    int crate_w = crate_image.width;
    // Fernando: We call this object again because...
    glBindTexture(GL_TEXTURE_2D, Global::getInstance().crateTexture);
    // Fernando: We call this block because...
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, crate_w, crate_h, 0, GL_RGB,
            GL_UNSIGNED_BYTE, crate_image.data);

    unsigned char *crateData = buildAlphaData(&crate_image);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, crate_w, crate_h, 0, GL_RGBA,
            GL_UNSIGNED_BYTE, crateData);
    free(crateData);

    //-Crate texture-END------------------------------------------------------
    // Start Menu texture and binding
    glGenTextures(1, &Global::getInstance().startMenuTexture);	
    int xres = start_menu_image.width;
    int yres = start_menu_image.height;
    glBindTexture(GL_TEXTURE_2D, Global::getInstance().startMenuTexture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    unsigned char *startScreenData = buildAlphaData(&start_menu_image);	
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, xres, yres, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, startScreenData);
    free(startScreenData);

    // Logo texture and binding
    glGenTextures(1, &Global::getInstance().logoTexture);	
    int lxres = logo_image.width;
    int lyres = logo_image.height;
    glBindTexture(GL_TEXTURE_2D, Global::getInstance().logoTexture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    unsigned char *logoData = buildAlphaData(&logo_image);	
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lxres, lyres, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, logoData);
    free(logoData);

    //leaderboard
    glGenTextures(1, &Global::getInstance().leaderboardTexture);	
    int lexres = leaderboard_image.width;
    int leyres = leaderboard_image.height;
    glBindTexture(GL_TEXTURE_2D, Global::getInstance().leaderboardTexture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    unsigned char *leaderboardData = buildAlphaData(&leaderboard_image);	
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lexres, leyres, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, leaderboardData);
    free(leaderboardData);

    //leaderboard title
    glGenTextures(1, &Global::getInstance().leaderboardTitleTexture);	
    int ltxres = leaderboard_title_image.width;
    int ltyres = leaderboard_title_image.height;
    glBindTexture(GL_TEXTURE_2D, Global::getInstance().leaderboardTitleTexture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    unsigned char *leaderboardTitleData = buildAlphaData(&leaderboard_title_image);	
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ltxres, ltyres, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, leaderboardTitleData);
    free(leaderboardTitleData);

    //number texture
    for (int i = 0; i < NUMBERS_ARRAY; i++) {
        glGenTextures(1, &Global::getInstance().numbersTexture[i]);	
        int numxres = numbers_image[i].width;
        int numyres = numbers_image[i].height;
        glBindTexture(GL_TEXTURE_2D, Global::getInstance().numbersTexture[i]);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        unsigned char *numData = buildAlphaData(&numbers_image[i]);	
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, numxres, numyres, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, numData);
        free(numData);
    }

    //letters texture
    for (int i = 0; i < LETTERS_ARRAY; i++) {
        glGenTextures(1, &Global::getInstance().lettersTexture[i]);	
        int letxres = letters_image[i].width;
        int letyres = letters_image[i].height;
        glBindTexture(GL_TEXTURE_2D, Global::getInstance().lettersTexture[i]);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        unsigned char *letData = buildAlphaData(&letters_image[i]);	
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, letxres, letyres, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, letData);
        free(letData);
    }

    //game time
    glGenTextures(1, &Global::getInstance().timeTexture);	
    int txres = time_image.width;
    int tyres = time_image.height;
    glBindTexture(GL_TEXTURE_2D, Global::getInstance().timeTexture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    unsigned char *timeData = buildAlphaData(&time_image);	
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, txres, tyres, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, timeData);
    free(timeData);
}

void init()
{

}

void checkMouse(XEvent *e)
{
    //printf("checkMouse()...\n"); fflush(stdout);
    //Did the mouse move?
    //Was a mouse button clicked?
    static int savex = 0;
    static int savey = 0;
    //
    if (e->type != ButtonRelease && e->type != ButtonPress &&
            e->type != MotionNotify)
        return;
    if (e->type == ButtonRelease) {
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
    if (e->type == MotionNotify) {
        if (savex != e->xbutton.x || savey != e->xbutton.y) {
            //Mouse moved
            savex = e->xbutton.x;
            savey = e->xbutton.y;
        }
    }
}

void screenCapture()
{
    static int fnum = 0;
    static int vid = 0;
    if (!vid) {
        DIR* viddir = opendir("vid");
        if (viddir) {
            closedir(viddir);
        } else {
            system("mkdir ./vid");
            vid = 1;
        }
    }
    unsigned char *data = (unsigned char *)malloc(Global::getInstance().xres 
            * Global::getInstance().yres * 3);
    glReadPixels(0, 0, 
            Global::getInstance().xres, 
            Global::getInstance().yres, 
            GL_RGB, GL_UNSIGNED_BYTE, data);
    char ts[32];
    sprintf(ts, "./vid/pic%03i.ppm", fnum);
    FILE *fpo = fopen(ts,"w");	
    if (fpo) {
        fprintf(fpo, "P6\n%i %i\n255\n", 
                Global::getInstance().xres, 
                Global::getInstance().yres);
        unsigned char *p = data;
        //go backwards a row at a time...
        p = p + ((Global::getInstance().yres-1) 
                * Global::getInstance().xres * 3);
        unsigned char *start = p;
        for (int i=0; i<Global::getInstance().yres; i++) {
            for (int j=0; j<Global::getInstance().xres*3; j++) {
                fprintf(fpo, "%c",*p);
                ++p;
            }
            start = start - (Global::getInstance().xres*3);
            p = start;
        }
        fclose(fpo);
        char s[256];
        sprintf(s, "convert ./vid/pic%03i.ppm ./vid/pic%03i.gif", fnum, fnum);
        system(s);
        unlink(ts);
    }
    ++fnum;
}


void moveHelicopter()
{
    if ((helicopter.pos[0] < -140.0 && helicopter.vel[0] < 0.0) ||
            (helicopter.pos[0] >= (float)Global::getInstance().xres+140.0 &&
             helicopter.vel[0] > 0.0))
    {
        helicopter.vel[0] = -helicopter.vel[0];
    }

    helicopter.pos[0] += helicopter.vel[0];
}

void moveBomb()
{
    double seconds_since_start = difftime( time(0), start);
    if ((bomb.pos[0] < -140.0 && bomb.vel[0] < 0.0) ||
            (bomb.pos[0] >= (float)Global::getInstance().xres+140.0 &&
             bomb.vel[0] > 0.0))
    {
        bomb.vel[0] = -bomb.vel[0];
    }

    bomb.pos[0] += bomb.vel[0];
    if(fmod(seconds_since_start, 3.0) == 1.0) {
        Global::getInstance().dropBomb = 1;
    }
    if(Global::getInstance().dropBomb) {
        bomb.pos[1] -= bomb.vel[1];
    }

    if(bomb.pos[1] < 325) {
        // printf("%f\n", bomb.pos[1]);
        bomb.pos[1] = 575;
        Global::getInstance().dropBomb = 0;
    }

    if(( // Check the x position
                abs(bomb.pos[0]-((float)Global::getInstance().xres)/2) < 50)
            &&
            //CHeck the y position
            abs(bomb.pos[1]-((float)Global::getInstance().yres/2) < 150)
      )
    {
        // printf("bomb in zone!\n");
        Global::getInstance().health--;
        printf("Ouch! Health is now: %d\n", Global::getInstance().health);
        Global::getInstance().exp44.pos[0] = 0;
        Global::getInstance().exp44.pos[1] = 0;
        Global::getInstance().exp44.pos[2] =   0.0;
        Global::getInstance().exp44.onoff ^= 1;
    }

    // printf("walk.gif pos: %f, %f\n", (float)Global::getInstance().xres/2, (float)Global::getInstance().yres/2);
}
void moveWalker(int direction)
{
    if ((walker.pos[0] < -140.0 && walker.vel[0] < 0.0) ||
            (walker.pos[0] >= (float)Global::getInstance().xres+140.0 &&
             walker.vel[0] > 0.0))
    {
        walker.vel[0] = -walker.vel[0];
    }
    //right movement 
    if (direction == 1){  
        walker.pos[0] += walker.vel[0];
    }
    //left movement
    if (direction == 2){ 
        walker.pos[0] -= walker.vel[0];
    }
}

int checkKeys(XEvent *e)
{
    //keyboard input?
    static int shift=0;
    if (e->type != KeyPress && e->type != KeyRelease)
        return 0;
    int key = XLookupKeysym(&e->xkey, 0);
    Global::getInstance().keys[key]=1;
    if (e->type == KeyRelease) {
        Global::getInstance().keys[key]=0;
        if (key == XK_Shift_L || key == XK_Shift_R)
            shift=0;
        return 0;
    }
    Global::getInstance().keys[key]=1;
    if (key == XK_Shift_L || key == XK_Shift_R) {
        shift=1;
        return 0;
    }
    (void)shift;
    switch (key) {
        case XK_h:
            //moveHelicopter();
            printf("h pressed!\n");
            printf("helicopter.pos[0]: %f\nhelicopter.pos[1]: %f\nhelicopter.vel[0]: %f\n", helicopter.pos[0], helicopter.pos[1], helicopter.vel[0]);
            // moveHelicopter();
            // printf("helicopter.pos[0]: %f\nhelicopter.pos[1]: %f\nhelicopter.vel[0]: %f\n", helicopter.pos[0], helicopter.pos[1], helicopter.vel[0]);
            break;
        case XK_s:
            //screenCapture();
            break;
        case XK_m:
            Global::getInstance().movie ^= 1;
            break;
        case XK_w:
            timers.recordTime(&timers.walkTime);
            Global::getInstance().walk ^= 1;
            break;
        case XK_e:
            Global::getInstance().exp.pos[0] = 200.0;
            Global::getInstance().exp.pos[1] = -60.0;
            Global::getInstance().exp.pos[2] =   0.0;
            timers.recordTime(&Global::getInstance().exp.time);
            Global::getInstance().exp.onoff ^= 1;
            break;
        case XK_f:
            Global::getInstance().exp44.pos[0] = 200.0;
            Global::getInstance().exp44.pos[1] = -60.0;
            Global::getInstance().exp44.pos[2] =   0.0;
            timers.recordTime(&Global::getInstance().exp44.time);
            Global::getInstance().exp44.onoff ^= 1;
            break;
        case XK_Left:
            walker.dir = 1;
            moveWalker(walker.dir);
            Global::getInstance().Forward = false;
            Global::getInstance().Backward = true;
            break;
        case XK_Right:
            walker.dir = 2;
            moveWalker(walker.dir);
            Global::getInstance().Backward = false;
            Global::getInstance().Forward = true;
            break;
        case XK_c:
            // If the credits are currently being shown, we are about to hide them/
            // We load the helicopter with the last known position so it will resume
            // where it left off.
            // if(Global::getInstance().showCredits == 1) {
            // 	helicopter.pos[0] = lastKnownHelicopterPos();
            // }
            if (!Global::getInstance().showLeaderboard &&
                    Global::getInstance().showStartMenu) {
                setHelicopterPos(helicopter.pos[0]);
                //Global::getInstance().showStartMenu ^=1;
                Global::getInstance().showCredits ^= 1;
                //Global::getInstance().displayHelicopter ^= 1;
            }
            break;
        case XK_Up:
            break;
        case XK_Down:
            break;
        case XK_equal:
            Global::getInstance().delay -= 0.005;
            if (Global::getInstance().delay < 0.005)
                Global::getInstance().delay = 0.005;
            break;
        case XK_minus:
            Global::getInstance().delay += 0.005;
            break;
        case XK_p:
            if (!Global::getInstance().showCredits &&
                    !Global::getInstance().showLeaderboard) {
                Global::getInstance().showStartMenu ^= 1;
            }
            if (!Global::getInstance().showStartMenu &&
                    !Global::getInstance().showCredits &&
                    !Global::getInstance().showLeaderboard) {
                start_time();
            }
            break;
        case XK_l:
            if (!Global::getInstance().showCredits &&
                    Global::getInstance().showStartMenu) {
                Global::getInstance().showLeaderboard ^= 1;
            }
            break;
        case XK_Escape:
            return 1;
            break;
    }
    return 0;
}

Flt VecNormalize(Vec vec)
{
    Flt len, tlen;
    Flt xlen = vec[0];
    Flt ylen = vec[1];
    Flt zlen = vec[2];
    len = xlen*xlen + ylen*ylen + zlen*zlen;
    if (len == 0.0) {
        MakeVector(vec, 0.0, 0.0, 1.0);
        return 1.0;
    }
    len = sqrt(len);
    tlen = 1.0 / len;
    vec[0] = xlen * tlen;
    vec[1] = ylen * tlen;
    vec[2] = zlen * tlen;
    return(len);
}

void physics(void)
{
    if(Global::getInstance().Forward == True && Global::getInstance().Backward == False) { 
        Global::getInstance().xc[0] += 0.002;
        Global::getInstance().xc[1] += 0.002;
        Global::getInstance().Forward = False;
    }
    if(Global::getInstance().Forward == False && Global::getInstance().Backward == True) { 
        Global::getInstance().xc[0] -= 0.002;
        Global::getInstance().xc[1] -= 0.002;
        Global::getInstance().Backward = False;
    }
    if (Global::getInstance().walk 
            || Global::getInstance().keys[XK_Right] 
            || Global::getInstance().keys[XK_Left]) {
        //man is walking...
        //when time is up, advance the frame.
        if ( Global::getInstance().keys[XK_Right]) {
            Global::getInstance().plat1.slidePlatformBackward();
        }
        if ( Global::getInstance().keys[XK_Left]) {
            Global::getInstance().plat1.slidePlatformForward();
        }
        timers.recordTime(&timers.timeCurrent);
        double timeSpan = timers.timeDiff(&timers.walkTime, 
                &timers.timeCurrent);
        if (timeSpan > Global::getInstance().delay) {
            //advance
            ++Global::getInstance().walkFrame;
            if (Global::getInstance().walkFrame >= 16)
                Global::getInstance().walkFrame -= 16;
            timers.recordTime(&timers.walkTime);
        }
        for (int i=0; i<20; i++) {
            if (Global::getInstance().keys[XK_Left]) {
                Global::getInstance().box[i][0] += 1.0 
                    * (0.05 / Global::getInstance().delay);
                if (Global::getInstance().box[i][0] > 
                        Global::getInstance().xres + 10.0)
                    Global::getInstance().box[i][0] -= 
                        Global::getInstance().xres + 10.0;
                Global::getInstance().camera[0] -= 2.0/lev.tilesize[0] 
                    * (0.05 / Global::getInstance().delay);
                if (Global::getInstance().camera[0] < 0.0)
                    Global::getInstance().camera[0] = 0.0;
            } else {
                Global::getInstance().box[i][0] -= 1.0 
                    * (0.05 / Global::getInstance().delay);
                if (Global::getInstance().box[i][0] < -10.0)
                    Global::getInstance().box[i][0] += 
                        Global::getInstance().xres + 10.0;
                Global::getInstance().camera[0] += 2.0/lev.tilesize[0] 
                    * (0.05 / Global::getInstance().delay);
                if (Global::getInstance().camera[0] < 0.0)
                    Global::getInstance().camera[0] = 0.0;
            }
        }
        if (Global::getInstance().exp.onoff) {
            Global::getInstance().exp.pos[0] -= 2.0 
                * (0.05 / Global::getInstance().delay);
        }
        if (Global::getInstance().exp44.onoff) {
            Global::getInstance().exp44.pos[0] -= 2.0 
                * (0.05 / Global::getInstance().delay);
        }
    }
    if (Global::getInstance().exp.onoff) {
        //explosion is happening
        timers.recordTime(&timers.timeCurrent);
        double timeSpan = timers.timeDiff(&Global::getInstance().exp.time, 
                &timers.timeCurrent);
        if (timeSpan > Global::getInstance().exp.delay) {
            //advance explosion frame
            ++Global::getInstance().exp.frame;
            if (Global::getInstance().exp.frame >= 23) {
                //explosion is done.
                Global::getInstance().exp.onoff = 0;
                Global::getInstance().exp.frame = 0;
            } else {
                timers.recordTime(&Global::getInstance().exp.time);
            }
        }
    }
    if (Global::getInstance().exp44.onoff) {
        //explosion is happening
        timers.recordTime(&timers.timeCurrent);
        double timeSpan = timers.timeDiff(&Global::getInstance().exp44.time, 
                &timers.timeCurrent);
        if (timeSpan > Global::getInstance().exp44.delay) {
            //advance explosion frame
            ++Global::getInstance().exp44.frame;
            if (Global::getInstance().exp44.frame >= 16) {
                //explosion is done.
                Global::getInstance().exp44.onoff = 0;
                Global::getInstance().exp44.frame = 0;
            } else {
                timers.recordTime(&Global::getInstance().exp44.time);
            }
        }
    }
    //====================================
    //Adjust position of ball.
    //Height of highest tile when ball is?
    //====================================
    /*
       Flt dd = lev.ftsz[0];
       int col = (int)((Global::getInstance().camera[0]
       +Global::getInstance().ball_pos[0]) / dd);
       col = col % lev.ncols;
       int hgt = 0;
       for (int i=0; i<lev.nrows; i++) {
       if (lev.arr[i][col] != ' ') {
       hgt = (lev.nrows-i) * lev.tilesize[1];
       break;
       }
       }
       if (Global::getInstance().ball_pos[1] < (Flt)hgt) {
       Global::getInstance().ball_pos[1] = (Flt)hgt;
       MakeVector(Global::getInstance().ball_vel, 0, 0, 0);
       } else {
       Global::getInstance().ball_vel[1] -= 0.9;
       }
       Global::getInstance().ball_pos[1] += Global::getInstance().ball_vel[1];
       */

    if (Global::getInstance().keys[XK_space]) {
        //a little time between each bullet
        struct timespec bt;
        clock_gettime(CLOCK_REALTIME, &bt);
        double ts = timers.timeDiff(&bullets.bulletTimer, &bt);
        if (ts > 0.1) {
            timers.timeCopy(&bullets.bulletTimer, &bt);

#ifdef PROFILE_VICTOR
            cout << "getting ready to shoot..." << endl;
#endif

            if (Global::getInstance().keys[XK_Up] 
                    && Global::getInstance().keys[XK_Right])
                shootBullet(&bullets, &bt, FrontDiag);
            else if (Global::getInstance().keys[XK_Up] 
                    && Global::getInstance().keys[XK_Left])
                shootBullet(&bullets, &bt, BackDiag);
            else if (Global::getInstance().keys[XK_Up])
                shootBullet(&bullets, &bt, Up);
            else if (Global::getInstance().keys[XK_Down])
                shootBullet(&bullets, &bt, Down);
            else if (Global::getInstance().keys[XK_Left])
                shootBullet(&bullets, &bt, Back);
            else 
                shootBullet(&bullets, &bt, Front);

#ifdef PROFILE_VICTOR
            cout << "shoot completed..." << endl;
#endif
        }
    }
    // Update bullet position
    updateBulletPosition(&bullets, 
            Global::getInstance().xres, 
            Global::getInstance().yres);

    // Check for collosion
    checkBulletHelicopterCollision(&bullets, 
            helicopter.pos[0], 
            helicopter.pos[1]);

    // Animate the helicopter, but only if the start menu isn't showing
    if (Global::getInstance().showStartMenu != 1) {
        moveHelicopter();
        moveBomb();
    }
    // Helicopter
    // helicopter.pos[0] += 10.0;
    // if (helicopter.pos[0] > xres) {
    // 	helicopter.pos[0] % xres;
    // }
}

void showHelicopter(int x, int y, float velocity)
{
    extern void renderHelicopter(int x, 
            int y, 
            GLuint helicopterID, 
            float velocity);
    renderHelicopter(x, y, 
            Global::getInstance().helicopterTexture, velocity);
}

void setHelicopterPos(float pos)
{
    extern void setLastKnownHelicopterPos(float pos);
    setLastKnownHelicopterPos(pos);
}

float lastKnownHelicopterPos()
{
    extern float getLastKnownHelicopterPos();
    return getLastKnownHelicopterPos();
}

void showBomb(int x, int y, float velocity)
{
    extern void renderBomb(int x, 
            int y, 
            GLuint bombID, 
            float velocity);
    renderBomb(x, y, 
            Global::getInstance().bombTexture, velocity);
}

void show_credits(Rect x, int y)
{
    glClear(GL_COLOR_BUFFER_BIT);
    extern void masonP(Rect x, int y);
    extern void showMasonPicture(int x, int y, GLuint textid);
    // Mason
    x.bot -= 100;
    masonP(x, y);
    showMasonPicture(500, x.bot, 
            Global::getInstance().creditPicsTexture[MASON]);
    // Fern
    extern void showFHText(Rect x);
    extern void showFernandoPicture(int x, int y, GLuint textid);
    x.bot -= 100; 
    showFHText(x);
    showFernandoPicture(500, x.bot, 
            Global::getInstance().creditPicsTexture[FERNANDO]);
    // Hasun
    extern void printHasunName(Rect x, int y);
    extern void showHasunPicture(int x, int y, GLuint textid);
    x.bot -= 100;
    printHasunName(x, y);
    showHasunPicture(500, x.bot, 
            Global::getInstance().creditPicsTexture[HASUN]);
    // Victor
    x.bot -= 100;
    showVictorText(x);
    showVictorPicture(500, x.bot, 
            Global::getInstance().creditPicsTexture[VICTOR]);
    // Emil
    extern void showEmil(Rect r, int y);
    extern void showEmilPicture(int x, int y, GLuint textid);
    x.bot -= 100;
    showEmil(x, y);
    showEmilPicture(500, x.bot, 
            Global::getInstance().creditPicsTexture[EMIL]);
}

void render(void)
{
    Rect r;
    //Clear the screen
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    float cx = Global::getInstance().xres/2.0;
    float cy = Global::getInstance().yres/2.0;
    if (Global::getInstance().showStartMenu) {
        start_menu(Global::getInstance().xres, 
                Global::getInstance().yres, 
                Global::getInstance().startMenuTexture);
        show_logo(Global::getInstance().xres, 
                Global::getInstance().yres, 
                Global::getInstance().logoTexture);
        show_keys(Global::getInstance().xres, 
                Global::getInstance().yres, 
                Global::getInstance().lettersTexture);
        if (Global::getInstance().showLeaderboard) {
            leaderboard(Global::getInstance().xres, 
                    Global::getInstance().yres, 
                    Global::getInstance().leaderboardTexture);
            leaderboard_title(Global::getInstance().xres, 
                    Global::getInstance().yres, 
                    Global::getInstance().leaderboardTitleTexture);
            generate_leaderboard();
            print_leaderboard(Global::getInstance().xres, 
                    Global::getInstance().yres,
                    Global::getInstance().numbersTexture,
                    Global::getInstance().lettersTexture);
        }
        if (Global::getInstance().showCredits) {
            r.bot = Global::getInstance().yres - 20;
            r.left = 10;
            r.center = 0;
            show_credits(r, cy);
        }
    } else if (Global::getInstance().health <= 0) {
        Global::getInstance().done = 1;
    } else {
            glClearColor(0.1,0.1,0.1,1.0);
            glClear(GL_COLOR_BUFFER_BIT);
            //background rendering 
            int bxres = Global::getInstance().xres;
            int byres = Global::getInstance().yres;
            glClear(GL_COLOR_BUFFER_BIT);
            glColor3f(1.0, 1.0, 1.0);
            glBindTexture(GL_TEXTURE_2D, Global::getInstance().backgroundTexture);
            glBegin(GL_QUADS);
            glTexCoord2f(Global::getInstance().xc[0], Global::getInstance().yc[1]); glVertex2i(0, 0);
            glTexCoord2f(Global::getInstance().xc[0], Global::getInstance().yc[0]); glVertex2i(0, byres);
            glTexCoord2f(Global::getInstance().xc[1], Global::getInstance().yc[0]); glVertex2i(bxres, byres);
            glTexCoord2f(Global::getInstance().xc[1], Global::getInstance().yc[1]); glVertex2i(bxres, 0);
            glEnd();

            /*
            //show ground
            glBegin(GL_QUADS);
            glColor3f(0.2, 0.2, 0.2);
            glVertex2i(0, 220);
            glVertex2i(Global::getInstance().xres, 220);
            glColor3f(0.4, 0.4, 0.4);
            glVertex2i(Global::getInstance().xres,   0);
            glVertex2i(0, 0);
            glEnd();

            //
            //show boxes as background
            for (int i=0; i<20; i++) {
            glPushMatrix();
            glTranslated(Global::getInstance().box[i][0],Global::getInstance().box[i][1],Global::getInstance().box[i][2]);
            glColor3f(0.2, 0.2, 0.2);
            glBegin(GL_QUADS);
            glVertex2i( 0,  0);
            glVertex2i( 0, 30);
            glVertex2i(20, 30);
            glVertex2i(20,  0);
            glEnd();
            glPopMatrix();
            }
            */
            //
            //========================
            //Render the tile system
            //========================
            /*
               int tx = lev.tilesize[0];
               int ty = lev.tilesize[1];
               Flt dd = lev.ftsz[0];
               Flt offy = lev.tile_base;
               int ncols_to_render = Global::getInstance().xres / lev.tilesize[0] + 2;
               int col = (int)(Global::getInstance().camera[0] / dd);
               col = col % lev.ncols;
            //Partial tile offset must be determined here.
            //The leftmost tile might be partially off-screen.
            //cdd: camera position in terms of tiles.
            Flt cdd = Global::getInstance().camera[0] / dd;
            //flo: just the integer portion
            Flt flo = floor(cdd);
            //dec: just the decimal portion
            Flt dec = (cdd - flo);
            //offx: the offset to the left of the screen to start drawing tiles
            Flt offx = -dec * dd;
            //Log("Global::getInstance().camera[0]: %lf   offx: %lf\n",Global::getInstance().camera[0],offx);
            for (int j=0; j<ncols_to_render; j++) {
            int row = lev.nrows-1;
            for (int i=0; i<lev.nrows; i++) {
            if (lev.arr[row][col] == 'w') {
            glColor3f(0.8, 0.8, 0.6);
            glPushMatrix();
            //put tile in its place
            glTranslated((Flt)j*dd+offx, (Flt)i*lev.ftsz[1]+offy, 0);
            glBegin(GL_QUADS);
            glVertex2i(0, 0);
            glVertex2i(0, ty);
            glVertex2i(tx, ty);
            glVertex2i(tx, 0);
            glEnd();
            glPopMatrix();
            }
            if (lev.arr[row][col] == 'b') {
            glColor3f(0.9, 0.2, 0.2);
            glPushMatrix();
            glTranslated((Flt)j*dd+offx, (Flt)i*lev.ftsz[1]+offy, 0);
            glBegin(GL_QUADS);
            glVertex2i(0, 0);
            glVertex2i(0, ty);
            glVertex2i(tx, ty);
            glVertex2i(tx, 0);
            glEnd();
            glPopMatrix();
            }
            --row;
            }
            col = (col+1) % lev.ncols;
            }
            glColor3f(1.0, 1.0, 0.1);
            glPushMatrix();
            //put ball in its place
            glTranslated(Global::getInstance().ball_pos[0], lev.tile_base+Global::getInstance().ball_pos[1], 0);
            glBegin(GL_QUADS);
            glVertex2i(-10, 0);
            glVertex2i(-10, 20);
            glVertex2i( 10, 20);
            glVertex2i( 10, 0);
            glEnd();
            glPopMatrix();
            */
            //--------------------------------------END TILE SYSTEM
            //



            //#define SHOW_FAKE_SHADOW
#ifdef SHOW_FAKE_SHADOW
            glColor3f(0.25, 0.25, 0.25);
            glBegin(GL_QUADS);
            glVertex2i(cx-60, 150);
            glVertex2i(cx+50, 150);
            glVertex2i(cx+50, 130);
            glVertex2i(cx-60, 130);
            glEnd();
#endif
            //
            // Commenting this out will make the man into a white box.
            float h = 50.0;
            float w = h * 0.5;
            glPushMatrix();
            glColor3f(1.0, 1.0, 1.0);
            glBindTexture(GL_TEXTURE_2D, Global::getInstance().walkTexture);
            //
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0.0f);
            glColor4ub(255,255,255,255);
            int ix = Global::getInstance().walkFrame % 8;
            int iy = 0;
            if (Global::getInstance().walkFrame >= 8)
                iy = 1;
            float fx = (float)ix / 8.0;
            float fy = (float)iy / 2.0;
            glBegin(GL_QUADS);
            if (Global::getInstance().keys[XK_Left]) {
                glTexCoord2f(fx+.125, fy+.5);
                glVertex2i(cx-w, cy-h);
                glTexCoord2f(fx+.125, fy);
                glVertex2i(cx-w, cy+h);
                glTexCoord2f(fx, fy);
                glVertex2i(cx+w, cy+h);
                glTexCoord2f(fx, fy+.5);
                glVertex2i(cx+w, cy-h);
            } else {
                glTexCoord2f(fx, fy+.5);
                glVertex2i(cx-w, cy-h);
                glTexCoord2f(fx, fy);
                glVertex2i(cx-w, cy+h);
                glTexCoord2f(fx+.125, fy);
                glVertex2i(cx+w, cy+h);
                glTexCoord2f(fx+.125, fy+.5);
                glVertex2i(cx+w, cy-h);
            }
            glEnd();
            glPopMatrix();
            /*float wid = 120;
              glPushMatrix();
              glBindTexture(GL_TEXTURE_2D, Global::getInstance().helicopterTexture);
              glBegin(GL_QUADS);
              if (helicopter.vel[0] > 0.0) {
              glTexCoord2f(1.0f, 1.0f);
              glVertex2i(-wid,-wid);
              glTexCoord2f(1.0f, 0.0f);
              glVertex2i(-wid, wid);
              glTexCoord2f(0.0f, 0.0f);
              glVertex2i( wid, wid);
              glTexCoord2f(0.0f, 1.0f);
              glVertex2i( wid,-wid);
              } else {
              glTexCoord2f(0.0f, 1.0f);
              glVertex2i(-wid,-wid);
              glTexCoord2f(0.0f, 0.0f);
              glVertex2i(-wid, wid);
              glTexCoord2f(1.0f, 0.0f);
              glVertex2i( wid, wid);
              glTexCoord2f(1.0f, 1.0f);
              glVertex2i( wid,-wid);
              }
              glEnd();
              glPopMatrix();
              */
            glBindTexture(GL_TEXTURE_2D, 0);
            glDisable(GL_ALPHA_TEST);
            //
            //
            if (Global::getInstance().exp.onoff) {
                h = 80.0;
                w = 80.0;
                glPushMatrix();
                glColor3f(1.0, 1.0, 1.0);
                glBindTexture(GL_TEXTURE_2D, Global::getInstance().exp.tex);
                glEnable(GL_ALPHA_TEST);
                glAlphaFunc(GL_GREATER, 0.0f);
                glColor4ub(255,255,255,255);
                glTranslated(Global::getInstance().exp.pos[0], Global::getInstance().exp.pos[1], Global::getInstance().exp.pos[2]);
                int ix = Global::getInstance().exp.frame % 5;
                int iy = Global::getInstance().exp.frame / 5;
                float tx = (float)ix / 5.0;
                float ty = (float)iy / 5.0;
                glBegin(GL_QUADS);
                if (Global::getInstance().keys[XK_Left]) {
                    //printf("I'm walking to the left");
                    glTexCoord2f(fx+.125, fy+.5);
                    glVertex2i(cx-w, cy-h);
                    glTexCoord2f(fx+.125, fy);
                    glVertex2i(cx-w, cy+h);
                    glTexCoord2f(fx, fy);
                    glVertex2i(cx+w, cy+h);
                    glTexCoord2f(fx, fy+.5);
                    glVertex2i(cx+w, cy-h);
                } else {
                    glTexCoord2f(fx, fy+.5);
                    glVertex2i(cx-w, cy-h);
                    glTexCoord2f(fx, fy);
                    glVertex2i(cx-w, cy+h);
                    glTexCoord2f(fx+.125, fy);
                    glVertex2i(cx+w, cy+h);
                    glTexCoord2f(fx+.125, fy+.5);
                    glVertex2i(cx+w, cy-h);
                }
                glEnd();
                glPopMatrix();
                glBindTexture(GL_TEXTURE_2D, 0);
                glDisable(GL_ALPHA_TEST);
            }
            //
            //
            if (Global::getInstance().exp44.onoff) {
                h = 80.0;
                w = 80.0;
                glPushMatrix();
                glColor3f(1.0, 1.0, 1.0);
                glBindTexture(GL_TEXTURE_2D, Global::getInstance().exp44.tex);
                glEnable(GL_ALPHA_TEST);
                glAlphaFunc(GL_GREATER, 0.0f);
                glColor4ub(255,255,255,255);
                glTranslated(Global::getInstance().exp44.pos[0], Global::getInstance().exp44.pos[1], Global::getInstance().exp44.pos[2]);
                int ix = Global::getInstance().exp44.frame % 4;
                int iy = Global::getInstance().exp44.frame / 4;
                float tx = (float)ix / 4.0;
                float ty = (float)iy / 4.0;
                glBegin(GL_QUADS);

                glTexCoord2f(tx, ty+0.25);
                glVertex2i(cx-w, cy-h);
                glTexCoord2f(tx, ty);
                glVertex2i(cx-w, cy+h);
                glTexCoord2f(tx+0.25, ty);
                glVertex2i(cx+w, cy+h);
                glTexCoord2f(tx+0.25, ty+0.25);
                glVertex2i(cx+w, cy-h);

                glEnd();
                glPopMatrix();
                glBindTexture(GL_TEXTURE_2D, 0);
                glDisable(GL_ALPHA_TEST);
                //
                //
                if (Global::getInstance().exp.onoff) {
                    h = 80.0;
                    w = 80.0;
                    glPushMatrix();
                    glColor3f(1.0, 1.0, 1.0);
                    glBindTexture(GL_TEXTURE_2D, Global::getInstance().exp.tex);
                    glEnable(GL_ALPHA_TEST);
                    glAlphaFunc(GL_GREATER, 0.0f);
                    glColor4ub(255,255,255,255);
                    glTranslated(Global::getInstance().exp.pos[0], Global::getInstance().exp.pos[1], Global::getInstance().exp.pos[2]);
                    int ix = Global::getInstance().exp.frame % 5;
                    int iy = Global::getInstance().exp.frame / 5;
                    float tx = (float)ix / 5.0;
                    float ty = (float)iy / 5.0;
                    glBegin(GL_QUADS);
                    glTexCoord2f(tx, ty+0.2);
                    glVertex2i(cx-w, cy-h);
                    glTexCoord2f(tx, ty);
                    glVertex2i(cx-w, cy+h);
                    glTexCoord2f(tx+0.2, ty);
                    glVertex2i(cx+w, cy+h);
                    glTexCoord2f(tx+0.2, ty+0.2);
                    glVertex2i(cx+w, cy-h);
                    glEnd();
                    glPopMatrix();
                    glBindTexture(GL_TEXTURE_2D, 0);
                    glDisable(GL_ALPHA_TEST);
                }
                //
                //
                if (Global::getInstance().exp44.onoff) {
                    h = 80.0;
                    w = 80.0;
                    glPushMatrix();
                    glColor3f(1.0, 1.0, 1.0);
                    glBindTexture(GL_TEXTURE_2D, Global::getInstance().exp44.tex);
                    glEnable(GL_ALPHA_TEST);
                    glAlphaFunc(GL_GREATER, 0.0f);
                    glColor4ub(255,255,255,255);
                    glTranslated(Global::getInstance().exp44.pos[0], Global::getInstance().exp44.pos[1], Global::getInstance().exp44.pos[2]);
                    int ix = Global::getInstance().exp44.frame % 4;
                    int iy = Global::getInstance().exp44.frame / 4;
                    float tx = (float)ix / 4.0;
                    float ty = (float)iy / 4.0;
                    glBegin(GL_QUADS);

                    glTexCoord2f(tx, ty+0.25);
                    glVertex2i(cx-w, cy-h);
                    glTexCoord2f(tx, ty);
                    glVertex2i(cx-w, cy+h);
                    glTexCoord2f(tx+0.25, ty);
                    glVertex2i(cx+w, cy+h);
                    glTexCoord2f(tx+0.25, ty+0.25);
                    glVertex2i(cx+w, cy-h);

                    glEnd();
                    glPopMatrix();
                    glBindTexture(GL_TEXTURE_2D, 0);
                    glDisable(GL_ALPHA_TEST);
                }

                // //will add to menu
                // unsigned int c = 0x00ffff44;
                // r.bot = Global::getInstance().yres - 20;
                // r.left = 10;
                // r.center = 0;
                // ggprint8b(&r, 16, c, "W   Walk cycle");
                // ggprint8b(&r, 16, c, "E   Explosion");
                // ggprint8b(&r, 16, c, "+   faster");
                // ggprint8b(&r, 16, c, "-   slower");
                // ggprint8b(&r, 16, c, "right arrow -> walk right");
                // ggprint8b(&r, 16, c, "left arrow  <- walk left");
                // ggprint8b(&r, 16, c, "frame: %i", Global::getInstance().walkFrame);
                // ggprint8b(&r, 16, c, "credits   c");
                if (Global::getInstance().movie) {
                    screenCapture();
                }
                glPushMatrix();
                showHelicopter(helicopter.pos[0], helicopter.pos[1], helicopter.vel[0]);
                glPopMatrix();
                glPushMatrix();
                showBomb(bomb.pos[0], bomb.pos[1]-100, bomb.vel[0]);;
                glPopMatrix();
                //draw bullets
                drawBullets(&bullets);

                // Fernando: Adding a platform entity to the game.
                glPushMatrix();
                // This breaks the wall because I was translating it in the walk2
                // file and in drawPlatform()
                // ---> glTranslated(plat1.pos[0],plat1.pos[1],0);	
                Global::getInstance().plat1.drawPlatform( 
                        Global::getInstance().plat1.getXpos(),
                        Global::getInstance().plat1.getYpos(), 
                        Global::getInstance().crateTexture);
                glPopMatrix();
            }

            if (Global::getInstance().movie) {
                screenCapture();
            }
            glPushMatrix();
            showHelicopter(helicopter.pos[0], helicopter.pos[1], helicopter.vel[0]);
            glPopMatrix();
            glPushMatrix();
            showBomb(bomb.pos[0], bomb.pos[1]-100, bomb.vel[0]);;
            glPopMatrix();
            //draw bullets
            drawBullets(&bullets);

            // Fernando: Adding a platform entity to the game.
            glPushMatrix();
            // This breaks the wall because I was translating it in the walk2
            // file and in drawPlatform()
            //glTranslated(plat1.pos[0],plat1.pos[1],0);	
            Global::getInstance().plat1.drawPlatform(
                    Global::getInstance().plat1.getXpos(),
                    Global::getInstance().plat1.getYpos(), 
                    Global::getInstance().crateTexture);
            glPopMatrix();
        }
        // Render the helicopter
        // glPushMatrix();
        // showHelicopter(helicopter.pos[0], helicopter.pos[1], helicopter.vel[0]);
        // showBomb(bomb.pos[0], bomb.pos[1], bomb.vel[0]);
        // printf("bomb.pos[0] %f, bomb.pos[1]: %f, bomb.vel[0]: %f\n", bomb.pos[0], bomb.pos[1], bomb.vel[0]);
        // glPopMatrix();

        // If the credits are shown, we should hide the helicopter by moving it off screen
        if (Global::getInstance().displayHelicopter == 0) {
            helicopter.pos[0] = -200;
        }
        if (!Global::getInstance().showLeaderboard &&
                !Global::getInstance().showCredits &&
                !Global::getInstance().showStartMenu &&
                Global::getInstance().health > 0) {
            print_time(Global::getInstance().yres, 
                    Global::getInstance().numbersTexture);
            print_score(Global::getInstance().playerScore,
                    Global::getInstance().xres,
                    Global::getInstance().yres, 
                    Global::getInstance().numbersTexture);
            print_health(Global::getInstance().health,
                    Global::getInstance().xres,
                    Global::getInstance().yres, 
                    Global::getInstance().numbersTexture);
        }
    }
