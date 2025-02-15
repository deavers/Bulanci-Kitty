#include "collision.h"
#include "common.h"
#include "player.h"
#include "bullet.h"
#include "bots.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

extern int levelMap[MAP_HEIGHT][MAP_WIDTH];
extern Bot bots[MAX_BOTS];
extern int activeBulletsCount;
extern int playersCount;

bool IsWalkable(int x, int y, int width, int height)
{
    int left = x / TEXTURE_SIZE;
    int top = y / TEXTURE_SIZE;
    int right = (x + width - 1) / TEXTURE_SIZE;
    int bottom = (y + height - 1) / TEXTURE_SIZE;

    return (levelMap[top][left] == 1 &&
            levelMap[top][right] == 1 &&
            levelMap[bottom][left] == 1 &&
            levelMap[bottom][right] == 1);
}

void HandleCollisions(Player *player1, Player *player2)
{
    SDL_Rect player1Rect = {player1->x, player1->y, TEXTURE_SIZE - 10, TEXTURE_SIZE - 10};
    SDL_Rect player2Rect = {player2->x, player2->y, TEXTURE_SIZE - 10, TEXTURE_SIZE - 10};

    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i].active)
        {
            SDL_Rect bulletRect = {bullets[i].x, bullets[i].y, 25, 25};

            if (bullets[i].owner == BULLET_PLAYER || bullets[i].owner == BULLET_PLAYER2)
            {
                for (int j = 0; j < MAX_BOTS; j++)
                {
                    if (bots[j].active)
                    {
                        SDL_Rect botRect = {bots[j].x, bots[j].y, CHARACTER_SIZE, CHARACTER_SIZE};
                        if (SDL_HasIntersection(&bulletRect, &botRect))
                        {
                            bullets[i].active = false;
                            bots[j].active = false;

                            if (bullets[i].owner == BULLET_PLAYER)
                                player1->kills++;
                            else
                                player2->kills++;

                            RespawnBot(&bots[j]);
                            break;
                        }
                    }
                }
            }
            else if (bullets[i].owner == BULLET_BOT)
            {
                if (SDL_HasIntersection(&bulletRect, &player1Rect))
                {
                    bullets[i].active = false;

                    if (player1->armor > 0)
                    {
                        player1->armor -= 100;
                        if (player1->armor < 0)
                        {
                            player1->healthPoint -= 100;
                            player1->armor = 0;
                        }
                    }
                    else
                        player1->healthPoint -= 100;

                    if (player1->healthPoint <= 0)
                    {
                        player1->isAlive = false;
                        player1->deathTime = SDL_GetTicks();
                    }
                }
                else if (SDL_HasIntersection(&bulletRect, &player2Rect))
                {
                    bullets[i].active = false;

                    if (player2->armor > 0)
                    {
                        player2->armor -= 100;
                        if (player2->armor < 0)
                        {
                            player2->healthPoint -= 100;
                            player2->armor = 0;
                        }
                    }
                    else
                        player2->healthPoint -= 100;

                    if (player2->healthPoint <= 0)
                    {
                        player2->isAlive = false;
                        player2->deathTime = SDL_GetTicks();
                    }
                }

                for (int j = 0; j < MAX_BOTS; j++)
                {
                    if (bots[j].active && j != bullets[i].idBot)
                    {
                        SDL_Rect botRect = {bots[j].x, bots[j].y, CHARACTER_SIZE, CHARACTER_SIZE};
                        if (SDL_HasIntersection(&bulletRect, &botRect))
                        {
                            bullets[i].active = false;
                            bots[j].active = false;

                            bots[bullets[i].idBot].kills++;

                            RespawnBot(&bots[j]);
                            break;
                        }
                    }
                }
            }
        }
    }
}

void DrawCircle(SDL_Renderer *renderer, int centerX, int centerY, int radius, SDL_Color color)
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx * dx + dy * dy) <= (radius * radius))
            {
                SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
            }
        }
    }
}

void DrawCircleOutline(SDL_Renderer *renderer, int centerX, int centerY, int radius, SDL_Color color)
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); // SDL_BLENDMODE_BLEND - ENABLES ALPHA
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    int x = radius - 1, y = 0, dx = 1, dy = 1, err = dx - (radius << 1);

    while (x >= y)
    {
        SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY - y);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY - x);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY - x);
        SDL_RenderDrawPoint(renderer, centerX + x, centerY - y);

        if (err <= 0)
        {
            y++;
            err += dy;
            dy += 2;
        }
        if (err > 0)
        {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}
