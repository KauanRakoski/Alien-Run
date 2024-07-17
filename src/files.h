#include <stdio.h>
#include "objects.h"
#include "raylib.h"

#ifndef FILES_H
#define FILES_H

#define TILE_SIZE 16
#define TILE_SPACING 1
#define DISPLAYED_SIZE 60

#define MAP_COLUMNS 300
#define MAP_ROWS 10

#define BLOCK_COLUMNS 100

#define SCORES_DATABASE "./resources/top_scores.bin"
#define MAX_NAME_CHAR 10
#define MAX_SAVED_PLAYERS 5


typedef struct {
    char name[MAX_NAME_CHAR];
    int attempts;
} PlayerData;

int calcTilePositon (int tileColumnOrRow);

// Traduz as informações do arquivo de texto em desenhos
void txtToMap(FILE *map, Texture2D *tileset, int startCol);
// Processa o mapa e gera os colisores e blocos especiais
void processMap(FILE *map, Block blocks[], int *blockCount, Jumper jumpers[], int *jumpersCount, Vector2 *spawnPoint, Level *level);
// Gera uma textura de mapa (através de txtToMap), com ela, gera um array de blocos colidíveis, define o spawnpoint e a linha de chegada
RenderTexture2D generateMapTexture (FILE *map, Texture2D *tileset, Block blocks[], int *blockCount, Jumper jumpers[], int *jumpersCount, Vector2 *spawn_point, Level *level);
// Desenha a textura do mapa na tela
void drawMap (RenderTexture2D mapTexture, int blockCol);

// Gera os blocos de renderização do mapa
void renderMapInColumns(FILE *map, Texture2D *tileset, RenderTexture2D targets[], int numBlocks);

// Inicializa a leaderboard com nomes aleatórios. Sobrescreve dados já existentes
void initLeaderboard (const char *filename);
// Carrega o leaderboard para um array passado por referência
void loadLeaderboard (const char *filename, PlayerData *player_scores);
// Armazena o novo número de tentativas caso seja menor que algum do leaderboard
// e atualiza a leaderboard
void checkStoreScore (const char *filename, int attempts);

#endif