#ifndef RECORDS_MENU_H
#define RECORDS_MENU_H

#include <SDL2/SDL.h>
#include "common.h"

void RecordsEvents(SDL_Event *event, GameStatus *status);
void Top1Events(SDL_Event *event, GameStatus *status);
void Top10Events(SDL_Event *event, GameStatus *status);

#endif
