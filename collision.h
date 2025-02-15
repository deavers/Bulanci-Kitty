#ifndef COLLISION_H
#define COLLISION_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "player.h"
#include "bots.h"

bool IsWalkable(int x, int y, int width, int height);
void HandleCollisions(Player *player1, Player *player2);
void DrawCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius, SDL_Color color);
void DrawCircleOutline(SDL_Renderer *renderer, int centerX, int centerY, int radius, SDL_Color color);

#endif