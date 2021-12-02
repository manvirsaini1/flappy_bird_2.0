/***************************************
AUTHOR: Manvir saini
FILE: msaini.cpp
DATE: November 29, 2021 
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

void show_msaini_credits(int y, int x)
{
    initialize_fonts();
    Rect r;
    r.bot = y + 20;
    r.left = x;
    r.center = 1;
    ggprint8b(&r, 16, 0x00ff00ff, "Manvir saini did a bit of programming");
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
        //printf("fname **%s**\n", fname);
        char name[40];
        strcpy(name, fname);
        int slen = strlen(name);
        name[slen - 4] = '\0';
        //printf("name **%s**\n", name);
        char ppmname[80];
        sprintf(ppmname, "%s.ppm", name);
        //printf("ppmname **%s**\n", ppmname);
        char ts[100];
        //system("convert eball.jpg eball.ppm");
        sprintf(ts, "convert %s %s", fname, ppmname);
        system(ts);
        //sprintf(ts, "%s", name);
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
Image im[1] = {"bird.png"};

class Bird
{
public:
    GLuint birdTexture;
    Image *sprite;
    float xc[2];
    float yc[2];
    float posX = 0.5;
    float posY = 1;
    float posZ = 0;
} bird;

void init_bird()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE_MINUS_SRC_COLOR, GL_SRC_COLOR);

    bird.sprite = &im[0];
    glGenTextures(1, &bird.birdTexture);
    int bw = bird.sprite->width;
    int bh = bird.sprite->height;
    glBindTexture(GL_TEXTURE_2D, bird.birdTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, bw, bh, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, bird.sprite->data);
    bird.xc[0] = 0.0125;
    bird.xc[1] = 1;
    bird.yc[0] = 0.0125;
    bird.yc[1] = 1;
}
void drawBird(bool *gameRunning, bool *gameOver)
{
    if (bird.posY < 0.001)
    {
        *gameRunning = false;
        *gameOver = true;
    }
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glBindTexture(GL_TEXTURE_2D, bird.birdTexture);

    glPushMatrix();
    glTranslatef(bird.posX, bird.posY, bird.posZ);

    glBegin(GL_QUADS);
    glTexCoord2f(bird.xc[1], bird.yc[0]);
    glVertex2i(200 / 2, 150 / 2);
    glTexCoord2f(bird.xc[1], bird.yc[1]);
    glVertex2i(200 / 2, bird.sprite->height / 4);
    glTexCoord2f(bird.xc[0], bird.yc[1]);
    glVertex2i(bird.sprite->width / 4, bird.sprite->height / 4);
    glTexCoord2f(bird.xc[0], bird.yc[0]);
    glVertex2i(bird.sprite->width / 4, 150 / 2);
    glEnd();

    glPopMatrix();
    glFlush();
}
std::chrono::steady_clock::time_point oldTimeSinceStart;
void startClockBird()
{
    oldTimeSinceStart = std::chrono::steady_clock::now();
}

void birdPhysics()
{
    std::chrono::steady_clock::time_point timeSinceStart = std::chrono::steady_clock::now();
    int deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(timeSinceStart - oldTimeSinceStart).count();
    oldTimeSinceStart = timeSinceStart;
    // bird.posY -= 0.0045; //gravity
    bird.posY -= 9.8 * deltaTime * 0.00005;
    // printf("%f",bird.posY); 0.009
}

void flap()
{
    //add rotation if time is there
    bird.posY += 0.2; // fly up
}
struct Coords
{
    float x;
    float y;
    float z;
    int w;
    int h;
};

Coords get_bird_coords()
{
    Coords b;
    b.x = bird.posX;
    b.y = bird.posY;
    b.z = bird.posZ;
    b.w = bird.sprite->width;
    b.h = bird.sprite->height;
    return b;
}