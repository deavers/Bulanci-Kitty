#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include "common.h"

typedef struct Player
{
    int id;
    int x;
    int y;
    int speed;
    int velX;
    int velY;
    Direction direction;
    Status status;
    int healthPoint;
    int armor;
    int attackPoint;
    int ammo_left;
    int full_ammo;
    int fireRate;
    int kills;
    WeaponType weapon;
    char name[50];
    int timeRecord;
    bool isAlive;
    int deathTime;
    int deathCount;
    SDL_Rect rect;
    bool hasWeapon[WEAPON_COUNT];
    int pistolFullAmmo;
    int pistolAmmo;
    int rifleFullAmmo;
    int rifleAmmo;
    int shotgunFullAmmo;
    int shotgunAmmo;
    int reloadStartTime;
} Player;

extern Player player1;
extern Player player2;

void PlayerShoot(Player *player, SDL_Texture *bulletTexture);
void InitPlayer(Player *player, int startX, int startY, int id);
void RespawnPlayer(Player *player);
void UpdatePlayer(Player *player, const Uint8 *keystate);
void RenderPlayer(SDL_Renderer *renderer, Player *player);

#endif