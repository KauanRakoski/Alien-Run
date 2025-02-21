#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "files.h"
#include "objects.h"

// ====================
//        MAPA
// ====================

/*
A ideia é criar funções genéricas que leiam e gerem um mundo a partir de qualquer mapa.
Desse modo, a possível adição de níveis torna-se algo simples (apenas criar outro arquivo)
*/

int calcTilePositon (int tileColumnOrRow){
    int tileIndex = (tileColumnOrRow - 1); 
    return tileIndex * (TILE_SIZE + TILE_SPACING);
}

void processMap(FILE *map, Block *blocks, int *blockCount, Jumper *jumpers, int *jumpersCount, Vector2 *spawnPoint, Level *level){
    char tilechar;
    Rectangle destRect = {.width = DISPLAYED_SIZE, .height = DISPLAYED_SIZE};
    int dest_x = 0, dest_y = 0;

    rewind(map);

    for (int i = 0; i < MAP_ROWS; i++){

        dest_x = 0;

        for (int j = 0; j < MAP_COLUMNS; j++){
            tilechar = fgetc(map);

            destRect.x = dest_x, destRect.y = dest_y;

            switch (tilechar){
                // Bloco de terra
                case 'G':
                    blocks[*blockCount].rect = destRect;
                    blocks[*blockCount].active = true;
                    blocks[*blockCount].spike = false;

                    *blockCount += 1;
                    break;

                // Bloco de grama
                case 't':
                    blocks[*blockCount].rect = destRect;
                    blocks[*blockCount].active = true;
                    blocks[*blockCount].spike = false;

                    *blockCount += 1;
                    break;

                case 'T':
                    blocks[*blockCount].rect = destRect;
                    blocks[*blockCount].active = true;
                    blocks[*blockCount].spike = false;

                    *blockCount += 1;
                    break;
                case 'y':
                    blocks[*blockCount].rect = destRect;
                    blocks[*blockCount].active = true;
                    blocks[*blockCount].spike = false;

                    *blockCount += 1;
                    break;

                // Gera uma plataforma
                case '_':
                    blocks[*blockCount].rect = destRect;
                    blocks[*blockCount].active = true;
                    blocks[*blockCount].spike = false;

                    *blockCount += 1;
                    break;

                // Gera um espinho
                case 'O':
                    blocks[*blockCount].rect = (Rectangle){dest_x, dest_y + 40, 60, 20};
                    blocks[*blockCount].active = true;
                    blocks[*blockCount].spike = true;

                    *blockCount += 1;
                    break;
                // Gera um espinho invertido
                case 'V':
                    blocks[*blockCount].rect = (Rectangle){dest_x, dest_y + 40, 60, 20};
                    blocks[*blockCount].active = true;
                    blocks[*blockCount].spike = true;

                    *blockCount += 1;
                    break;

                // Gera um Jumper
                case 'J':
                    jumpers[*jumpersCount].rect = destRect;
                    jumpers[*jumpersCount].activated = false;
                    
                    *jumpersCount += 1;
                    break;
                // Gera o spawnpoint
                case 'P':
                    *spawnPoint = (Vector2){dest_x, dest_y};
                    break;
                // Gera a linha de chegada
                case 'W':
                    level->winCoordinateX = dest_x;
                    break;

                default:
                    break;
            }

            dest_x += DISPLAYED_SIZE;
        }
        
        dest_y += DISPLAYED_SIZE;
    }
}

void txtToMap(FILE *map, Texture2D *tileset, int startCol){
    char tileChar;
    Rectangle tileRect = {.width = TILE_SIZE, .height = TILE_SIZE}, destRect = {.width = DISPLAYED_SIZE, .height = DISPLAYED_SIZE};
    int dest_y = 0, dest_x = 0;

    rewind(map);

    // Iteramos cada "linha" e cada coluna
    for (int i = 0; i < MAP_ROWS; i++){

        // Avançamos até a coluna de início de leitura
        for (int j = 0; j < startCol; j++){
            fgetc(map);
        }

        dest_x = 0;

        for (int j = startCol; j < startCol + BLOCK_COLUMNS && j < MAP_COLUMNS; j++){

            tileChar = fgetc(map);
            bool shouldDraw = true;

            destRect.x = dest_x, destRect.y = dest_y;

            switch (tileChar){
                // Desenha o chão de terra
                case 'G':
                    tileRect.x = calcTilePositon(14);
                    tileRect.y = calcTilePositon(9);
                    break;
                
                case '0':
                    tileRect.x = calcTilePositon(12);
                    tileRect.y = calcTilePositon(7);
                    break;

                // Desenha o chão de grama
                case 't':
                    tileRect.x = calcTilePositon(11);
                    tileRect.y = calcTilePositon(14);
                    break;

                case 'T':
                    tileRect.x = calcTilePositon(12);
                    tileRect.y = calcTilePositon(14);
                    break;
                case '_':            
                    tileRect.x = calcTilePositon(7);
                    tileRect.y = calcTilePositon(8);
                    break;
                case 'y':
                    tileRect.x = calcTilePositon(13);
                    tileRect.y = calcTilePositon(14);
                    break;

                case 'e':
                    tileRect.x = calcTilePositon(11);
                    tileRect.y = calcTilePositon(15);
                    break;
                
                 case 'd':
                    tileRect.x = calcTilePositon(13);
                    tileRect.y = calcTilePositon(15);
                    break;
                // Desenha os espinhos
                case 'O':
                    tileRect.x = calcTilePositon(4);
                    tileRect.y = calcTilePositon(10);
                    break;
                
                // Desenha os espinhos invertidos
                case 'V':
                    tileRect.x = calcTilePositon(7);
                    tileRect.y = calcTilePositon(9);
                    break;

                // Gera a linha de chegada
                case 'W':
                    tileRect.x = calcTilePositon(7);
                    tileRect.y = calcTilePositon(10);
                    break;
                case '|':
                    tileRect.x = calcTilePositon(7);
                    tileRect.y = calcTilePositon(11);
                    break;
                
                // Se for outro caractére, ignoramos
                default:
                    shouldDraw = false;
                    break;
            }
            
            if (shouldDraw)
                DrawTexturePro(*tileset, tileRect, destRect, (Vector2){0, 0}, 0.0f, WHITE);

            dest_x += DISPLAYED_SIZE;
        }

        dest_y += DISPLAYED_SIZE;

        // Avançamos até o final da linha para passar pra próxima
        for (int j = startCol + BLOCK_COLUMNS; j < MAP_COLUMNS; j++){
            fgetc(map);
        }
    }
}

// O mapa é muito grande para ser renderizado em uma única vez, por isso quebramos em pedaços e
// criamos uma matriz de texturas para desenhá-lo.
void renderMapInColumns(FILE *map, Texture2D *tileset, RenderTexture2D targets[], int numBlocks) {
    for (int block = 0; block < numBlocks; block++) {
        BeginTextureMode(targets[block]);
            ClearBackground((Color){34, 35, 35, 255});
            txtToMap(map, tileset, block * BLOCK_COLUMNS);
        EndTextureMode();
    }
}

// Desenha as texturas do mapa de modo correto
void drawMap(RenderTexture2D mapTexture, int blockCol){
    DrawTexturePro(mapTexture.texture, 
        (Rectangle){ 0, 0, mapTexture.texture.width, -mapTexture.texture.height }, // precisamos inverter por diferentes sistemas interpretarem y de modo diferente
        (Rectangle){ blockCol * BLOCK_COLUMNS * DISPLAYED_SIZE, 0, mapTexture.texture.width, mapTexture.texture.height },
                       (Vector2){ 0, 0 }, 0.0f, WHITE);
}

// ====================
//     LEADERBOARD
// ====================

/*
O placar de melhores jogadores será armazenado em um arquivo binário na forma de array de objetos.
Cada objeto identificara um jogador pelo nome e pelo número de tentativas (quanto menos, melhor)
*/

// Função que inicializa um novo banco de dados de leaderboard
// Caso ele não exista ainda
void initLeaderboard (const char *filename){
    FILE *leaderboard_db = fopen(filename, "rb");

    // Se já existir banco de dados, retornamos
    if (leaderboard_db != NULL){
        fseek(leaderboard_db, 0, SEEK_END);
        long fileSize = ftell(leaderboard_db);
        fclose(leaderboard_db);

        if (fileSize > 0) return;
    }

    leaderboard_db = fopen(filename, "wb");
    rewind(leaderboard_db);

    PlayerData db_players[MAX_SAVED_PLAYERS];
    
    if (!leaderboard_db){
        perror("Unable to load leaderboard database");
        exit(EXIT_FAILURE);
    }

    // Criamos jogadores genéricos
    for (int i = 0; i < MAX_SAVED_PLAYERS; i++){
        snprintf(db_players[i].name, MAX_NAME_CHAR, "Player %d", i + 1);
        db_players[i].attempts = 40 + i;
    }

    fwrite(db_players, sizeof(db_players), MAX_SAVED_PLAYERS, leaderboard_db);

    fclose(leaderboard_db);
}

// Função que carrega o leaderboard em uma variável passada como ponteiro, para visualização.
void loadLeaderboard (const char *filename, PlayerData *player_scores){
    FILE *leaderboard_db = fopen(filename, "rb");

    fread(player_scores, sizeof(PlayerData), MAX_SAVED_PLAYERS, leaderboard_db);
    
    fclose(leaderboard_db);
}

// Função que recebe o valor de tentativas do jogador e armazena se for menor que alguma das já armazenadas
void checkStoreScore (const char *filename, int attempts){
    FILE *leaderboard_db = fopen(filename, "rb+");

    PlayerData players_db[MAX_SAVED_PLAYERS];

    fread(players_db, sizeof(PlayerData), MAX_SAVED_PLAYERS, leaderboard_db);

    for (int i = 0; i < MAX_SAVED_PLAYERS; i++){
        // Se o jogador tiver concluído em menos tentativas que db[i], 
        if (attempts < players_db[i].attempts){
            // Passamos cada jogador para a próxima posição
            for (int j = MAX_SAVED_PLAYERS - 1; j > i; j--){
                players_db[j] = players_db[j - 1];
            }

            // E colocamos ele no lugar de db[i] (db[i] foi para db[i+1])
            // Garantindo seu posicionamento correto
            strncpy(players_db[i].name, "Player 0", MAX_NAME_CHAR);
            players_db[i].attempts = attempts;

            break;
        }
    }

    // Voltamos ao início do arquivo e escrevemos a leaderboard atualizada
    rewind(leaderboard_db);
    fwrite(players_db, sizeof(PlayerData), MAX_SAVED_PLAYERS, leaderboard_db);
    fclose(leaderboard_db);
}