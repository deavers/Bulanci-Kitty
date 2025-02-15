#ifndef HUD_H
#define HUD_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "player.h"

void DrawHUDPlayer(SDL_Renderer *renderer, Player *player1, Player *player2);
void DrawDeathAnimation(SDL_Renderer *renderer, Player *player, SDL_Texture *deathTextures[]);
void RenderHUD(SDL_Renderer *renderer, Player *player1, Player *player2, Uint32 elapsedTime, 
SDL_Texture *pillowMainHUD1[], SDL_Texture *pillowMainHUD2[], SDL_Texture *pillowPlayer1Dead[], SDL_Texture *pillowPlayer2Dead[]);
void HUDDraw(SDL_Renderer *renderer, Player *player1, Player *player2, Uint32 elapsedTime);

#endif
