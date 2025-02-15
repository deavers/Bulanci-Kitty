#include "weapons.h"
#include "player.h"
#include "common.h"

Weapon weapons[MAX_WEAPONS];
extern int levelMap[MAP_HEIGHT][MAP_WIDTH];
extern int weaponsSpawned;

void AddWeapon(WeaponType type, int x, int y)
{
    for (int i = 0; i < MAX_WEAPONS; i++)
    {
        if (!weapons[i].active)
        {
            weapons[i].type = type;
            weapons[i].rect = (SDL_Rect){x, y, 100, 50};
            weapons[i].active = true;
            weapons[i].spawnTime = SDL_GetTicks();
            break;
        }
    }
}

void RenderWeapons(SDL_Renderer *renderer, SDL_Texture *weaponTextures[])
{
    for (int i = 0; i < MAX_WEAPONS; i++)
    {
        if (weapons[i].active)
        {
            SDL_RenderCopy(renderer, weaponTextures[weapons[i].type], NULL, &weapons[i].rect);
        }
    }
}

void HandleWeaponPickup(Player *player)
{
    SDL_Rect playerRect = {player->x, player->y, TEXTURE_SIZE, TEXTURE_SIZE};

    for (int i = 0; i < MAX_WEAPONS; i++)
    {
        if (weapons[i].active && SDL_HasIntersection(&playerRect, &weapons[i].rect))
        {
            // Save ammo
            int *currentAmmo, *currentFullAmmo;
            switch (player->weapon)
            {
            case WEAPON_PISTOL:
                currentAmmo = &player->pistolAmmo;
                currentFullAmmo = &player->pistolFullAmmo;
                break;
            case WEAPON_RIFLE:
                currentAmmo = &player->rifleAmmo;
                currentFullAmmo = &player->rifleFullAmmo;
                break;
            case WEAPON_SHOTGUN:
                currentAmmo = &player->shotgunAmmo;
                currentFullAmmo = &player->shotgunFullAmmo;
                break;
            }
            *currentAmmo = player->ammo_left;
            *currentFullAmmo = player->full_ammo;

            weapons[i].active = false;
            player->weapon = weapons[i].type;

            switch (player->weapon)
            {
            case WEAPON_PISTOL:
                player->hasWeapon[WEAPON_PISTOL] = true;
                player->ammo_left = 12;
                player->full_ammo = 36;
                player->fireRate = 500;
                break;

            case WEAPON_RIFLE:
                player->hasWeapon[WEAPON_RIFLE] = true;
                player->ammo_left = 30;
                player->full_ammo = 90;
                player->fireRate = 200;
                break;

            case WEAPON_SHOTGUN:
                player->hasWeapon[WEAPON_SHOTGUN] = true;
                player->ammo_left = 6;
                player->full_ammo = 24;
                player->fireRate = 1000;
                break;
            }
        }
    }
}

void UpdateWeapons()
{
    Uint32 currentTime = SDL_GetTicks();
    for (int i = 0; i < MAX_WEAPONS; i++)
    {
        if (weapons[i].active && currentTime - weapons[i].spawnTime > 20000)
        {
            weapons[i].active = false;
            weaponsSpawned -= 1;
            // printf("%d, %d despawned\n", weapons[i].rect.x, weapons[i].rect.y);
        }
    }
}

void WeaponMainMenu(SDL_Renderer *renderer)
{
    int weaponSpawnChance = rand() % 4000;
    if (weaponSpawnChance < 5)
    {
        int weaponX = 1 + rand() % (MAP_WIDTH - 2);
        int weaponY = 1 + rand() % (MAP_HEIGHT - 2);

        while (levelMap[weaponY][weaponX] != 1)
        {
            weaponX = 1 + rand() % (MAP_WIDTH - 2);
            weaponY = 1 + rand() % (MAP_HEIGHT - 2);
        }

        int weaponType = rand() % 2 + 1;

        AddWeapon(weaponType, weaponX * TEXTURE_SIZE, weaponY * TEXTURE_SIZE);
        weaponsSpawned += 1;
        // printf("Weapon spawned at %d, %d\n", weaponX, weaponY);
    }
    RenderWeapons(renderer, weaponTextures);
}

void ClearWeapons()
{
    for (int i = 0; i < MAX_WEAPONS; i++)
    {
        if (weapons[i].active)
        {
            weapons[i].active = false;
        }
    }
}