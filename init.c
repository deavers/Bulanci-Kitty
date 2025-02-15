#include <SDL2/SDL_mixer.h>
#include "init.h"
#include "common.h"
#include "bots.h"

SDL_Texture *bulletTexture = NULL;
SDL_Texture *weaponTextures[WEAPON_COUNT] = {NULL};
SDL_Texture *weaponHUD = {NULL};
SDL_Texture *weaponTexturesLocked[WEAPON_COUNT] = {NULL};
SDL_Texture *weaponTexturesActive[WEAPON_COUNT] = {NULL};
SDL_Texture *deathTextures[NUM_CHARACTER_TEXTURES] = {NULL};

extern char botNames[MAX_BOTS][MAX_NAME_LENGTH];
extern int botNameCount;

int InitSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL Loading Error!\n");
        return 100;
    }

    if (TTF_Init() == -1)
    {
        printf("TTF Loading Error!\n");
        return 100;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("IMG_Init Error: %s\n", IMG_GetError());
        TTF_Quit();
        SDL_Quit();
        return -1;
    }
    return 0;
}

SDL_Window *CreateWindow()
{
    SDL_Window *window = SDL_CreateWindow(
        "Bulanci", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN_DESKTOP |
            SDL_WINDOW_SHOWN);
    if (!window)
    {
        printf("Window Loading Error!\n");
        return NULL;
    }
    return window;
}

SDL_Renderer *CreateRenderer(SDL_Window *window)
{
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return NULL;
    }
    return renderer;
}

TTF_Font *LoadFont(const char *fontPath, int fontSize)
{
    TTF_Font *font = TTF_OpenFont(fontPath, fontSize);
    if (!font)
    {
        printf("Error loading font: %s\n", fontPath);
        return NULL;
    }
    return font;
}

void LoadBotNames(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Error: Unable to open bot names file!\n");
        return;
    }

    char buffer[MAX_NAME_LENGTH];
    while (fgets(buffer, MAX_NAME_LENGTH, file))
    {
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character
        strncpy(botNames[botNameCount++], buffer, MAX_NAME_LENGTH);
        if (botNameCount >= MAX_BOT_NAMES)
            break;
    }

    fclose(file);
}

void LoadBullet(SDL_Renderer *renderer)
{
    SDL_Surface *bulletSurface = IMG_Load("Assets/bullet.png");
    if (!bulletSurface)
    {
        printf("Error loading bullet.png!\n");
        return;
    }
    bulletTexture = SDL_CreateTextureFromSurface(renderer, bulletSurface);
    SDL_FreeSurface(bulletSurface);
    if (!bulletTexture)
    {
        printf("Error creating bullet texture!\n");
        return;
    }
}

void LoadWeapons(SDL_Renderer *renderer)
{
    weaponTextures[WEAPON_PISTOL] = IMG_LoadTexture(renderer, "Assets/Textures/Weapons/Pistol.png");
    weaponTextures[WEAPON_RIFLE] = IMG_LoadTexture(renderer, "Assets/Textures/Weapons/Rifle.png");
    weaponTextures[WEAPON_SHOTGUN] = IMG_LoadTexture(renderer, "Assets/Textures/Weapons/Shotgun.png");

    for (int i = 0; i < 3; i++)
    {
        if (!weaponTextures[i])
        {
            printf("Failed to load weapon texture %d!\n", i);
        }
    }
}

void LoadWeaponsLocked(SDL_Renderer *renderer)
{
    weaponTexturesLocked[WEAPON_PISTOL] = IMG_LoadTexture(renderer, "Assets/Textures/Weapons/Pistol.png");
    weaponTexturesLocked[WEAPON_RIFLE] = IMG_LoadTexture(renderer, "Assets/Textures/Weapons/Rifle_locked.png");
    weaponTexturesLocked[WEAPON_SHOTGUN] = IMG_LoadTexture(renderer, "Assets/Textures/Weapons/Shotgun_locked.png");

    for (int i = 0; i < WEAPON_COUNT; i++)
    {
        if (!weaponTexturesLocked[i])
        {
            printf("Failed to load weapon texture %d!\n", i);
        }
    }
}

void LoadWeaponsActive(SDL_Renderer *renderer)
{
    weaponTexturesActive[WEAPON_PISTOL] = IMG_LoadTexture(renderer, "Assets/Textures/Weapons/Pistol_active.png");
    weaponTexturesActive[WEAPON_RIFLE] = IMG_LoadTexture(renderer, "Assets/Textures/Weapons/Rifle_active.png");
    weaponTexturesActive[WEAPON_SHOTGUN] = IMG_LoadTexture(renderer, "Assets/Textures/Weapons/Shotgun_active.png");

    for (int i = 0; i < WEAPON_COUNT; i++)
    {
        if (!weaponTexturesLocked[i])
        {
            printf("Failed to load weapon texture %d!\n", i);
        }
    }
}

void LoadWeaponHUD(SDL_Renderer *renderer)
{
    weaponHUD = IMG_LoadTexture(renderer, "Assets/Textures/Weapons/inf.png");

    if (!weaponHUD)
    {
        printf("Failed to load weapon HUD texture!\n");
    }
}

void FreeWeaponTextures(SDL_Texture *weaponTextures[], SDL_Texture *weaponTexturesLocked[])
{
    for (int i = 0; i < 3; i++)
    {
        if (weaponTextures[i])
        {
            SDL_DestroyTexture(weaponTextures[i]);
        }
    }
    for (int i = 0; i < 2; i++)
    {
        if (weaponTexturesLocked[i])
        {
            SDL_DestroyTexture(weaponTexturesLocked[i]);
        }
    }
    if (weaponHUD)
    {
        SDL_DestroyTexture(weaponHUD);
    }
}

void FreeItemsTextures(SDL_Texture *itemsTextures[])
{
    for (int i = 0; i < ITEM_COUNT; i++)
    {
        if (itemsTextures[i])
        {
            SDL_DestroyTexture(itemsTextures[i]);
        }
    }
}

void CloseSDL(SDL_Window *window, SDL_Renderer *renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}
