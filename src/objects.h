#include "raylib.h"
#include "gui.h"

#ifndef OBJECTS_H
#define OBJECTS_H

#define MAX_BLOCKS 2000
#define MAX_JUMPERS 10
#define G_UP 700
#define G_DOWN 1500

typedef enum {
    ANIM_WALK,
    ANIM_JUMP
} AnimationType;

typedef struct {
    AnimationType type;
    int currentFrame;
    float frameTime;
    float frameCounter;
    int lastFrame;
} Animation;

typedef struct {
    Vector2 position;
    Vector2 speed;
    bool canJump;
    float Jumpspeed;
    float gravity;
    int attempts;

    Animation anim;
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
void drawPlayer (Player *player, Texture2D *tileset);

void InitBlocks(Block *blocks);
void checkColisions (Player *player, Block *blocks, int blockCount, Sound *death_exp, Jumper *jumpers, int jmpCount, Level *level);

void checkJumpers (Player *player, Jumper *jumpers, int jmpCount);
void drawJumpers (Jumper *jumpers, int jmpCount, Texture2D *tileset);

void drawProgressBar (float playerPositionX, float winCoordinatesX);

void checkWin(Player *player, Level *level, GameScreen *screen, Vector2 *Spawnpoint, Jumper *jumpers, int jmpCount);

#endif