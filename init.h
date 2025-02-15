#ifndef INIT_H
#define INIT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "common.h"

int InitSDL();
SDL_Window *CreateWindow();
SDL_Renderer *CreateRenderer(SDL_Window *window);
TTF_Font *LoadFont(const char *fontPath, int fontSize);

void CloseSDL(SDL_Window *window, SDL_Renderer *renderer);
void LoadBotNames(const char *filename);
void LoadBullet(SDL_Renderer *renderer);
void LoadWeapons(SDL_Renderer *renderer);
void LoadWeaponsLocked(SDL_Renderer *renderer);
void LoadWeaponHUD(SDL_Renderer *renderer);
void FreeWeaponTextures(SDL_Texture *weaponTextures[], SDL_Texture *weaponTexturesLocked[]);
void FreeItemsTextures(SDL_Texture *itemsTextures[]);

#endif