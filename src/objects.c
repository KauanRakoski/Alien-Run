#include "objects.h"
#include "files.h"
#include "gui.h"

// =========================
//          Jogador
// =========================
void updatePlayer (Player *player, float deltaTime){

    // Lógica de Pulo
    if (IsKeyDown(KEY_SPACE) && player->canJump){
        player->speed.y = -350;
        player->canJump = false; // evita double jump
    }

    // Lógica Posicional
    player->position.x += player->speed.x * deltaTime;

    player->speed.y += player->gravity * deltaTime;
    player->position.y += player->speed.y * deltaTime;

    // Lógica de animação
    if (player->canJump){
        // Enquanto o jogador estiver no chão, incrementamos a contagem de tempo de frame
        player->anim.frameCounter += deltaTime;

        // Se o tempo ultrapassado for maior que o intervalo de frame,
        // movemos para o próximo frame do sprite e reiniciamos o contador
        if (player->anim.frameCounter >= player->anim.frameTime){
            player->anim.frameCounter = 0.0f;
            player->anim.currentFrame++;

            // Caso ultrapasse as animações, voltamos à primeira
            if (player->anim.currentFrame > player->anim.lastFrame)
                player->anim.currentFrame = 2; // Começa no 2
        }
    } else{
        // Se o jogador estiver no ar, simplesmente deixamos no sprite de queda
        player->anim.currentFrame = 5;
    }
    
};

void drawPlayer (Player *player, Texture2D *tileset){
    // Criamos um retângulo baseado na posição do jogador para ser a origem de destino
    Rectangle playerDraw = {(int)player->position.x - 20, (int)player->position.y - 20, 80, 80};

    // e baseado no frame de animação, buscamos a origem da animação no tilesheet
    Rectangle sourceRect = {
        .x = calcTilePositon(player->anim.currentFrame),
        .y = calcTilePositon(13),
        .width = TILE_SIZE,
        .height = TILE_SIZE
    };
    
    DrawTexturePro(*tileset, sourceRect, playerDraw, (Vector2){0, 0}, 0.0f, WHITE);
}

// =========================
//    Barra de progresso
// =========================
void drawProgressBar (float playerPositionX, float winCoordinatesX){
    DrawRectangle((int)playerPositionX - 300, -160, 600, 30, BLACK);
    DrawRectangle((int)playerPositionX - 300, -160, (playerPositionX / winCoordinatesX) * 600, 30, WHITE);
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

// Função que testa a condição do jogador estar sobre a plataforma
int isAbovePlatform(Block *bl, Rectangle playerRect){
    // ela precisa estar ativa e não ser um espinho
    return (bl->active && !bl->spike &&
            bl->rect.x <= playerRect.x + playerRect.width && // a margem esquerda da pf menor que a direita do jogador
            bl->rect.x + bl->rect.width >= playerRect.x && // a margem esquerda do jogador maior que a direita da pf
            bl->rect.y <= playerRect.y + playerRect.height && // player adentra plataforma
            playerRect.y + playerRect.height <=  bl->rect.y + 20);
}

void checkColisions (Player *player, Block *blocks, int blockCount, Sound *death_exp, Jumper *jumpers, int jmpCount, Level *level){
    Rectangle playerRect = {.x = player->position.x, .y = player->position.y, .width = DISPLAYED_SIZE, .height = DISPLAYED_SIZE};
    bool onPlatform = false;

    // Para cada colisor da matriz de colisores,
    for (int i = 0; i < blockCount; i ++){
        Block *bl = blocks + i;

        // Se o jogador estiver sobre o colisor
        if (isAbovePlatform(bl, playerRect)){
                onPlatform = true;
                // Resetamos a gravidade e fazemos o jogador parar de cair
                player->gravity = 0;
                player->speed.y = 0;
                // Posicionamos o jogador imediatamente acima da plataforma
                player->position.y = bl->rect.y - playerRect.height;
                break;
            }
        // Se o jogador não estiver em cima do colisor, mas ainda colidir com ele resetamos o nível
        else if (CheckCollisionRecs(playerRect, bl->rect)){
            PlaySound(*death_exp);
            
            player->position = (Vector2){80, 200};
            player->speed.y = 0;
            player->attempts++;
            level->attempts++;

            // resetamos o estado dos jumpers para sua animação
            for (int i = 0; i < jmpCount; i++){
                jumpers[i].activated = false;
            }
        }
    }

    // Se o jogador estiver no ar, ele não pode pular
    if (!onPlatform){
        player->canJump = false;

        // Dividimos a gravidade para a queda ser pesada e o pulo ser leve
        if (player->speed.y > 0)
            player->gravity = G_DOWN;
        else
            player->gravity = G_UP;
    } else
        player->canJump = true; // se o jogador estiver na plataforma, ele pode pular
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
//        Win Logic
// =========================
void checkWin(Player *player, Level *level, GameScreen *screen, Vector2 *Spawnpoint, Jumper *jumpers, int jmpCount){
    if (player->position.x >= level->winCoordinateX){
        checkStoreScore(SCORES_DATABASE, player->attempts);
        player->position = *Spawnpoint;
        *screen = SCREEN_WIN;

        for (int i = 0; i < jmpCount; i++){
            Jumper *jmp = jumpers + i;

            jmp->activated = false;
        }
    }

    return;
}
