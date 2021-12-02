/***************************************
AUTHOR: Harmanpreet Cheema
FILE: hcheema.cpp
DATE: September 30, 2021 
****************************************/

#include "fonts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <chrono>
#include <sstream>



int Score = 0;
bool Scored = false;
// Credit Function
void show_hcheema_credits(int x, int y)
{
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 1;
    Rect r2;
    r2.bot = y+40;
    r2.left = x;
    r2.center = 1;
    Rect r3;
    r3.bot = y+20;
    r3.left = x;
    r3.center = 1;
    std::ostringstream oss;
    oss << "Score: " << Score;
    std::string var = oss.str();
    ggprint8b(&r3, 16, 0x000000ff, "Credits!");
    ggprint8b(&r2, 16, 0x000000ff, var.c_str());
    ggprint8b(&r, 16, 0x00ff0000, "2. Harmanpreet Singh");
    // printf("Harman did a bit of programming.");
}

// Lab-10: Add feature to quit game on pressing Q
void quit_game()
{
    exit(0);
}

class Image
{
public:
    int width, height;
    unsigned char *data;
    ~Image() { delete[] data; }
    Image(const char *fname)
    {
        if (fname[0] == '\0')
            return;
        char name[40];
        strcpy(name, fname);
        int slen = strlen(name);
        name[slen - 4] = '\0';
        char ppmname[80];
        sprintf(ppmname, "%s.ppm", name);
        char ts[100];
        sprintf(ts, "convert %s %s", fname, ppmname);
        system(ts);
        FILE *fpi = fopen(ppmname, "r");
        if (fpi)
        {
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
            for (int i = 0; i < n; i++)
                data[i] = fgetc(fpi);
            fclose(fpi);
        }
        else
        {
            printf("ERROR opening image: %s\n", ppmname);
            exit(0);
        }
        unlink(ppmname);
    }
};
Image pipeim[1] = {"pipe.png"};

class Pipe
{
public:
    GLuint UpperPipeTexture;
    Image *UpperPipeSprite;
    GLuint LowerPipeTexture;
    Image *LowerPipeSprite;
    float xc[2] = {0, 1};
    float yc[2] = {0, 1};
    float posX = 2;
    float posY = random() % 5 * 0.3 - 0.3;
    float posZ = 0;
    Pipe()
    {
        glEnable(GL_BLEND);
        
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        UpperPipeSprite = &pipeim[0];
        glGenTextures(1, &UpperPipeTexture);
        int p1w = UpperPipeSprite->width;
        int p1h = UpperPipeSprite->height;
        glBindTexture(GL_TEXTURE_2D, UpperPipeTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, p1w, p1h, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, UpperPipeSprite->data);

        LowerPipeSprite = &pipeim[0];
        glGenTextures(1, &LowerPipeTexture);
        int p2w = LowerPipeSprite->width;
        int p2h = LowerPipeSprite->height;
        glBindTexture(GL_TEXTURE_2D, LowerPipeTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, p2w, p2h, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, LowerPipeSprite->data);
    }
};

Pipe pipes;

std::chrono::steady_clock::time_point TimeSinceStart;
void startClockPipe()
{
    TimeSinceStart = std::chrono::steady_clock::now();
}

void drawPipes()
{
    int x = 0;
    int y = 0;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glBindTexture(GL_TEXTURE_2D, pipes.UpperPipeTexture);

    glPushMatrix();
    glTranslatef(pipes.posX, pipes.posY + 1.5, pipes.posZ);

    glBegin(GL_QUADS);
    glTexCoord2f(pipes.xc[1], pipes.yc[0]);
    glVertex2i(x, y);
    glTexCoord2f(pipes.xc[1], pipes.yc[1]);
    glVertex2i(x, pipes.UpperPipeSprite->height);
    glTexCoord2f(pipes.xc[0], pipes.yc[1]);
    glVertex2i(pipes.UpperPipeSprite->width, pipes.UpperPipeSprite->height);
    glTexCoord2f(pipes.xc[0], pipes.yc[0]);
    glVertex2i(pipes.UpperPipeSprite->width, y);
    glEnd();

    glPopMatrix();
    glFlush();

    glBindTexture(GL_TEXTURE_2D, pipes.LowerPipeTexture);

    glPushMatrix();
    glTranslatef(pipes.posX, pipes.posY - 1.2, pipes.posZ);
    glBegin(GL_QUADS);
    glTexCoord2f(pipes.xc[0], pipes.yc[1]);
    glVertex2i(x, y);
    glTexCoord2f(pipes.xc[0], pipes.yc[0]);
    glVertex2i(x, pipes.LowerPipeSprite->height + y);
    glTexCoord2f(pipes.xc[1], pipes.yc[0]);
    glVertex2i(pipes.LowerPipeSprite->width + x, pipes.LowerPipeSprite->height + y);
    glTexCoord2f(pipes.xc[1], pipes.yc[1]);
    glVertex2i(pipes.LowerPipeSprite->width + x, y);
    glEnd();

    glPopMatrix();
    glFlush();
}

void pipePhysics()
{
    pipes.posX -= 0.005 *11;
}

// int Score = 0;
// bool Scored = false;
void reset_pipes()
{ // generates next set of pipes
    pipes.posX = 2;
    pipes.posY = random() % 5 * 0.3 - 0.3;
    pipes.posZ = 0;
    pipes.xc[0] = 0;
    pipes.xc[1] = 1;
    pipes.yc[0] = 0;
    pipes.yc[1] = 1;
    Scored = false;
}

struct Coords
{
    float x;
    float y;
    float z;
    int w;
    int h;
};
extern Coords get_bird_coords();
void UpdatePipes(bool *gameRunning, bool *gameOver)
{
    std::chrono::steady_clock::time_point NewTime = std::chrono::steady_clock::now();
    Coords Bird = get_bird_coords();
    if (pipes.posX+0.2799 > Bird.x && pipes.posX-0.2799 < Bird.x )
    {   
        if (pipes.posY + 1.11 < Bird.y || pipes.posY + 0.68 > Bird.y)
        {
            Rect r;
            r.bot = 220;
            r.left = 320;
            r.center = 1;
            ggprint8b(&r, 16, 0x00000000, "Game Over");
            *gameRunning = false;
            *gameOver = true;
            Scored = true;
        }
    }
    if ((pipes.posX < Bird.x) && !Scored)
    {
        Score++;
        Scored = true;
        printf("%d ", Score);
    }
    int ElapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(NewTime - TimeSinceStart).count();
    if (ElapsedTime > 7000)
    {
        reset_pipes();
        startClockPipe();
    }
}

