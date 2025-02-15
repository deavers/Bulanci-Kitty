#include "character.h"
#include "common.h"
#include <SDL2/SDL_image.h>

SDL_Texture *characterTextures1[NUM_CHARACTER_TEXTURES] = {NULL};
SDL_Texture *characterTextures2[NUM_CHARACTER_TEXTURES] = {NULL};


int LoadTextures(SDL_Renderer *renderer, SDL_Texture *textures[], char *color)
{
    char filePath[256];
    SDL_Surface *surface = NULL;

    for (int i = 1; i < NUM_CHARACTER_TEXTURES - 9; i++)
    {
        sprintf(filePath, "Assets/Characters/pillow%s%d.png", color, i);
        surface = IMG_Load(filePath);
        if (!surface)
        {
            printf("Error loading character texture: %s\n", filePath);
            return -1;
        }
        textures[i - 1] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }

    return 0;
}

int LoadCharacterTextures(SDL_Renderer *renderer, char *color1, char *color2)
{
    // Load textures for the first player
    if (LoadTextures(renderer, characterTextures1, color1) != 0)
        return -1;

    // Load textures for the second player
    if (LoadTextures(renderer, characterTextures2, color2) != 0)
        return -1;

    return 0;
}

void DrawCharacter(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y)
{
    SDL_Rect destRect = {
        x,
        y,
        TEXTURE_SIZE - 10,
        TEXTURE_SIZE - 10};
    SDL_RenderCopy(renderer, texture, NULL, &destRect);
}

void ClearCharacterTextures()
{
    for (int i = 0; i < NUM_CHARACTER_TEXTURES; i++)
    {
        if (characterTextures1[i])
        {
            SDL_DestroyTexture(characterTextures1[i]);
            characterTextures1[i] = NULL;
        }
        if (characterTextures2[i])
        {
            SDL_DestroyTexture(characterTextures2[i]);
            characterTextures2[i] = NULL;
        }
    }
}
