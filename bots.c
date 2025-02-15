#include "bots.h"
#include "common.h"
#include "player.h"
#include "bullet.h"
#include "character.h"
#include "collision.h"
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>

Bot bots[MAX_BOTS];
char botNames[MAX_BOTS][MAX_NAME_LENGTH];
int botNameCount = 0;
SDL_Surface *surface = NULL;

extern Settings settings;
extern SDL_Texture *bulletTexture;
extern int levelMap[MAP_HEIGHT][MAP_WIDTH];
extern int activeBulletsCount;
extern int playersCount;

void ChangeBotDirection(Bot *bot)
{
    int newDirection = rand() % NUM_BOT_DIRECTIONS;
    bot->currentDirection = newDirection;
}

bool CheckCollisionWithWall(Bot *bot, int dx, int dy)
{
    // if wall
    int newX = bot->x + dx * bot->velX;
    int newY = bot->y + dy * bot->velY;

    if (newX < 0 || newY < 0 || newX >= MAP_WIDTH * TEXTURE_SIZE || newY >= MAP_HEIGHT * TEXTURE_SIZE)
    {
        return true; // if out of border
    }

    // get border at new position of map
    return levelMap[newY / TEXTURE_SIZE][newX / TEXTURE_SIZE] != 0;
}

bool IsClearPath(Bot *bot, float targetX, float targetY)
{
    int startX = bot->x / TEXTURE_SIZE;
    int startY = bot->y / TEXTURE_SIZE;
    int endX = targetX / TEXTURE_SIZE;
    int endY = targetY / TEXTURE_SIZE;

    int dx = abs(endX - startX);
    int dy = abs(endY - startY);
    int sx = (startX < endX) ? 1 : -1;
    int sy = (startY < endY) ? 1 : -1;
    int err = dx - dy;

    while (true)
    {
        // Get every line and check clearnes
        if (levelMap[startY][startX] == 0)
        {
            return false;
        }

        if (startX == endX && startY == endY)
            break;

        int e2 = err * 2;
        if (e2 > -dy)
        {
            err -= dy;
            startX += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            startY += sy;
        }
    }

    // Direction clear
    return true;
}

void BotMove(Bot *bot)
{
    int dx = 0, dy = 0;

    switch (bot->currentDirection)
    {
    case DIRECTION_UP:
        dy = -1;
        break;
    case DIRECTION_DOWN:
        dy = 1;
        break;
    case DIRECTION_LEFT:
        dx = -1;
        break;
    case DIRECTION_RIGHT:
        dx = 1;
        break;
    }

    bot->x += dx * bot->velX;
    bot->y += dy * bot->velY;

    if (CheckCollisionWithWall(bot, dx, dy))
    {
        ChangeBotDirection(bot);
    }
}

void BotShootAtTarget(Bot *bot, float targetX, float targetY, SDL_Texture *bulletTexture)
{
    if (bot->ammo_left > 0)
    {
        int dx = targetX - bot->x;
        int dy = targetY - bot->y;
        int distanceSquared = dx * dx + dy * dy;
        int detectionRadius = 200;

        if (distanceSquared <= detectionRadius * detectionRadius)
        {
            // Check if clear path
            if (IsClearPath(bot, targetX, targetY))
            {
                if (SDL_GetTicks() - bot->lastShootTime >= bot->shootDelay)
                {
                    bot->ammo_left -= 1;
                    if (bot->ammo_left == 0)
                    {
                        bot->ammo_left = 12;
                        bot->full_ammo = 10000;
                    }

                    float distance = sqrtf(distanceSquared);
                    float normDx = (dx / distance);
                    float normDy = (dy / distance);

                    CreateBullet(bot->x + CHARACTER_SIZE / 2, bot->y + CHARACTER_SIZE / 2,
                                 normDx * 5, normDy * 5, bulletTexture, BULLET_BOT, bot->id);

                    bot->lastShootTime = SDL_GetTicks();
                }
            }
        }
    }
}

int LoadBotTextures(SDL_Renderer *renderer, SDL_Texture *textures[], const char *color)
{
    char filePath[256];

    for (int i = 0; i < BOT_ANIMATION_FRAMES - 1; i++)
    {
        sprintf(filePath, "Assets/Characters/pillow%s%d.png", color, i + 1);
        surface = IMG_Load(filePath);
        if (!surface)
        {
            printf("Error loading bot texture: %s\n", filePath);
            return -1;
        }
        textures[i] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!textures[i])
        {
            printf("Error creating texture for %s\n", filePath);
            return -1;
        }
    }

    return 0;
}

void InitBots(SDL_Renderer *renderer, const char *botColors[], int numColors)
{
    for (int i = 0; i < MAX_BOTS; i++)
    {
        bots[i].active = false;
    }

    for (int i = 0; i < settings.botCount; i++)
    {
        int xrand = 1 + rand() % (MAP_WIDTH - 2);
        int yrand = 1 + rand() % (MAP_HEIGHT - 2);

        while (levelMap[yrand][xrand] != 1)
        {
            xrand = 1 + rand() % (MAP_WIDTH - 2);
            yrand = 1 + rand() % (MAP_HEIGHT - 2);
        }

        bots[i].active = true;
        bots[i].isAlive = true;
        bots[i].id = i;
        bots[i].x = xrand * TEXTURE_SIZE;
        bots[i].y = yrand * TEXTURE_SIZE;
        bots[i].currentDirection = rand() % NUM_BOT_DIRECTIONS;
        bots[i].currentFrame = 1;
        bots[i].lastFrameTime = SDL_GetTicks();
        bots[i].frameDelay = 80;
        bots[i].lastShootTime = 0;
        bots[i].weapon = WEAPON_PISTOL;
        bots[i].ammo_left = 12;
        bots[i].full_ammo = 10000;
        bots[i].shootDelay = 1000;

        strncpy(bots[i].name, botNames[i], MAX_NAME_LENGTH);
        const char *color = botColors[rand() % numColors];

        if (LoadBotTextures(renderer, bots[i].textures, color) != 0)
        {
            printf("Failed to load bot textures for bot %d\n", i);
            bots[i].active = false;
        }
    }
}

void RenderBots(SDL_Renderer *renderer)
{
    for (int i = 0; i < settings.botCount; i++)
    {
        if (!bots[i].active || !bots[i].isAlive || !bots[i].textures[bots[i].currentFrame])
            continue;

        int frame = bots[i].currentFrame;
        SDL_Texture *texture = bots[i].textures[frame];
        if (texture)
        {
            SDL_Rect destRect = {bots[i].x, bots[i].y, TEXTURE_SIZE - 10, TEXTURE_SIZE - 10};
            SDL_RenderCopy(renderer, texture, NULL, &destRect);
        }
    }
}

void RespawnBot(Bot *bot)
{
    int Xrand, Yrand;
    Xrand = 1 + rand() % (MAP_WIDTH - 2);
    Yrand = 1 + rand() % (MAP_HEIGHT - 2);

    while (levelMap[Yrand][Xrand] != 1)
    {
        Xrand = 1 + rand() % (MAP_WIDTH - 2);
        Yrand = 1 + rand() % (MAP_HEIGHT - 2);
    }

    bot->x = Xrand * TEXTURE_SIZE;
    bot->y = Yrand * TEXTURE_SIZE;
    bot->active = true;
    bot->isAlive = true;
    bot->currentDirection = rand() % NUM_BOT_DIRECTIONS;
    bot->currentFrame = 1;
    bot->lastFrameTime = SDL_GetTicks();
    bot->state = BOT_STATE_MOVING;
    bot->stateStartTime = SDL_GetTicks();
    bot->stepsRemaining = rand() % 100 + 50;
    bot->weapon = WEAPON_PISTOL;
    bot->ammo_left = 12;
    bot->full_ammo = 10000;
    bot->shootDelay = 1000;
}

void UpdateBots()
{
    Uint32 currentTime = SDL_GetTicks();

    for (int i = 0; i < settings.botCount; i++)
    {
        if (!bots[i].active || !bots[i].isAlive)
            continue;

        if (settings.botDifficulty >= 2)
        {
            if (player1.isAlive)
            {
                BotShootAtTarget(&bots[i], player1.x, player1.y, bulletTexture);
            }

            if (player2.isAlive)
            {
                BotShootAtTarget(&bots[i], player2.x, player2.y, bulletTexture);
            }

            for (int j = 0; j < settings.botCount; j++)
            {
                if (i != j && bots[j].active)
                {
                    BotShootAtTarget(&bots[i], bots[j].x, bots[j].y, bulletTexture);
                }
            }
        }

        Uint32 moveDelay = 200;
        int chancetoMove = rand() % 100;
        if (chancetoMove < 5)
        {
            moveDelay = 100;
        }

        if (currentTime - bots[i].lastMoveTime > moveDelay)
        {
            int newX = bots[i].x;
            int newY = bots[i].y;

            switch (bots[i].currentDirection)
            {
            case 0: // UP
                newY -= CHARACTER_SIZE / 4;
                break;
            case 1: // DOWN
                newY += CHARACTER_SIZE / 4;
                break;
            case 2: // LEFT
                newX -= CHARACTER_SIZE / 4;
                break;
            case 3: // RIGHT
                newX += CHARACTER_SIZE / 4;
                break;
            }

            if (IsWalkable(newX, newY, TEXTURE_SIZE, TEXTURE_SIZE))
            {
                bots[i].x = newX;
                bots[i].y = newY;
                bots[i].stepsRemaining--;
                bots[i].state = BOT_STATE_MOVING;
            }
            else
            {
                bots[i].stepsRemaining = 0;
                bots[i].state = BOT_STATE_PAUSED;
            }

            if (bots[i].x < 0)
                bots[i].x = 0;
            if (bots[i].x > SCREEN_WIDTH - CHARACTER_SIZE)
                bots[i].x = SCREEN_WIDTH - CHARACTER_SIZE;
            if (bots[i].y < 0)
                bots[i].y = 0;
            if (bots[i].y > SCREEN_HEIGHT - CHARACTER_SIZE)
                bots[i].y = SCREEN_HEIGHT - CHARACTER_SIZE;

            if (bots[i].stepsRemaining <= 0)
            {
                bots[i].currentDirection = rand() % NUM_BOT_DIRECTIONS;
                chancetoMove = rand() % 100;
                if (chancetoMove < 5)
                {
                    bots[i].stepsRemaining = rand() % 10 + 5;
                }
                else if (chancetoMove > 5 && chancetoMove < 15)
                {
                    bots[i].stepsRemaining = rand() % 5;
                }
                else
                {
                    bots[i].stepsRemaining = rand() % 20 + 10;
                }
            }

            bots[i].lastMoveTime = currentTime;
        }

        if (currentTime - bots[i].lastFrameTime >= bots[i].frameDelay)
        {
            int frameStart, frameEnd;
            if (bots[i].state == BOT_STATE_MOVING)
            {
                switch (bots[i].currentDirection)
                {
                case 0: // UP
                    frameStart = WALK_UP_START;
                    frameEnd = WALK_UP_END;
                    break;
                case 1: // DOWN
                    frameStart = WALK_DOWN_START;
                    frameEnd = WALK_DOWN_END;
                    break;
                case 2: // LEFT
                    frameStart = WALK_LEFT_START;
                    frameEnd = WALK_LEFT_END;
                    break;
                case 3: // RIGHT
                    frameStart = WALK_RIGHT_START;
                    frameEnd = WALK_RIGHT_END;
                    break;
                default:
                    frameStart = IDLE_RIGHT_START;
                    frameEnd = IDLE_RIGHT_END;
                    break;
                }
            }
            else
            {
                switch (bots[i].currentDirection)
                {
                case 0: // UP
                    frameStart = IDLE_UP_START;
                    frameEnd = IDLE_UP_END;
                    break;
                case 1: // DOWN
                    frameStart = IDLE_DOWN_START;
                    frameEnd = IDLE_DOWN_END;
                    break;
                case 2: // LEFT
                    frameStart = IDLE_LEFT_START;
                    frameEnd = IDLE_LEFT_END;
                    break;
                case 3: // RIGHT
                    frameStart = IDLE_RIGHT_START;
                    frameEnd = IDLE_RIGHT_END;
                    break;
                default:
                    frameStart = IDLE_RIGHT_START;
                    frameEnd = IDLE_RIGHT_END;
                    break;
                }
            }

            bots[i].currentFrame++;
            if (bots[i].currentFrame > frameEnd)
            {
                bots[i].currentFrame = frameStart;
            }

            bots[i].lastFrameTime = currentTime;
        }
    }
}

void ClearBotTextures(SDL_Texture *textures[])
{
    for (int i = 0; i < BOT_ANIMATION_FRAMES; i++)
    {
        if (textures[i])
        {
            SDL_DestroyTexture(textures[i]);
            textures[i] = NULL;
        }
    }
}

void ClearBots()
{
    for (int i = 0; i < MAX_BOTS; i++)
    {
        ClearBotTextures(bots[i].textures);
        bots[i].active = false;
    }
}
