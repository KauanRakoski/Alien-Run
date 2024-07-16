#include "raylib.h"

#ifndef OBJECTS_H
#define OBJECTS_H

#define MAX_BLOCKS 2000
#define MAX_JUMPERS 10
#define G_UP 600
#define G_DOWN 1100

typedef struct {
    Vector2 position;
    Vector2 speed;
    bool canJump;
    float Jumpspeed;
    float gravity;
    int attempts;
} Player;

typedef struct {
    Rectangle rect;
    bool active;
    bool spike;
} Block;

typedef struct {
    Rectangle rect;
    int activated;
} Jumper;

typedef struct {
    int attempts;
    float winCoordinateX;
} Level;

void updatePlayer (Player *player, float deltaTime);
void drawPlayer (Player *player);

void InitBlocks(Block *blocks);
void checkColisions (Player *player, Block *blocks, int blockCount, Sound *death_exp, Jumper *jumpers, int jmpCount, Level *level);

void checkJumpers (Player *player, Jumper *jumpers, int jmpCount);
void drawJumpers (Jumper *jumpers, int jmpCount, Texture2D *tileset);

void drawProgressBar (float playerPositionX, float winCoordinatesX);

#endif