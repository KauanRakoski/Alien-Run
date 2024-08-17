#include "raylib.h"
#include "files.h"
#include <stdio.h>
#include "gui.h"
#include "objects.h"

int main (void){
    // Lemos o arquivo de mapa, se houver algum problema retornamos
    FILE *map = fopen("./resources/map1.txt", "r");
    if (map == NULL) return 1;

    // Verificamos se o leaderboar está inicializado corretamente
    // Se necessário, inicializamos
    initLeaderboard(SCORES_DATABASE);

    // Variável para monitorar o estado da janela
    bool isWindowOpen = true;

    // Inicializamos o dispositivo de áudio e
    // carregamos os sons utilizados
    InitAudioDevice();

    Music soundTrack = LoadMusicStream("./resources/soundtrack.mp3");
    SetMusicVolume(soundTrack, 0.5f);
    PlayMusicStream(soundTrack);

    Sound death_exp = LoadSound("./resources/death_sound.mp3");
    
    // Definimos a tela inicial de jogo
    // Um spawnpoint padrão e uma estrutura de nível padrão
    GameScreen screen = SCREEN_START;
    Vector2 Spawnpoint = {0, 200};

    Level level = {
        .attempts = 1,
        .winCoordinateX = 18000
    };

    // Criamos duas matrizes para armazenar os jumpers
    // e os blocos padrões do jogo, além de contadores de suas quantidades
    Jumper jumpers[MAX_JUMPERS];
    int jmpCount = 0;

    Block blocks[MAX_BLOCKS];
    int blockCount = 0;
    InitBlocks(blocks);

    // Inicializamos uma janela e carregamos os recursos gráficos
    InitWindow(SCREEN_WIDHT, SCREEN_HEIGHT, "Inf-Dash");

    Texture2D tilemap = LoadTexture("./resources/monochrome_tilemap_transparent.png");
    Font logo = LoadFont("./resources/karma_future.TTF");
    
    // Calculamos quantos blocos ("Pedaços de mapa") precisamos para renderizar um mapa
    // do tamanho desejado e criamos uma matriz de textura para renderizá-lo
    int numBlocks = MAP_COLUMNS / BLOCK_COLUMNS;
    RenderTexture2D targets[numBlocks];
    for (int i = 0; i < numBlocks; i++){
        targets[i] = LoadRenderTexture(BLOCK_COLUMNS * DISPLAYED_SIZE, MAP_ROWS * DISPLAYED_SIZE);
    }

    // Processamos as informações relevantes do mapa e geramos sua textura
    processMap(map, blocks, &blockCount, jumpers, &jmpCount, &Spawnpoint, &level);
    renderMapInColumns(map, &tilemap, targets, numBlocks);

    // Utilizamos informações do mapa (spawnpoint)
    // e geramos um registro de jogador
    Player player = {
        .position = Spawnpoint,
        .speed = {400.0f, 300.0f},
        .gravity = 0,
        .attempts = 1,
        .anim = {ANIM_WALK, 2, 0.2f, 0.0f, 4}
    };

    SetTargetFPS(60); // O jogo roda em 60 FPS

    // Inicializamos uma câmera que acompanha o jogador em sua posição X
    Camera2D camera = {0};
    camera.target = (Vector2){.x = player.position.x, .y = 200};
    camera.offset = (Vector2){  SCREEN_WIDHT/2.0f, SCREEN_HEIGHT/2.0f };
    camera.zoom = 0.75f;

    SetExitKey(0); // Removemos esc para sair do jogo

    while (!WindowShouldClose()){

        if(!isWindowOpen) break; // Se a variável de estado do jogo for falsa, fechamos

        float deltaTime = GetFrameTime(); // utilizaremos o dt para calculos de física
        
        // Se o jogador estiver no jogo, gerenciamos colisões, jumpers, atualização, música
        // E checamos por compleção de nível
        if (screen == SCREEN_LEVEL1){
            camera.target.x = player.position.x;
            checkColisions(&player, blocks, blockCount, &death_exp, jumpers, jmpCount, &level, &soundTrack, Spawnpoint);
            checkJumpers(&player, jumpers, jmpCount);
            updatePlayer(&player, deltaTime);

            UpdateMusicStream(soundTrack);

            checkWin(&player, &level, &screen, &Spawnpoint, jumpers, jmpCount, &soundTrack);

            if (IsKeyPressed(KEY_ESCAPE)) screen = SCREEN_PAUSE;
        }
        // Se estivermos em leaderboar e quisermos sair, apertamos esc
        if (screen == SCREEN_LEADERBOARD){
            if (IsKeyPressed(KEY_ESCAPE)){
                screen = SCREEN_START;
            }
        }

        // Aqui está a nossa lógica de desenho   
        BeginDrawing();
        ClearBackground((Color){34, 35, 35, 255});

        if (screen == SCREEN_START){
            DrawStartScreen(&screen, &logo, &isWindowOpen);
        }

        else if (screen == SCREEN_LEADERBOARD){
            drawLeaderboardPage();
        }

        else if (screen == SCREEN_LEVEL1){
            BeginMode2D(camera);                

                // Desenhamos as partes do mapa
                for (int i = 0; i < numBlocks; i++) {
                    drawMap(targets[i], i);
                }

                // Desenhamos o jogador, a barra de progresso e os jumpers
                drawPlayer(&player, &tilemap);
                drawProgressBar (player.position.x, level.winCoordinateX);
                drawJumpers(jumpers, jmpCount, &tilemap);
                
            EndMode2D();
        }

        else if (screen == SCREEN_PAUSE) DrawPauseMenu(&screen);

        else if (screen == SCREEN_WIN) drawWinPage(&player.attempts, &screen);
        
        EndDrawing();
    }

    for (int i = 0; i < numBlocks; i++) {
        UnloadRenderTexture(targets[i]);
    }

    fclose(map);

    // Liberamos os recursos
    UnloadFont(logo);
    UnloadTexture(tilemap);

    UnloadMusicStream(soundTrack);
    UnloadSound(death_exp);

    CloseWindow();

    return 0;
}