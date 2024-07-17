#include "raylib.h"

#ifndef GUI_H
#define GUI_H

#define SCREEN_WIDHT 840
#define SCREEN_HEIGHT 600

typedef enum {
    SCREEN_START,
    SCREEN_LEVEL1,
    SCREEN_LEADERBOARD,
    SCREEN_WIN,
} GameScreen;

typedef struct {
    Rectangle rect;
    GameScreen screenSet;
} Button;

// Desenha a tela inicial e verifica o clique dos botões atualizando o estado de jogo
void DrawStartScreen (GameScreen *screen, Font *logo, bool *isWindowOpen);

// Desenha o leaderboard
void drawLeaderboardPage();

// Desenha a tela de ganhar o jogo
void drawWinPage (int *attempts, GameScreen *screen);

#endif