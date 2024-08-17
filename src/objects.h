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

void updatePlayer (Player *player, float deltaTime); // atualiza a posição do jogador e suas animações
void drawPlayer (Player *player, Texture2D *tileset); // desenha a textura do jogador

void InitBlocks(Block *blocks); // Inicializa a matriz de blocos colidíveis

// Checa as colisões do jogador com os blocos e reinicia o jogo (incluindo jumpers) em caso de "morte"
void checkColisions (Player *player, Block *blocks, int blockCount, Sound *death_exp, Jumper *jumpers, int jmpCount, Level *level, Music *Soundtrack, Vector2 Spawnpoint);

void checkJumpers (Player *player, Jumper *jumpers, int jmpCount); // gerencia a função e animação dos jumpers
void drawJumpers (Jumper *jumpers, int jmpCount, Texture2D *tileset); // desenha os jumpers

void drawProgressBar (float playerPositionX, float winCoordinatesX); // desenha a barra que indica porcentagem do nível completada

void checkWin(Player *player, Level *level, GameScreen *screen, Vector2 *Spawnpoint, Jumper *jumpers, int jmpCount, Music *Soundtrack); // gerencia o sistema de completar o nível

#endif
