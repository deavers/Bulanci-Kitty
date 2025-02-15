#ifndef BOTS_H
#define BOTS_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "init.h"
#include "player.h"

extern SDL_Texture *bulletTexture;

typedef enum
{
    BOT_STATE_MOVING,
    BOT_STATE_PAUSED
} BotState;

typedef struct Bot
{
    bool active;
    bool isAlive;
    int id;
    int x, y;
    int velX, velY;
    int colorIndex;
    int state;
    int kills;
    Uint32 stateStartTime;
    int stepsRemaining;
    int currentDirection; // 0: UP, 1: DOWN, 2: LEFT, 3: RIGHT
    int currentFrame;
    Uint32 lastFrameTime;
    Uint32 frameDelay;
    SDL_Texture *textures[NUM_CHARACTER_TEXTURES];
    Uint32 lastShootTime;
    Uint32 lastMoveTime;
    Uint32 shootDelay;
    int fireRate;
    char name[MAX_BOT_NAMES];
    int ammo_left;
    int full_ammo;
    WeaponType weapon;
} Bot;

void BotMove(Bot *bot);
void BotShootAtTarget(Bot *bot, float targetX, float targetY, SDL_Texture *bulletTexture);
void ChangeBotDirection(Bot *bot);
bool CheckCollisionWithWall(Bot *bot, int dx, int dy);
bool IsClearPath(Bot *bot, float targetX, float targetY);
int LoadBotTextures(SDL_Renderer *renderer, SDL_Texture *textures[], const char *color);
void InitBots(SDL_Renderer *renderer, const char *botColors[], int numColors);
void RenderBots(SDL_Renderer *renderer);
void RespawnBot(Bot *bot);
void UpdateBots();
void ClearBotTextures(SDL_Texture *textures[]);
void ClearBots();

#endif
