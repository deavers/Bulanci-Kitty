#ifndef WEAPONS_H
#define WEAPONS_H

#include <SDL2/SDL.h>
#include "player.h"
#include "weapons.h"

#define MAX_WEAPONS 20

extern Weapon weapons[MAX_WEAPONS];
extern SDL_Texture *weaponTextures[WEAPON_COUNT];

void AddWeapon(WeaponType type, int x, int y);
void RenderWeapons(SDL_Renderer *renderer, SDL_Texture *weaponTextures[]);
void HandleWeaponPickup(Player *player);
void UpdateWeapons();
void WeaponMainMenu(SDL_Renderer *renderer);
void ClearWeapons();

#endif