// Pocket Trivia
// Kevin Wong

#include "spritehandler.h"

#ifndef _GALACTICDEFENSE_H
#define _GALACTICDEFENSE_H

#define BLACK makecol(0,0,0)
#define WHITE makecol(255,255,255)
#define YELLOW makecol(255,216,0)
#define RED makecol(255,0,0)
#define ORANGE makecol(255,127.5,0)

#define PI 3.1415926535
#define ACCELERATION 0.1f
#define STEP 3

#define NUM 20
#define WIDTH 800
#define HEIGHT 600
#define MODE GFX_AUTODETECT_WINDOWED
#define CHAR_PER_LENGTH 8
#define BULLET_CAP 15
#define BULLETSPEED 6
#define BULLETCOOLDOWN 10;
#define SMALLASTEROID_COUNT 7
#define LARGEASTEROID_COUNT 3
#define ASTEROID_COUNT SMALLASTEROID_COUNT + LARGEASTEROID_COUNT
#define ASTEROID_SPEED 

#define SPACESHIP_SPRITE "sprites/Spaceship.bmp"
#define BACKGROUND_SPRITE "sprites/Background.bmp"
#define BULLET_SPRITE "sprites/Bullet.bmp"
#define SMALL_ASTEROID_SPRITE "sprites/Asteroid1.bmp"
#define LARGE_ASTEROID_SPRITE "sprites/Asteroid20.bmp"
#define LARGE_ASTEROID_SPRITE2 "sprites/Asteroid21.bmp"
#define LARGE_ASTEROID_SPRITE3 "sprites/Asteroid22.bmp"

// Gameover variable
int gameover = 0;
int bullet_index = 0;
int bullet_cooldown = 0;
int score = 0;
int hardmode = 0;

//create a back buffer
BITMAP *buffer;
BITMAP *background;
sprite *spaceship;
spritehandler *bullets;
spritehandler *asteroids;

#endif
