#include "objects.h"
#include "files.h"

// =========================
//          Jogador
// =========================
void updatePlayer (Player *player, float deltaTime){

    if (IsKeyDown(KEY_SPACE) && player->canJump){
        player->speed.y = -350;
        player->canJump = false;
    }

    player->position.x += player->speed.x * deltaTime;

    player->speed.y += player->gravity * deltaTime;
    player->position.y += player->speed.y * deltaTime;
};

void drawPlayer (Player *player){
    DrawRectangle(player->position.x, player->position.y, 60, 60, WHITE);
}

// =========================
//    Barra de progresso
// =========================
void drawProgressBar (float playerPositionX, float winCoordinatesX){
    DrawRectangle(playerPositionX - 300, -160, 600, 30, BLACK);
    DrawRectangle(playerPositionX - 300, -160, (playerPositionX / winCoordinatesX) * 600, 30, WHITE);
}

// =========================
//        Colisores
// =========================
void InitBlocks(Block *blocks) {
    for (int i = 0; i < MAX_BLOCKS; i++) {
        blocks[i].rect = (Rectangle){0, 0, DISPLAYED_SIZE, DISPLAYED_SIZE};
        blocks[i].active = false;
    }
}

int isAbovePlatform(Block *bl, Rectangle playerRect){
    return (bl->active && !bl->spike &&
            bl->rect.x <= playerRect.x + playerRect.width && // a margem esquerda da pf menor que a direita do jogador
            bl->rect.x + bl->rect.width >= playerRect.x && // a margem esquerda do jogador maior que a direita da pf
            bl->rect.y <= playerRect.y + playerRect.height && // player adentra plataforma
            playerRect.y + playerRect.height <=  bl->rect.y + 20);
}

void checkColisions (Player *player, Block *blocks, int blockCount, Sound *death_exp, Jumper *jumpers, int jmpCount, Level *level){
    Rectangle playerRect = {.x = player->position.x, .y = player->position.y, .width = DISPLAYED_SIZE, .height = DISPLAYED_SIZE};
    bool onPlatform = false;

    for (int i = 0; i < blockCount; i ++){
        Block *bl = blocks + i;

        if (isAbovePlatform(bl, playerRect)){
                onPlatform = true;
                player->gravity = 0;
                player->speed.y = 0;
                player->position.y = bl->rect.y - playerRect.height;
                break;
            }
        else if (CheckCollisionRecs(playerRect, bl->rect)){
            PlaySound(*death_exp);
            
            player->position = (Vector2){80, 200};
            player->speed.y = 0;
            player->attempts++;
            level->attempts++;

            for (int i = 0; i < jmpCount; i++){
                jumpers[i].activated = false;
            }
        }
    }

    if (!onPlatform){
        player->canJump = false;
        
        if (player->speed.y > 0)
            player->gravity = G_DOWN;
        else
            player->gravity = G_UP;
    } else
        player->canJump = true;
}

// =========================
//         Jumpers
// =========================
void checkJumpers (Player *player, Jumper *jumpers, int jmpCount){
    Rectangle playerRect = {.x = player->position.x, .y = player->position.y,
                            .width = DISPLAYED_SIZE, .height = DISPLAYED_SIZE};

    for (int i = 0; i < jmpCount; i++){
        Jumper *jmp = jumpers + i;

        if (CheckCollisionRecs(playerRect, jmp->rect)){
            player->speed.y = -500;
            jmp->activated = true;
        }
    }
}

void drawJumpers (Jumper *jumpers, int jmpCount, Texture2D *tileset){
    Rectangle source = {.y = calcTilePositon(9), .width = TILE_SIZE, .height = TILE_SIZE};

    for (int i = 0; i < jmpCount; i++){
        Jumper *jmp = jumpers + i;

        if (jmp->activated)
            source.x = calcTilePositon(5);
        else
            source.x = calcTilePositon(4);
            
        DrawTexturePro(*tileset, source, jmp->rect, (Vector2){0,0}, 0.0f, WHITE);
    }
}

// =========================
//      Desenho geral
// =========================