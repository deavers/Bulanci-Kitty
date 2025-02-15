#ifndef SHOW_WINDOW_H
#define SHOW_WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "character.h"
#include "player.h"

void ShowMainWindow(SDL_Renderer *renderer, TTF_Font *font, SDL_Texture *backgroundTexture);
void ShowGameMenuWindow(SDL_Renderer *renderer, TTF_Font *font, SDL_Texture *backgroundTexture, int *botCount, int *playersCount, int *botDifficulty, char *color1, char *color2);
void ShowGameStartWindow(SDL_Renderer *renderer, TTF_Font *font, SDL_Texture *levelmap, Player *player1, Player *player2, int *botCount, int *botDifficulty);
void ShowOptionsWindow(SDL_Renderer *renderer, TTF_Font *font, SDL_Texture *backgroundTexture, int *soundValue, int *brightnessValue);
void ShowRecordsWindow(SDL_Renderer *renderer, TTF_Font *font, SDL_Texture *backgroundTexture);
void ShowTop1Window(SDL_Renderer *renderer, TTF_Font *font, SDL_Texture *backgroundTexture);
void ShowTop10Window(SDL_Renderer *renderer, TTF_Font *font, SDL_Texture *backgroundTexture);
void ShowGameOverWindow(SDL_Renderer *renderer, TTF_Font *font);

#endif