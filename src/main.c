#include "raylib.h"
#include "files.h"
#include <stdio.h>
#include "gui.h"
#include "objects.h"

int main (void){
    FILE *map = fopen("./resources/testmap.txt", "r");
    if (map == NULL) return 1;

    initLeaderboard(SCORES_DATABASE);

    bool isWindowOpen = true;
    InitAudioDevice();

    Music soundTrack = LoadMusicStream("./resources/soundtrack.mp3");
    SetMusicVolume(soundTrack, 0.5f);
    PlayMusicStream(soundTrack);

    Sound death_exp = LoadSound("./resources/death_sound.mp3");
    
    GameScreen screen = SCREEN_START;
    Vector2 Spawnpoint;

    Level level = {
        .attempts = 1,
    };

    Jumper jumpers[MAX_JUMPERS];
    int jmpCount = 0;

    Block blocks[MAX_BLOCKS];
    int blockCount = 0;

    InitBlocks(blocks);

    InitWindow(SCREEN_WIDHT, SCREEN_HEIGHT, "Inf-Dash");

    // Texture2D tilemap = LoadTexture("./resources/tilemap.png");
    Texture2D tilemap = LoadTexture("./resources/monochrome_tilemap_transparent.png");
    // Texture2D game_bg = LoadTexture("./resources/tilemap-backgrounds.png");

    Font logo = LoadFont("./resources/karma_future.TTF");

    // RenderTexture2D mapTexture = generateMapTexture(map, &tilemap, blocks, &blockCount, jumpers, &jmpCount, &Spawnpoint, &level);
    
    int numBlocks = MAP_COLUMNS / BLOCK_COLUMNS;

    RenderTexture2D targets[numBlocks];
    for (int i = 0; i < numBlocks; i++){
        targets[i] = LoadRenderTexture(BLOCK_COLUMNS * DISPLAYED_SIZE, MAP_ROWS * DISPLAYED_SIZE);
    }

    processMap(map, blocks, &blockCount, jumpers, &jmpCount, &Spawnpoint, &level);
    renderMapInColumns(map, &tilemap, targets, numBlocks);

    Player player = {
        .position = Spawnpoint,
        .speed = {400.0f, 300.0f},
        .gravity = 0,
        .attempts = 1
    };

    SetTargetFPS(60);

    Camera2D camera = {0};

    camera.target = (Vector2){.x = player.position.x, .y = 200};
    camera.offset = (Vector2){  SCREEN_WIDHT/2.0f, SCREEN_HEIGHT/2.0f };
    camera.zoom = 0.75f;

    SetExitKey(0);

    while (!WindowShouldClose()){

        if(!isWindowOpen) break;

        float deltaTime = GetFrameTime();
        camera.target.x = player.position.x;

        if (screen == SCREEN_LEVEL1){
            checkColisions(&player, blocks, blockCount, &death_exp, jumpers, jmpCount, &level);
            checkJumpers(&player, jumpers, jmpCount);
            updatePlayer(&player, deltaTime);

            UpdateMusicStream(soundTrack);
        }
        if (screen == SCREEN_LEADERBOARD){
            if (IsKeyPressed(KEY_ESCAPE)){
                screen = SCREEN_START;
            }
        }

        if (player.position.x >= level.winCoordinateX){
            checkStoreScore (SCORES_DATABASE, player.attempts);
            player.position = Spawnpoint;
            screen = SCREEN_WIN;
        }
            

        
        BeginDrawing();

        if (screen == SCREEN_START){
            DrawStartScreen(&screen, &logo, &isWindowOpen);
        }
        else if (screen == SCREEN_LEADERBOARD){
            drawLeaderboardPage();
        }

        else if (screen == SCREEN_LEVEL1){
            BeginMode2D(camera);
 
                ClearBackground((Color){34, 35, 35, 255});

                       
                for (int i = 0; i < numBlocks; i++) {
                    drawMap(targets[i], i);
                }

                // drawMap(mapTexture);
                drawProgressBar (player.position.x, level.winCoordinateX);
                drawJumpers(jumpers, jmpCount, &tilemap);
                drawPlayer(&player);

            EndMode2D();

        }

        if (screen == SCREEN_WIN){
            drawWinPage(&player.attempts, &screen);
        }
        
        EndDrawing();
    }

    for (int i = 0; i < numBlocks; i++) {
        UnloadRenderTexture(targets[i]);
    }

    fclose(map);

    UnloadFont(logo);
    UnloadTexture(tilemap);

    UnloadMusicStream(soundTrack);
    UnloadSound(death_exp);

    CloseWindow();

    return 0;
}