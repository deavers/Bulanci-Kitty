#ifndef BULLET_H
#define BULLET_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "common.h"

extern Bullet bullets[MAX_BULLETS];

void InitBullets();
int CreateBullet(int x, int y, int velX, int velY, SDL_Texture *texture, BulletOwner owner, int idBot);
void UpdateBullets();
void RenderBullets(SDL_Renderer *renderer);
void ClearBullets();

#endif