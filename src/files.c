#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void txtToMap(FILE *map, Texture2D *tileset, Block blocks[], int *blockCount, Jumper jumpers[], int *jumpersCount, Vector2 *spawn_point, Level *level){
    char tileChar;
    Rectangle tileRect = {.width = TILE_SIZE, .height = TILE_SIZE}, destRect = {.width = DISPLAYED_SIZE, .height = DISPLAYED_SIZE};
    int dest_y = 0, dest_x = 0;

    rewind(map);

    // Iteramos cada "linha" e cada coluna
    for (int i = 0; i < MAP_ROWS; i++){
        dest_x = 0;

        for (int j = 0; j < MAP_COLUMNS; j++){
            tileChar = fgetc(map);
            bool shouldDraw = true;

            destRect.x = dest_x, destRect.y = dest_y;

            switch (tileChar){
                // Desenha o chão de terra
                case 'G':
                    blocks[*blockCount].rect = destRect;
                    blocks[*blockCount].active = true;
                    blocks[*blockCount].spike = false;

                    *blockCount += 1;

                    tileRect.x = calcTilePositon(14);
                    tileRect.y = calcTilePositon(9);
                    break;
                
                case '0':
                    tileRect.x = calcTilePositon(12);
                    tileRect.y = calcTilePositon(7);
                    break;

                // Desenha o chão de grama
                case 't':
                    blocks[*blockCount].rect = destRect;
                    blocks[*blockCount].active = true;
                    blocks[*blockCount].spike = false;

                    *blockCount += 1;
                    
                    tileRect.x = calcTilePositon(11);
                    tileRect.y = calcTilePositon(14);
                    break;
                case 'T':
                    blocks[*blockCount].rect = destRect;
                    blocks[*blockCount].active = true;
                    blocks[*blockCount].spike = false;

                    *blockCount += 1;
                    
                    tileRect.x = calcTilePositon(12);
                    tileRect.y = calcTilePositon(14);
                    break;
                case '_':
                    blocks[*blockCount].rect = destRect;
                    blocks[*blockCount].active = true;
                    blocks[*blockCount].spike = false;

                    *blockCount += 1;
                    
                    tileRect.x = calcTilePositon(7);
                    tileRect.y = calcTilePositon(8);
                    break;
                case 'y':
                    blocks[*blockCount].rect = destRect;
                    blocks[*blockCount].active = true;
                    blocks[*blockCount].spike = false;

                    *blockCount += 1;
                    
                    tileRect.x = calcTilePositon(13);
                    tileRect.y = calcTilePositon(14);
                    break;
                // Desenha os espinhos
                case 'S':
                    blocks[*blockCount].rect = (Rectangle){dest_x, dest_y + 40, 60, 20};
                    blocks[*blockCount].active = true;
                    blocks[*blockCount].spike = true;

                    *blockCount += 1;

                    tileRect.x = calcTilePositon(4);
                    tileRect.y = calcTilePositon(10);
                    break;
                
                // Desenha os espinhos invertidos
                case 'V':
                    blocks[*blockCount].rect = (Rectangle){dest_x, dest_y + 40, 60, 20};
                    blocks[*blockCount].active = true;
                    blocks[*blockCount].spike = true;

                    *blockCount += 1;

                    tileRect.x = calcTilePositon(7);
                    tileRect.y = calcTilePositon(9);
                    break;

                // Gera um jumper
                case 'J':
                    shouldDraw = false;
                    jumpers[*jumpersCount].rect = destRect;
                    jumpers[*jumpersCount].activated = false;
                    
                    *jumpersCount += 1;
                    tileRect.x = calcTilePositon(8);
                    tileRect.y = calcTilePositon(6);
                    break;

                // Gera o spawnpoint
                case 'P':
                    shouldDraw = false;
                    *spawn_point = (Vector2){dest_x, dest_y};
                    break;

                // Gera a linha de chegada
                case 'W':
                    level->winCoordinateX = dest_x;

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
    }
}

RenderTexture2D generateMapTexture (FILE *map, Texture2D *tileset, Block blocks[], int *blockCount, Jumper jumpers[], int *jumpersCount, Vector2 *spawn_point, Level *level){
    RenderTexture2D mapTexture = LoadRenderTexture(210 * DISPLAYED_SIZE, MAP_ROWS * DISPLAYED_SIZE);   

    BeginTextureMode(mapTexture);
        txtToMap(map, tileset, blocks, blockCount, jumpers, jumpersCount, spawn_point, level);
    EndTextureMode();

    return mapTexture;
}

void drawMap(RenderTexture2D mapTexture){
    DrawTexturePro(mapTexture.texture, 
        (Rectangle){ 0, 0, mapTexture.texture.width, -mapTexture.texture.height },
        (Rectangle){ 0, 0, mapTexture.texture.width, mapTexture.texture.height },
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
// Cuidado: ela irá sobreescrever dados já existentes!
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

    for (int i = 0; i < MAX_SAVED_PLAYERS; i++){
        snprintf(db_players[i].name, MAX_NAME_CHAR, "Player %d", i + 1);
        db_players[i].attempts = 40 + GetRandomValue(0, 50);
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

        if (attempts < players_db[i].attempts){
            for (int j = MAX_SAVED_PLAYERS - 1; j > i; j--){
                players_db[j] = players_db[j - 1];
            }

            strncpy(players_db[i].name, "Player", MAX_NAME_CHAR);
            players_db[i].attempts = attempts;

            break;
        }
    }

    rewind(leaderboard_db);
    fwrite(players_db, sizeof(PlayerData), MAX_SAVED_PLAYERS, leaderboard_db);
    fclose(leaderboard_db);
}