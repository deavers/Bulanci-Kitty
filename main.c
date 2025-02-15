#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <math.h>
#include <time.h>
#include "init.h"
#include "show_window.h"
#include "buttons.h"
#include "common.h"
#include "main_menu.h"
#include "character.h"
#include "options_menu.h"
#include "records_menu.h"
#include "bullet.h"
#include "bots.h"
#include "collision.h"
#include "player.h"
#include "hud.h"
#include "weapons.h"
#include "options_menu.h"
#include "music.h"
#include "player.h"

const char *Color[6] = {"Red", "Purple", "Orange", "Green", "Gray", "Blue"};

Settings settings;

SDL_Texture *pillowOrange;
SDL_Texture *pillowPurple;
SDL_Texture *pillowGreen;

int levelMap[MAP_HEIGHT][MAP_WIDTH];

extern Bot bots[MAX_BOTS];
extern void RenderBots(SDL_Renderer *renderer);
extern SDL_Texture *weaponTextures[WEAPON_COUNT];
extern SDL_Texture *weaponHUD;
extern Mix_Music *gameMusic[3];
extern Mix_Music *mainMenuMusic;
extern Mix_Music *currentMusic;

int weaponsSpawned = 0;
int playersCount = 1;
bool running = true;
static bool shooting[2] = {false, false};

int LoadLevel(const char *filePath)
{
    FILE *file = fopen(filePath, "r");
    if (!file)
    {
        printf("Error opening level file!\n");
        return 1;
    }

    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            if (fscanf(file, "%d", &levelMap[i][j]) != 1)
            {
                printf("Error reading level data!\n");
                fclose(file);
                return 1;
            }
        }
    }

    fclose(file);
    return 0;
}

void ShootingLogic(SDL_Event *event, SDL_Texture *bulletTexture, int playersCount)
{
    static Uint32 lastShotTime[2] = {0, 0};

    if (event->type == SDL_KEYDOWN)
    {
        if (event->key.keysym.sym == SDLK_LCTRL)
            shooting[0] = true;
        if (playersCount == 2 && event->key.keysym.sym == SDLK_m)
            shooting[1] = true;
    }
    else if (event->type == SDL_KEYUP)
    {
        if (event->key.keysym.sym == SDLK_LCTRL)
            shooting[0] = false;
        if (playersCount == 2 && event->key.keysym.sym == SDLK_m)
            shooting[1] = false;
    }

    Uint32 currentTime = SDL_GetTicks();

    for (int i = 0; i < playersCount; i++)
    {
        if (!shooting[i])
            continue;

        Player *player;

        if (i == 0)
            player = &player1;
        else
            player = &player2;

        if (!player->isAlive)
            continue;

        Uint32 timeSinceLastShot = currentTime - lastShotTime[i];

        if (timeSinceLastShot >= player->fireRate)
        {
            if (player->weapon == WEAPON_SHOTGUN)
            {
                for (int j = -1; j <= 1; j++)
                {
                    int dx, dy;
                    switch (player->direction)
                    {
                    case DIRECTION_UP:
                        dx = j;
                        dy = -1;
                        break;
                    case DIRECTION_DOWN:
                        dx = j;
                        dy = 1;
                        break;
                    case DIRECTION_LEFT:
                        dx = -1;
                        dy = j;
                        break;
                    case DIRECTION_RIGHT:
                        dx = 1;
                        dy = j;
                        break;
                    default:
                        dx = 0;
                        dy = 0;
                        break;
                    }
                    if (player->ammo_left > 0)
                    {
                        player->ammo_left -= 1;
                        if (player->id == 1)
                            CreateBullet(player->x + CHARACTER_SIZE / 2, player->y + CHARACTER_SIZE / 2, dx * 5, dy * 5, bulletTexture, BULLET_PLAYER, 15);
                        else
                            CreateBullet(player->x + CHARACTER_SIZE / 2, player->y + CHARACTER_SIZE / 2, dx * 5, dy * 5, bulletTexture, BULLET_PLAYER2, 16);
                    }
                }
            }
            else
            {
                PlayerShoot(player, bulletTexture);
            }

            lastShotTime[i] = currentTime;
        }
    }
}

void PillowsMainMenu(SDL_Renderer *renderer, SDL_Texture *pillowTextures[], SDL_Rect mainMenuButtonRects[], int mainMenuSelectedButton, TTF_Font *Font, SDL_Texture *pillowOrange, SDL_Texture *pillowPurple, SDL_Texture *pillowGreen)
{
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface *surfaceBulanci = TTF_RenderText_Solid(Font, "BULANCI", white);
    SDL_Texture *textureBulanci = SDL_CreateTextureFromSurface(renderer, surfaceBulanci);

    int bulanciWidth = surfaceBulanci->w * 4;
    int bulanciHeight = surfaceBulanci->h * 4;

    SDL_Rect rectBulanci = {
        (SCREEN_WIDTH - bulanciWidth) / 2,         // Centered horizontally
        (SCREEN_HEIGHT - bulanciHeight) / 2 - 300, // Centered vertically + 300 up
        bulanciWidth,
        bulanciHeight};

    SDL_RenderCopy(renderer, textureBulanci, NULL, &rectBulanci);
    SDL_FreeSurface(surfaceBulanci);
    SDL_DestroyTexture(textureBulanci);

    // Properties
    SDL_Rect rectPillowOrange = {
        rectBulanci.x - 60, // Zleva od B
        rectBulanci.y + 80,
        TEXTURE_SIZE, TEXTURE_SIZE};
    SDL_Rect rectPillowPurple = {
        rectBulanci.x + 240, // Nad A
        rectBulanci.y - 40,
        TEXTURE_SIZE, TEXTURE_SIZE};
    SDL_Rect rectPillowGreen = {
        rectBulanci.x + bulanciWidth / 2 + 180, // Mezi C a I
        rectBulanci.y + bulanciHeight / 2 + 20,
        TEXTURE_SIZE, TEXTURE_SIZE};

    // Characters in main menu
    if (pillowOrange)
        SDL_RenderCopy(renderer, pillowOrange, NULL, &rectPillowOrange);
    if (pillowPurple)
        SDL_RenderCopy(renderer, pillowPurple, NULL, &rectPillowPurple);
    if (pillowGreen)
        SDL_RenderCopy(renderer, pillowGreen, NULL, &rectPillowGreen);
}

void RenderDebugMenu(SDL_Renderer *renderer, TTF_Font *font)
{
    SDL_Color debugColor = {255, 0, 0, 255}; // Красный цвет для дебаг текста
    char buffer[10];
    int offsetX = 0; // Отступ от левого края
    int offsetY = 0; // Отступ от верхнего края
    int spacing = TEXTURE_SIZE;

    for (int i = 0; i < MAP_HEIGHT; i++)
    {
        for (int j = 0; j < MAP_WIDTH; j++)
        {
            int displayValue = levelMap[i][j];
            sprintf(buffer, "%d ", displayValue);

            SDL_Surface *textSurface = TTF_RenderText_Solid(font, buffer, debugColor);
            if (!textSurface)
            {
                printf("Ошибка создания текстовой поверхности: %s\n", TTF_GetError());
                continue;
            }

            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            if (!textTexture)
            {
                printf("Ошибка создания текстуры из поверхности: %s\n", SDL_GetError());
                SDL_FreeSurface(textSurface);
                continue;
            }

            SDL_Rect destRect = {
                offsetX + j * spacing, // Позиция по X
                offsetY + i * spacing, // Позиция по Y
                textSurface->w,
                textSurface->h};

            SDL_RenderCopy(renderer, textTexture, NULL, &destRect);

            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }
    }
}

void SaveGameResults(int playersCount)
{
    FILE *file = fopen("Records/game_results.txt", "w");
    if (file)
    {
        if (playersCount == 1)
        {
            fprintf(file, "%s %d kills %d deaths\n", player1.name, player1.kills, player1.deathCount);
        }
        else
        {
            fprintf(file, "%s %d kills %d deaths\n", player1.name, player1.kills, player1.deathCount);
            fprintf(file, "%s %d kills %d deaths\n", player2.name, player2.kills, player2.deathCount);
        }
        fclose(file);
    }
    else
    {
        printf("Error opening game_results.txt for writing!\n");
    }
}

void ResetGame()
{
    ClearBots();
    ClearBullets();
    ClearWeapons();

    shooting[0] = false;
    shooting[1] = false;
}

void LoadResources(SDL_Renderer *renderer)
{
    pillowOrange = IMG_LoadTexture(renderer, "Assets/Characters/pillowOrange13.png");
    pillowPurple = IMG_LoadTexture(renderer, "Assets/Characters/pillowPurple35.png");
    pillowGreen = IMG_LoadTexture(renderer, "Assets/Characters/pillowGreen72.png");
}

void CleanUp()
{
    SDL_DestroyTexture(pillowOrange);
    SDL_DestroyTexture(pillowPurple);
    SDL_DestroyTexture(pillowGreen);
}

void HandleEvents(SDL_Renderer *renderer, TTF_Font *reloadingFont, SDL_Event *event, GameStatus *status)
{
    switch (*status)
    {
    case STATE_MAIN_MENU:
        MainMenuEvents(event, status);
        break;
    case STATE_PLAY:
        GameStartEvents(event, status, renderer, reloadingFont);
        UpdateBullets();
        ShootingLogic(event, bulletTexture, playersCount);
        break;
    case STATE_GAME_MENU:
        GameMenuEvents(event, status);
        break;
    case STATE_OPTIONS:
        OptionsMenuEvents(renderer, event, status);
        break;
    case STATE_RECORDS:
        RecordsEvents(event, status);
        break;
    case STATE_TOP_1:
        Top1Events(event, status);
        break;
    case STATE_TOP_10:
        Top10Events(event, status);
        break;
    case STATE_GAME_OVER:
        GameOverEvents(event, status);
        break;
    case STATE_QUIT:
        running = false;
        break;
    default:
        break;
    }
}

void HandleWindow(SDL_Renderer *renderer, TTF_Font *Font, SDL_Texture *backgroundTexture, SDL_Texture *levelMapTexture, GameStatus *status, char *colorMain1, char *colorMain2)
{
    switch (*status)
    {
    case STATE_MAIN_MENU:
        ShowMainWindow(renderer, Font, backgroundTexture);
        break;
    case STATE_PLAY:
        ShowGameStartWindow(renderer, Font, levelMapTexture, &player1, &player2, &settings.botCount, &settings.botDifficulty);
        break;
    case STATE_GAME_MENU:
        ShowGameMenuWindow(renderer, Font, backgroundTexture, &settings.botCount, &playersCount, &settings.botDifficulty, colorMain1, colorMain2);
        break;
    case STATE_OPTIONS:
        ShowOptionsWindow(renderer, Font, backgroundTexture, &settings.soundValue, &settings.brightnessValue);
        break;
    case STATE_RECORDS:
        ShowRecordsWindow(renderer, Font, backgroundTexture);
        break;
    case STATE_TOP_1:
        ShowTop1Window(renderer, Font, backgroundTexture);
        break;
    case STATE_TOP_10:
        ShowTop10Window(renderer, Font, backgroundTexture);
        break;
    case STATE_GAME_OVER:
        ShowGameOverWindow(renderer, Font);
        break;
    case STATE_QUIT:
        running = false;
        break;
    default:
        break;
    }
}

void PlayerLiveAnim(SDL_Renderer *renderer, SDL_Texture *pillowMainHUD1[], SDL_Texture *pillowMainHUD2[], char *colorMain1, char *colorMain2)
{
    for (int i = IDLE_DOWN_START; i <= IDLE_DOWN_END; i++)
    {
        char framePath[256];
        sprintf(framePath, "Assets/Characters/pillow%s%d.png", colorMain1, i);
        SDL_Surface *pillowAnimSurface = IMG_Load(framePath);
        // printf("Frame path: %s - %d - %d\n", framePath, i, i - IDLE_DOWN_START);

        if (!pillowAnimSurface)
        {
            printf("Error loading pillow frame - %d!\n", i);
            continue;
        }
        pillowMainHUD1[i - IDLE_DOWN_START] = SDL_CreateTextureFromSurface(renderer, pillowAnimSurface);
        SDL_FreeSurface(pillowAnimSurface);
        if (!pillowMainHUD1[i - IDLE_DOWN_START])
            printf("Error creating pillow texture from %s!\n", framePath);
    }

    for (int i = IDLE_DOWN_START; i <= IDLE_DOWN_END; i++)
    {
        char framePath[256];
        sprintf(framePath, "Assets/Characters/pillow%s%d.png", colorMain2, i);
        SDL_Surface *pillowAnimSurface = IMG_Load(framePath);
        if (!pillowAnimSurface)
        {
            printf("Error loading pillow frame - %d!\n", i);
            continue;
        }
        pillowMainHUD2[i - IDLE_DOWN_START] = SDL_CreateTextureFromSurface(renderer, pillowAnimSurface);
        SDL_FreeSurface(pillowAnimSurface);
        if (!pillowMainHUD2[i - IDLE_DOWN_START])
            printf("Error creating pillow texture from %s!\n", framePath);
    }
}

void PlayerDeadAnim(SDL_Renderer *renderer, SDL_Texture *pillowPlayer1Dead[], SDL_Texture *pillowPlayer2Dead[], char *colorMain1, char *colorMain2)
{
    for (int i = DEAD_DOWN_START; i <= DEAD_DOWN_END; i++)
    {
        char framePath[256];
        sprintf(framePath, "Assets/Characters/pillow%s%d.png", colorMain1, i);
        SDL_Surface *pillowAnimSurface = IMG_Load(framePath);
        // printf("Frame path: %s - %d - %d\n", framePath, i, i - DEAD_DOWN_START);

        if (!pillowAnimSurface)
        {
            printf("Error loading pillow frame - %d!\n", i);
            continue;
        }
        pillowPlayer1Dead[i - DEAD_DOWN_START] = SDL_CreateTextureFromSurface(renderer, pillowAnimSurface);
        SDL_FreeSurface(pillowAnimSurface);
        if (!pillowPlayer1Dead[i - DEAD_DOWN_START])
            printf("Error creating pillow texture from %s!\n", framePath);
    }

    for (int i = DEAD_DOWN_LEFT_START; i <= DEAD_DOWN_LEFT_END; i++)
    {
        char framePath[256];
        sprintf(framePath, "Assets/Characters/pillow%s%d.png", colorMain2, i);
        SDL_Surface *pillowAnimSurface = IMG_Load(framePath);
        if (!pillowAnimSurface)
        {
            printf("Error loading pillow frame - %d!\n", i);
            continue;
        }
        pillowPlayer2Dead[i - DEAD_DOWN_LEFT_START] = SDL_CreateTextureFromSurface(renderer, pillowAnimSurface);
        SDL_FreeSurface(pillowAnimSurface);
        if (!pillowPlayer2Dead[i - DEAD_DOWN_LEFT_START])
            printf("Error creating pillow texture from %s!\n", framePath);
    }
}

int main()
{
    srand(time(NULL));

    // SDL init
    if (InitSDL())
        return 1;

    // Window init
    SDL_Window *window = CreateWindow();
    if (!window)
        return 1;

    // Renderer creater
    SDL_Renderer *renderer = CreateRenderer(window);
    if (!renderer)
        return 1;

    // Alpha enable
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // Load sounds
    Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    LoadMusic();

    // Load font
    TTF_Font *Font = LoadFont("Assets/Font/Lilita_One_Regular.ttf", 35);
    if (!Font)
        return 1;
    TTF_Font *reloadingFont = LoadFont("Assets/Font/Lilita_One_Regular.ttf", 15);
    if (!reloadingFont)
        return 1;

    // Initialize Background
    SDL_Surface *backgroundSurface = IMG_Load("Assets/mainmenu_back.jpg");
    if (!backgroundSurface)
    {
        printf("Error loading background image!\n");
        return 1;
    }
    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    // Load level
    if (LoadLevel("Assets/level_map.txt") != 0)
    {
        printf("Error loading level!\n");
        return 1;
    }
    SDL_Texture *levelMapTexture = IMG_LoadTexture(renderer, "Assets/Map/level3.png");
    if (!levelMapTexture)
    {
        printf("Error loading level_map.png!\n");
        return 1;
    }

    int xColor1 = rand() % 6; // 0 - 5
    char *colorMain1 = (char *)Color[xColor1];

    int xColor2 = rand() % 6; // 0 - 5

    if (xColor1 == xColor2)
        xColor2 = (xColor2 + 1) % 6;

    char *colorMain2 = (char *)Color[xColor2];

    // printf("X color: %d\n", xColor);
    // printf("Main character color: %s\n", colorMain);

    // Load character textures
    if (LoadCharacterTextures(renderer, colorMain1, colorMain2) != 0)
    {
        printf("Error loading character textures!\n");
        return 1;
    }

    // Load Player 1 live animation
    SDL_Texture *pillowMainHUD1[PILLOW_ANIMATION_FRAMES];
    SDL_Texture *pillowMainHUD2[PILLOW_ANIMATION_FRAMES];
    PlayerLiveAnim(renderer, pillowMainHUD1, pillowMainHUD2, colorMain1, colorMain2);

    // Load Player 1 dead animation
    SDL_Texture *pillowPlayer1Dead[PILLOW_ANIMATION_FRAMES_DEAD];
    SDL_Texture *pillowPlayer2Dead[PILLOW_ANIMATION_FRAMES_DEAD];
    PlayerDeadAnim(renderer, pillowPlayer1Dead, pillowPlayer2Dead, colorMain1, colorMain2);

    // Load pillow textures
    for (int i = 0; i < GIF_FRAME_COUNT; i++)
    {
        char frame_Path[256];
        sprintf(frame_Path, "Assets/pillow_frames/frame_%d.png", i);
        SDL_Surface *pillowSurface = IMG_Load(frame_Path);
        if (!pillowSurface)
        {
            printf("Error loading pillow frame - %d\n", i);
            continue;
        }
        pillowTextures[i] = SDL_CreateTextureFromSurface(renderer, pillowSurface);
        SDL_FreeSurface(pillowSurface);
    }

    // Load Settings
    settings.botCount = 1;
    settings.botDifficulty = 1; // 1 easy, 2 medium, 3 hard
    settings.soundValue = 5;
    settings.brightnessValue = 10;

    // Load Resourses
    LoadResources(renderer);

    // Load Bullets
    InitBullets();
    LoadBullet(renderer);

    // Load Weapons
    LoadWeaponHUD(renderer);
    LoadWeapons(renderer);
    LoadWeaponsLocked(renderer);

    // Load Bots
    LoadBotNames("Assets/bots_name.txt");
    InitBots(renderer, Color, NUM_COLORS);
    bool botsSpawned = false;

    // Main Characters
    InitPlayer(&player1, 0, 0, 1);
    InitPlayer(&player2, 0, 0, 2);

    // Game status
    GameStatus status = STATE_MAIN_MENU;
    SDL_Event event;
    running = true;
    bool showDebugMenu = false;

    // Main menu
    int currentFrame = 0;
    Uint32 lastFrameTime = 0;
    Uint32 gameStartTime = 0;

    Mix_VolumeMusic(settings.soundValue * 12.8); // Set volume to 50%
    PlayMusic(mainMenuMusic);

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
                status = STATE_MAIN_MENU;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F1)
                showDebugMenu = !showDebugMenu;

            HandleEvents(renderer, reloadingFont, &event, &status);
        }

        const Uint8 *keystate = SDL_GetKeyboardState(NULL);

        if (status == STATE_PLAY)
        {
            if (gameStartTime == 0)
            {
                int randMusic = rand() % 3;
                PlayMusic(gameMusic[randMusic]);
                gameStartTime = SDL_GetTicks();

                RespawnPlayer(&player1);
                if (playersCount == 2)
                    RespawnPlayer(&player2);
            }

            UpdatePlayer(&player1, keystate);

            if (playersCount == 2)
                UpdatePlayer(&player2, keystate);

            UpdateBots();
            UpdateBullets();

            HandleCollisions(&player1, &player2);

            HandleWeaponPickup(&player1);
            if (playersCount == 2)
                HandleWeaponPickup(&player2);
        }

        if (!player1.isAlive && SDL_GetTicks() - player1.deathTime >= 3000)
            RespawnPlayer(&player1);

        if (playersCount == 2 && !player2.isAlive && SDL_GetTicks() - player2.deathTime >= 3000)
            RespawnPlayer(&player2);

        if (status == STATE_PLAY && !botsSpawned)
        {
            InitBots(renderer, Color, NUM_COLORS);
            botsSpawned = true;
        }

        if (status == STATE_PLAY && (player1.kills >= 20 || player2.kills >= 20) && settings.botDifficulty == 1)
        {
            status = STATE_GAME_OVER;

            gameStartTime = 0;
            botsSpawned = false;

            ResetGame();

            PlayMusic(mainMenuMusic);
        }

        if (status == STATE_PLAY && SDL_GetTicks() - gameStartTime >= TOTAL_GAME_TIME)
        {
            status = STATE_GAME_OVER;
            gameStartTime = 0;
            botsSpawned = false;

            ResetGame();

            PlayMusic(mainMenuMusic);
        }

        HandleWindow(renderer, Font, backgroundTexture, levelMapTexture, &status, colorMain1, colorMain2);

        if (status == STATE_PLAY)
        {
            if (weaponsSpawned < MAX_WEAPONS)
                WeaponMainMenu(renderer);

            UpdateWeapons();
            RenderHUD(renderer, &player1, &player2, SDL_GetTicks() - gameStartTime, pillowMainHUD1, pillowMainHUD2, pillowPlayer1Dead, pillowPlayer2Dead);
            HUDDraw(renderer, &player1, &player2, SDL_GetTicks() - gameStartTime);
        }

        if (status == STATE_MAIN_MENU)
        {
            PillowsMainMenu(renderer, pillowTextures, mainMenuButtonRects, mainMenuSelectedButton, Font, pillowOrange, pillowPurple, pillowGreen);
            PlayMusic(mainMenuMusic);
            gameStartTime = 0;
            botsSpawned = false;
            ResetGame();
            player1.kills = 0;
            player2.kills = 0;
        }

        if (showDebugMenu)
            RenderDebugMenu(renderer, Font);

        UpdateBrightness(renderer);

        SDL_RenderPresent(renderer);
    }

    ClearBullets();
    ClearBots();
    ClearWeapons();
    ClearCharacterTextures();

    for (int i = 0; i < PILLOW_ANIMATION_FRAMES; i++)
    {
        if (pillowTextures[i])
            SDL_DestroyTexture(pillowTextures[i]);
    }
    for (int i = 0; i < WEAPON_COUNT; i++)
    {
        if (weaponTextures[i])
            SDL_DestroyTexture(weaponTextures[i]);
    }

    SDL_DestroyTexture(levelMapTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(bulletTexture);

    TTF_CloseFont(Font);
    TTF_CloseFont(reloadingFont);
    FreeMusic();
    Mix_CloseAudio();
    CleanUp();
    CloseSDL(window, renderer);
    return 0;
}