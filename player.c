#include "player.h"
#include "common.h"
#include "character.h"
#include "collision.h"
#include "bullet.h"
#include <time.h>

Player player1;
Player player2;

extern int playersCount;
extern int levelMap[MAP_HEIGHT][MAP_WIDTH];

void PlayerShoot(Player *player, SDL_Texture *bulletTexture)
{
    if (player->ammo_left > 0)
    {
        int dx = 0, dy = 0;
        switch (player->direction)
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
        player->ammo_left -= 1;
        if (player->id == 1)
            CreateBullet(player->x + CHARACTER_SIZE / 2, player->y + CHARACTER_SIZE / 2, dx * 5, dy * 5, bulletTexture, BULLET_PLAYER, 15);
        else
            CreateBullet(player->x + CHARACTER_SIZE / 2, player->y + CHARACTER_SIZE / 2, dx * 5, dy * 5, bulletTexture, BULLET_PLAYER2, 16);
    }
}

void InitPlayer(Player *player, int startX, int startY, int id)
{
    player->id = id;
    player->x = startX;
    player->y = startY;
    player->speed = 2;
    player->velX = 0;
    player->velY = 0;
    player->direction = DIRECTION_RIGHT;
    player->status = STATUS_IDLE;
    player->healthPoint = 100;
    player->armor = 100;
    player->attackPoint = 100;
    player->ammo_left = 12;
    player->full_ammo = 36;
    player->fireRate = 1000;
    player->kills = 0;
    player->timeRecord = 0;
    player->weapon = WEAPON_PISTOL;
    player->hasWeapon[WEAPON_PISTOL] = true;
    player->hasWeapon[WEAPON_RIFLE] = false;
    player->hasWeapon[WEAPON_SHOTGUN] = false;
    player->pistolAmmo = 12;
    player->pistolFullAmmo = 36;
    player->rifleFullAmmo = 0;
    player->rifleAmmo = 0;
    player->shotgunFullAmmo = 0;
    player->shotgunAmmo = 0;
    player->isAlive = true;
    if (player->id == 1)
        strcpy(player->name, "Player 1");
    else
        strcpy(player->name, "Player 2");
}

void RespawnPlayer(Player *player)
{
    int Xrand, Yrand;
    Xrand = 1 + rand() % (MAP_WIDTH - 2);
    Yrand = 1 + rand() % (MAP_HEIGHT - 2);

    while (levelMap[Yrand][Xrand] != 1)
    {
        Xrand = 1 + rand() % (MAP_WIDTH - 2);
        Yrand = 1 + rand() % (MAP_HEIGHT - 2);
    }

    player->x = Xrand * TEXTURE_SIZE;
    player->y = Yrand * TEXTURE_SIZE;
    player->healthPoint = 100;
    player->armor = 100;
    player->weapon = WEAPON_PISTOL;
    player->ammo_left = 12;
    player->full_ammo = 36;
    player->fireRate = 1000;
    player->isAlive = true;
    player->deathTime = 0;
    player->deathCount++;
    player->rifleAmmo = 0;
    player->rifleFullAmmo = 0;
    player->shotgunAmmo = 0;
    player->shotgunFullAmmo = 0;
    player->hasWeapon[WEAPON_PISTOL] = true;
    player->hasWeapon[WEAPON_RIFLE] = false;
    player->hasWeapon[WEAPON_SHOTGUN] = false;
}

void UpdatePlayer(Player *player, const Uint8 *keystate)
{
    if (!player->isAlive)
        return;

    int newVelX = 0;
    int newVelY = 0;

    if (playersCount == 1 || player->id == 1)
    {
        if (keystate[SDL_SCANCODE_W])
        {
            newVelY = -player->speed;
            player->direction = DIRECTION_UP;
            player->status = STATUS_WALK;
        }
        if (keystate[SDL_SCANCODE_S])
        {
            newVelY = player->speed;
            player->direction = DIRECTION_DOWN;
            player->status = STATUS_WALK;
        }
        if (keystate[SDL_SCANCODE_A])
        {
            newVelX = -player->speed;
            player->direction = DIRECTION_LEFT;
            player->status = STATUS_WALK;
        }
        if (keystate[SDL_SCANCODE_D])
        {
            newVelX = player->speed;
            player->direction = DIRECTION_RIGHT;
            player->status = STATUS_WALK;
        }
    }
    else if (playersCount == 2 && player->id == 2)
    {
        if (keystate[SDL_SCANCODE_UP])
        {
            newVelY = -player->speed;
            player->direction = DIRECTION_UP;
            player->status = STATUS_WALK;
        }
        if (keystate[SDL_SCANCODE_DOWN])
        {
            newVelY = player->speed;
            player->direction = DIRECTION_DOWN;
            player->status = STATUS_WALK;
        }
        if (keystate[SDL_SCANCODE_LEFT])
        {
            newVelX = -player->speed;
            player->direction = DIRECTION_LEFT;
            player->status = STATUS_WALK;
        }
        if (keystate[SDL_SCANCODE_RIGHT])
        {
            newVelX = player->speed;
            player->direction = DIRECTION_RIGHT;
            player->status = STATUS_WALK;
        }
    }

    if (playersCount == 1 || player->id == 1)
    {
        if (keystate[SDL_SCANCODE_LSHIFT])
            player->speed = 4;
        else
            player->speed = 2;
    }
    else if (playersCount == 2 && player->id == 2)
    {
        if (keystate[SDL_SCANCODE_RSHIFT])
            player->speed = 4;
        else
            player->speed = 2;
    }

    if (!newVelX && !newVelY)
        player->status = STATUS_IDLE;

    // New position
    int newX = player->x + newVelX;
    int newY = player->y + newVelY;

    if (IsWalkable(newX, newY, TEXTURE_SIZE, TEXTURE_SIZE))
    {
        player->x = newX;
        player->y = newY;
    }

    if (player->x < 0)
        player->x = 0;
    if (player->x > SCREEN_WIDTH - TEXTURE_SIZE)
        player->x = SCREEN_WIDTH - TEXTURE_SIZE;
    if (player->y < 0)
        player->y = 0;
    if (player->y > SCREEN_HEIGHT - TEXTURE_SIZE)
        player->y = SCREEN_HEIGHT - TEXTURE_SIZE;

    if (player->reloadStartTime != 0)
    {
        Uint32 elapsedTime = SDL_GetTicks() - player->reloadStartTime;
        Uint32 reloadDuration = 1300;

        if (elapsedTime >= reloadDuration)
        {
            // Complete reloading
            switch (player->weapon)
            {
            case WEAPON_PISTOL:
            {
                int ammoNeeded = 12 - player->ammo_left;
                if (player->full_ammo >= ammoNeeded)
                {
                    player->ammo_left += ammoNeeded;
                    player->full_ammo -= ammoNeeded;
                }
                else
                {
                    player->ammo_left += player->full_ammo;
                    player->full_ammo = 0;
                }
            }
            break;
            case WEAPON_RIFLE:
            {
                int ammoNeeded = 30 - player->ammo_left;
                if (player->full_ammo >= ammoNeeded)
                {
                    player->ammo_left += ammoNeeded;
                    player->full_ammo -= ammoNeeded;
                }
                else
                {
                    player->ammo_left += player->full_ammo;
                    player->full_ammo = 0;
                }
            }
            break;
            case WEAPON_SHOTGUN:
            {
                int ammoNeeded = 6 - player->ammo_left;
                if (player->full_ammo >= ammoNeeded)
                {
                    player->ammo_left += ammoNeeded;
                    player->full_ammo -= ammoNeeded;
                }
                else
                {
                    player->ammo_left += player->full_ammo;
                    player->full_ammo = 0;
                }
            }
            break;
            }
            player->reloadStartTime = 0;
        }
    }
}

void RenderPlayer(SDL_Renderer *renderer, Player *player)
{
    int textureIndex;
    SDL_Texture **textures;
    if (player->id == 1)
        textures = characterTextures1;
    else
        textures = characterTextures2;

    if (player->status == STATUS_IDLE)
    {
        switch (player->direction)
        {
        case DIRECTION_LEFT:
            textureIndex = IDLE_LEFT_START + ((SDL_GetTicks() / 200) % (IDLE_LEFT_END - IDLE_LEFT_START));
            break;
        case DIRECTION_RIGHT:
            textureIndex = IDLE_RIGHT_START + ((SDL_GetTicks() / 200) % (IDLE_RIGHT_END - IDLE_RIGHT_START));
            break;
        case DIRECTION_UP:
            textureIndex = IDLE_UP_START + ((SDL_GetTicks() / 200) % (IDLE_UP_END - IDLE_UP_START));
            break;
        case DIRECTION_DOWN:
            textureIndex = IDLE_DOWN_START + ((SDL_GetTicks() / 200) % (IDLE_DOWN_END - IDLE_DOWN_START));
            break;
        default:
            textureIndex = IDLE_RIGHT_START + ((SDL_GetTicks() / 200) % (IDLE_RIGHT_END - IDLE_RIGHT_START));
            break;
        }
    }
    else if (player->status == STATUS_WALK)
    {
        switch (player->direction)
        {
        case DIRECTION_LEFT:
            textureIndex = WALK_LEFT_START + ((SDL_GetTicks() / 100) % (WALK_LEFT_END - WALK_LEFT_START));
            break;
        case DIRECTION_RIGHT:
            textureIndex = WALK_RIGHT_START + ((SDL_GetTicks() / 100) % (WALK_RIGHT_END - WALK_RIGHT_START));
            break;
        case DIRECTION_UP:
            textureIndex = WALK_UP_START + ((SDL_GetTicks() / 100) % (WALK_UP_END - WALK_UP_START));
            break;
        case DIRECTION_DOWN:
            textureIndex = WALK_DOWN_START + ((SDL_GetTicks() / 100) % (WALK_DOWN_END - WALK_DOWN_START));
            break;
        default:
            textureIndex = WALK_RIGHT_START + ((SDL_GetTicks() / 100) % (WALK_RIGHT_END - WALK_RIGHT_START));
            break;
        }
    }

    if (textures[textureIndex])
    {
        SDL_Rect destRect = {
            player->x,
            player->y,
            TEXTURE_SIZE - 10,
            TEXTURE_SIZE - 10};

        SDL_Rect destRectDead = {0, 0, 0, 0};

        if (player->isAlive)
            SDL_RenderCopy(renderer, textures[textureIndex], NULL, &destRect);
        else
            SDL_RenderCopy(renderer, textures[textureIndex], NULL, &destRectDead);
    }
}
