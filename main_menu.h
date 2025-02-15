#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <SDL2/SDL.h>
#include "common.h"
#include "character.h"

void MainMenuEvents(SDL_Event *event, GameStatus *status);
void GameMenuEvents(SDL_Event *event, GameStatus *status);
void GameStartEvents(SDL_Event *event, GameStatus *status, SDL_Renderer *renderer, TTF_Font *font);
void GameOverEvents(SDL_Event *event, GameStatus *status);

#endif