#ifndef CHARACTERS_H
#define CHARACTERS_H

#include <SDL2/SDL.h>
#include "common.h"

// RIGHT
#define IDLE_RIGHT_START 5
#define IDLE_RIGHT_END 12
#define WALK_RIGHT_START 37
#define WALK_RIGHT_END 40

// LEFT
#define IDLE_LEFT_START 13
#define IDLE_LEFT_END 20
#define WALK_LEFT_START 41
#define WALK_LEFT_END 44

// UP
#define IDLE_UP_START 21
#define IDLE_UP_END 28
#define WALK_UP_START 45
#define WALK_UP_END 48

// DOWN
#define IDLE_DOWN_START 29
#define IDLE_DOWN_END 36
#define WALK_DOWN_START 49
#define WALK_DOWN_END 52
#define DEAD_DOWN_START 83
#define DEAD_DOWN_END 86

// DOWN LEFT
#define WALK_DOWN_LEFT_START 63
#define WALK_DOWN_LEFT_END 66
#define DEAD_DOWN_LEFT_START 87
#define DEAD_DOWN_LEFT_END 90

extern SDL_Texture *characterTextures1[NUM_CHARACTER_TEXTURES];
extern SDL_Texture *characterTextures2[NUM_CHARACTER_TEXTURES];

int LoadTextures(SDL_Renderer *renderer, SDL_Texture *textures[], char *color);
int LoadCharacterTextures(SDL_Renderer *renderer, char *color1, char *color2);
void DrawCharacter(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y);
void ClearCharacterTextures();

#endif