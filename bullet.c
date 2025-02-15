#include "bullet.h"
#include "common.h"
#include "bots.h"
#include "player.h"
#include <stdbool.h>

Bullet bullets[MAX_BULLETS];
int activeBulletsCount = 0;
extern int levelMap[MAP_HEIGHT][MAP_WIDTH];

void InitBullets()
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        bullets[i].active = false;
        bullets[i].texture = NULL;
    }
}

int CreateBullet(int x, int y, int velX, int velY, SDL_Texture *texture, BulletOwner owner, int idBot)
{
    if (activeBulletsCount >= MAX_BULLETS)
        return 0;

    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!bullets[i].active)
        {
            bullets[i].x = x;
            bullets[i].y = y;
            bullets[i].velX = velX;
            bullets[i].velY = velY;
            bullets[i].texture = texture;
            bullets[i].active = true;
            bullets[i].owner = owner;
            bullets[i].idBot = idBot;
            activeBulletsCount++;
            return 1;
        }
    }
}

void UpdateBullets()
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i].active)
        {
            bullets[i].x += bullets[i].velX;
            bullets[i].y += bullets[i].velY;

            // Bullet border checker
            if (bullets[i].x < 0 || bullets[i].x > SCREEN_WIDTH || bullets[i].y < 0 || bullets[i].y > SCREEN_HEIGHT)
            {
                bullets[i].active = false;
                activeBulletsCount--;
                continue;
            }

            // Map collision checker
            int mapX = bullets[i].x / TEXTURE_SIZE;
            int mapY = bullets[i].y / TEXTURE_SIZE;
            if (levelMap[mapY][mapX] == 0)
            {
                bullets[i].active = false;
                activeBulletsCount--;
            }
        }
    }
}

void RenderBullets(SDL_Renderer *renderer)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i].active && bullets[i].texture)
        {
            SDL_Rect destRect = {
                bullets[i].x,
                bullets[i].y,
                25,
                25};
            SDL_RenderCopy(renderer, bullets[i].texture, NULL, &destRect);
        }
    }
}

void ClearBullets()
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        bullets[i].active = false;
        bullets[i].texture = NULL;
    }
    activeBulletsCount = 0;
}