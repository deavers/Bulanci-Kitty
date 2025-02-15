#ifndef OPTIONS_MENU_H
#define OPTIONS_MENU_H

#include <SDL2/SDL.h>
#include "common.h"

void OptionsMenuEvents(SDL_Renderer *renderer, SDL_Event *event, GameStatus *status);
void UpdateBrightness(SDL_Renderer *renderer);
void UpdateSound();

#endif
