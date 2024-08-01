#include "gui.h"
#include "raylib.h"
#include "files.h"

void checkButtons (Vector2 *mousePosition,  GameScreen *screen, bool *isWindowOpen){
    Button startBtn = {.rect = {.x = 300, .y = 310, .width = 200, .height = 50}, .screenSet = SCREEN_LEVEL1};
    Button leaderBoardBtn =  {.rect = {.x = 280, .y = 360, .width = 200, .height = 50}, .screenSet = SCREEN_LEADERBOARD};
    Button quitBtn = {.rect = {.x = 300, .y = 410, .width = 200, .height = 50},};
    
    Button buttons[] = {startBtn, leaderBoardBtn};
    int numButtons = sizeof(buttons) / sizeof(Button);

    for (int i = 0; i < numButtons; i++){
        Button button = buttons[i];

        if (CheckCollisionPointRec(*mousePosition, button.rect)){
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                *screen = button.screenSet;
            }
        }
    }

    if (CheckCollisionPointRec(*mousePosition, quitBtn.rect)){
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            *isWindowOpen = false;
    }
}

void DrawStartScreen (GameScreen *screen, Font *logo, bool *isWindowOpen){
    ClearBackground((Color){34, 35, 35, 255});

    DrawTextEx(*logo, "ALIEN RUN", (Vector2){200, 150}, 100, 1.0f, WHITE);

    DrawText("Iniciar", 300, 330, 30, WHITE);
    DrawText("Leaderboard", 280, 380, 30, WHITE);
    DrawText("Sair", 300, 430, 30, WHITE);

    Vector2 mousePos = GetMousePosition();

    checkButtons(&mousePos, screen, isWindowOpen);    
}

void drawLeaderboardPage(){
    PlayerData player_scores[MAX_SAVED_PLAYERS];
    loadLeaderboard(SCORES_DATABASE, player_scores);
    char p[50];

    ClearBackground((Color){34, 35, 35, 255});
    DrawText("Leaderboard", 240, 50, 50, WHITE);

    for (int i = 0; i < MAX_SAVED_PLAYERS; i++){
        snprintf(p, 50, "%s \t\t\t\t %d", player_scores[i].name, player_scores[i].attempts);
        DrawText(p, 240, 300 + 40 * i, 30, WHITE);
    }
}

void drawWinPage (int *attempts, GameScreen *screen){
    char texto[15];

    sprintf(texto, "%d tentativas", *attempts);

    DrawRectangle(300, 150, 300, 300, (Color){0,0,0,200});
    DrawText("Fase concluída!", 340, 270, 30, WHITE);
    DrawText(texto, 400, 320, 20, WHITE);

    if(IsKeyPressed(KEY_ENTER)){
        *screen = SCREEN_START;
        *attempts = 1;
    }
}