#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "buttons.h"
#include "common.h"
#include "character.h"
#include "bullet.h"
#include "player.h"
#include "init.h"
#include "music.h"

extern Settings settings;
extern int playersCount;

extern SDL_Texture *loadingTextures[GIF_FRAME_RELOAD_COUNT];

extern Mix_Chunk *MainMenuFx;

extern Player player1;
extern Player player2;

extern void SaveGameResults(int playersCount);

void MainMenuEvents(SDL_Event *event, GameStatus *status)
{
    static int previousHoveredButton = -1;

    if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            int mouseX = event->button.x;
            int mouseY = event->button.y;

            for (int i = 0; i < 4; i++)
            {
                if (mouseX >= mainMenuButtonRects[i].x && mouseX <= mainMenuButtonRects[i].x + mainMenuButtonRects[i].w &&
                    mouseY >= mainMenuButtonRects[i].y && mouseY <= mainMenuButtonRects[i].y + mainMenuButtonRects[i].h)
                {
                    switch (i)
                    {
                    case 0:
                        *status = STATE_GAME_MENU;
                        break;
                    case 1:
                        *status = STATE_OPTIONS;
                        break;
                    case 2:
                        *status = STATE_RECORDS;
                        break;
                    case 3:
                        *status = STATE_QUIT;
                        break;
                    default:
                        break;
                    }
                    break;
                }
            }
        }
    }
    else if (event->type == SDL_MOUSEMOTION)
    {
        int mouseX = event->motion.x;
        int mouseY = event->motion.y;
        int buttonHovered = -1;

        for (int i = 0; i < 4; i++)
        {
            if (mouseX >= mainMenuButtonRects[i].x && mouseX <= mainMenuButtonRects[i].x + mainMenuButtonRects[i].w &&
                mouseY >= mainMenuButtonRects[i].y && mouseY <= mainMenuButtonRects[i].y + mainMenuButtonRects[i].h)
            {
                mainMenuSelectedButton = i;
                buttonHovered = i;
                break;
            }
        }

        if (buttonHovered != previousHoveredButton && buttonHovered != -1)
        {
            PlayMusicChunk(MainMenuFx);
            previousHoveredButton = buttonHovered;
        }
        else if (buttonHovered == -1)
            previousHoveredButton = -1;
    }
    else if (event->type == SDL_KEYDOWN)
    {
        switch (event->key.keysym.sym)
        {
        case SDLK_DOWN:
            mainMenuSelectedButton = (mainMenuSelectedButton + 1) % 4;
            PlayMusicChunk(MainMenuFx);
            break;
        case SDLK_UP:
            mainMenuSelectedButton = (mainMenuSelectedButton + 3) % 4;
            PlayMusicChunk(MainMenuFx);
            break;
        case SDLK_RETURN:
        case SDLK_SPACE:
            switch (mainMenuSelectedButton)
            {
            case 0: // Start
                *status = STATE_GAME_MENU;
                break;
            case 1: // Options
                *status = STATE_OPTIONS;
                break;
            case 2: // Records
                *status = STATE_RECORDS;
                break;
            case 3: // Quit
                *status = STATE_QUIT;
                break;
            default:
                break;
            }
            PlayMusicChunk(MainMenuFx);
            break;
        default:
            break;
        }
    }
}

void GameMenuEvents(SDL_Event *event, GameStatus *status)
{
    static int previousHoveredButton = -1;

    if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            int mouseX = event->button.x;
            int mouseY = event->button.y;

            for (int i = 0; i < 5; i++)
            {
                if (mouseX >= gameStartButtonRects[i].x && mouseX <= gameStartButtonRects[i].x + gameStartButtonRects[i].w &&
                    mouseY >= gameStartButtonRects[i].y && mouseY <= gameStartButtonRects[i].y + gameStartButtonRects[i].h)
                {
                    switch (i)
                    {
                    case 0: // AI: 1-10
                        if (settings.botCount < 10)
                            settings.botCount += 1;
                        break;
                    case 1: // Difficulty: Easy-Medium-Hard
                        settings.botDifficulty = (settings.botDifficulty % 3) + 1;
                        break; // Cicle 1-2-3-1
                    case 2:    // Players: 1-2
                        playersCount = (playersCount % 2) + 1;
                        break;
                    case 3: // Start
                        *status = STATE_PLAY;
                        break;
                    case 4: // Back
                        *status = STATE_MAIN_MENU;
                        break;
                    default:
                        break;
                    }
                    break;
                }
            }
        }
    }
    else if (event->type == SDL_MOUSEMOTION)
    {
        int mouseX = event->motion.x;
        int mouseY = event->motion.y;
        int buttonHovered = -1;

        for (int i = 0; i < 5; i++)
        {
            if (mouseX >= gameStartButtonRects[i].x && mouseX <= gameStartButtonRects[i].x + gameStartButtonRects[i].w &&
                mouseY >= gameStartButtonRects[i].y && mouseY <= gameStartButtonRects[i].y + gameStartButtonRects[i].h)
            {
                gameStartSelectedButton = i;
                buttonHovered = i;
                break;
            }
        }

        if (buttonHovered != previousHoveredButton && buttonHovered != -1)
        {
            PlayMusicChunk(MainMenuFx);
            previousHoveredButton = buttonHovered;
        }
        else if (buttonHovered == -1)
            previousHoveredButton = -1;
    }
    else if (event->type == SDL_KEYDOWN)
    {
        switch (event->key.keysym.sym)
        {
        case SDLK_DOWN:
            gameStartSelectedButton = (gameStartSelectedButton + 1) % 5;
            PlayMusicChunk(MainMenuFx);
            break;
        case SDLK_UP:
            gameStartSelectedButton = (gameStartSelectedButton - 1 + 5) % 5;
            PlayMusicChunk(MainMenuFx);
            break;
        case SDLK_RETURN:
        case SDLK_SPACE:
            switch (gameStartSelectedButton)
            {
            case 0: // AI: 1-10
                if (settings.botCount < 10)
                    settings.botCount += 1;
                break;
            case 1: // Difficulty: Easy-Medium-Hard
                settings.botDifficulty = (settings.botDifficulty % 3) + 1;
                break; // Cicle 1-2-3-1
            case 2:    // Players: 1-2
                playersCount = (playersCount % 2) + 1;
                break;
            case 3: // Start
                *status = STATE_PLAY;
                break;
            case 4: // Back
                *status = STATE_MAIN_MENU;
                break;
            default:
                break;
            }
            PlayMusicChunk(MainMenuFx);
            break;
        case SDLK_LEFT:
            if (gameStartSelectedButton == 0 && settings.botCount > 1)
            {
                settings.botCount -= 1;
            }
            else if (gameStartSelectedButton == 1)
            {
                settings.botDifficulty = (settings.botDifficulty == 1) ? 3 : settings.botDifficulty - 1;
            }
            else if (gameStartSelectedButton == 2)
            {
                playersCount = (playersCount == 1) ? 2 : 1;
            }
            PlayMusicChunk(MainMenuFx);
            break;
        case SDLK_RIGHT:
            if (gameStartSelectedButton == 0 && settings.botCount < 10)
            {
                settings.botCount += 1;
            }
            else if (gameStartSelectedButton == 1)
            {
                settings.botDifficulty = (settings.botDifficulty == 3) ? 1 : settings.botDifficulty + 1;
            }
            else if (gameStartSelectedButton == 2)
            {
                playersCount = (playersCount == 2) ? 1 : 2;
            }
            PlayMusicChunk(MainMenuFx);
            break;
        default:
            break;
        }
    }
}

void GameStartEvents(SDL_Event *event, GameStatus *status, SDL_Renderer *renderer, TTF_Font *font)
{
    *status = STATE_PLAY;

    // Save weapons ammo
    if (player1.weapon == WEAPON_RIFLE)
    {
        player1.rifleAmmo = player1.ammo_left;
        player1.rifleFullAmmo = player1.full_ammo;
    }
    else if (player1.weapon == WEAPON_SHOTGUN)
    {
        player1.shotgunAmmo = player1.ammo_left;
        player1.shotgunFullAmmo = player1.full_ammo;
    }
    else
    {
        player1.pistolAmmo = player1.ammo_left;
        player1.pistolFullAmmo = player1.full_ammo;
    }

    // Save weapons ammo
    if (player2.weapon == WEAPON_RIFLE)
    {
        player2.rifleAmmo = player2.ammo_left;
        player2.rifleFullAmmo = player2.full_ammo;
    }
    else if (player1.weapon == WEAPON_SHOTGUN)
    {
        player2.shotgunAmmo = player2.ammo_left;
        player2.shotgunFullAmmo = player2.full_ammo;
    }
    else
    {
        player2.pistolAmmo = player2.ammo_left;
        player2.pistolFullAmmo = player2.full_ammo;
    }

    SDL_Rect weaponReloadRect;
    weaponReloadRect.w = 24;
    weaponReloadRect.h = 24;

    // Weapon actions
    if (event->type == SDL_KEYDOWN)
    {
        switch (event->key.keysym.sym)
        {
        case SDLK_1:
            player1.weapon = WEAPON_PISTOL;
            player1.ammo_left = player1.pistolAmmo;
            player1.full_ammo = player1.pistolFullAmmo;
            break;

        case SDLK_2:
            if (player1.hasWeapon[WEAPON_RIFLE])
            {
                player1.weapon = WEAPON_RIFLE;
                player1.ammo_left = player1.rifleAmmo;
                player1.full_ammo = player1.rifleFullAmmo;
            }
            break;

        case SDLK_3:
            if (player1.hasWeapon[WEAPON_SHOTGUN])
            {
                player1.weapon = WEAPON_SHOTGUN;
                player1.ammo_left = player1.shotgunAmmo;
                player1.full_ammo = player1.shotgunFullAmmo;
            }
            break;

        case SDLK_r:
            if (player1.weapon == WEAPON_PISTOL && player1.ammo_left != 12 && player1.full_ammo > 0)
                player1.reloadStartTime = SDL_GetTicks();
            else if (player1.weapon == WEAPON_RIFLE && player1.ammo_left != 30 && player1.full_ammo > 0)
                player1.reloadStartTime = SDL_GetTicks();
            else if (player1.weapon == WEAPON_SHOTGUN && player1.ammo_left != 6 && player1.full_ammo > 0)
                player1.reloadStartTime = SDL_GetTicks();
            break;

        case SDLK_0:
            player2.weapon = WEAPON_PISTOL;
            player2.ammo_left = player2.pistolAmmo;
            player2.full_ammo = player2.pistolFullAmmo;
            break;

        case SDLK_9:
            if (player2.hasWeapon[WEAPON_RIFLE])
            {
                player2.weapon = WEAPON_RIFLE;
                player2.ammo_left = player2.rifleAmmo;
                player2.full_ammo = player2.rifleFullAmmo;
            }
            break;

        case SDLK_8:
            if (player2.hasWeapon[WEAPON_SHOTGUN])
            {
                player2.weapon = WEAPON_SHOTGUN;
                player2.ammo_left = player2.shotgunAmmo;
                player2.full_ammo = player2.shotgunFullAmmo;
            }
            break;

        case SDLK_p:
            if (player2.weapon == WEAPON_PISTOL && player2.ammo_left != 12 && player2.full_ammo > 0)
                player2.reloadStartTime = SDL_GetTicks();
            else if (player2.weapon == WEAPON_RIFLE && player2.ammo_left != 30 && player2.full_ammo > 0)
                player2.reloadStartTime = SDL_GetTicks();
            else if (player2.weapon == WEAPON_SHOTGUN && player2.ammo_left != 6 && player2.full_ammo > 0)
                player2.reloadStartTime = SDL_GetTicks();
            break;

        default:
            break;
        }
    }

    // Reload weapon for player 1
    if (player1.reloadStartTime != 0)
    {
        Uint32 reloadDuration = 1300;
        if (SDL_GetTicks() - player1.reloadStartTime >= reloadDuration)
        {
            switch (player1.weapon)
            {
            case WEAPON_PISTOL:
                if (player1.full_ammo >= 12 - player1.ammo_left)
                {
                    player1.full_ammo -= (12 - player1.ammo_left);
                    player1.ammo_left = 12;
                }
                else
                {
                    player1.ammo_left += player1.full_ammo;
                    player1.full_ammo = 0;
                }
                break;
            case WEAPON_RIFLE:
                if (player1.full_ammo >= 30 - player1.ammo_left)
                {
                    player1.full_ammo -= (30 - player1.ammo_left);
                    player1.ammo_left = 30;
                }
                else
                {
                    player1.ammo_left += player1.full_ammo;
                    player1.full_ammo = 0;
                }
                break;
            case WEAPON_SHOTGUN:
                if (player1.full_ammo >= 6 - player1.ammo_left)
                {
                    player1.full_ammo -= (6 - player1.ammo_left);
                    player1.ammo_left = 6;
                }
                else
                {
                    player1.ammo_left += player1.full_ammo;
                    player1.full_ammo = 0;
                }
                break;
            }

            player1.reloadStartTime = 0;
        }
        else
        {
            if (player1.isAlive)
            {
                player1.fireRate = -1;
                // Display reloading text
                const char *reloadingText[] = {"Reloading .", "Reloading ..", "Reloading ..."};
                Uint32 elapsedTime = SDL_GetTicks() - player1.reloadStartTime;
                int dotIndex = (elapsedTime / 300) % 3;

                SDL_Color textColor = {255, 255, 255, 255}; // White

                SDL_Surface *surface = TTF_RenderText_Solid(font, reloadingText[dotIndex], textColor);
                SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_Rect textRect = {
                    player1.x + (CHARACTER_SIZE - surface->w) / 2,
                    player1.y - 20,
                    surface->w,
                    surface->h};
                SDL_RenderCopy(renderer, texture, NULL, &textRect);
                SDL_FreeSurface(surface);
                SDL_DestroyTexture(texture);

                if (player1.weapon == WEAPON_PISTOL)
                {
                    player1.fireRate = 500;
                }
                else if (player1.weapon == WEAPON_RIFLE)
                {
                    player1.fireRate = 200;
                }
                else if (player1.weapon == WEAPON_SHOTGUN)
                {
                    player1.fireRate = 1000;
                }
            }
        }
    }

    // Reload weapon for player 2
    if (player2.reloadStartTime != 0)
    {
        Uint32 reloadDuration = 1300;
        if (SDL_GetTicks() - player2.reloadStartTime >= reloadDuration)
        {
            switch (player2.weapon)
            {
            case WEAPON_PISTOL:
                if (player2.full_ammo >= 12 - player2.ammo_left)
                {
                    player2.full_ammo -= (12 - player2.ammo_left);
                    player2.ammo_left = 12;
                }
                else
                {
                    player2.ammo_left += player2.full_ammo;
                    player2.full_ammo = 0;
                }
                break;
            case WEAPON_RIFLE:
                if (player2.full_ammo >= 30 - player2.ammo_left)
                {
                    player2.full_ammo -= (30 - player2.ammo_left);
                    player2.ammo_left = 30;
                }
                else
                {
                    player2.ammo_left += player2.full_ammo;
                    player2.full_ammo = 0;
                }
                break;
            case WEAPON_SHOTGUN:
                if (player2.full_ammo >= 6 - player2.ammo_left)
                {
                    player2.full_ammo -= (6 - player2.ammo_left);
                    player2.ammo_left = 6;
                }
                else
                {
                    player2.ammo_left += player2.full_ammo;
                    player2.full_ammo = 0;
                }
                break;
            }

            player2.reloadStartTime = 0;
        }
        else
        {
            if (player2.isAlive)
            {
                player2.fireRate = -1;
                // Display reloading text
                const char *reloadingText[] = {"Reloading .", "Reloading ..", "Reloading ..."};
                Uint32 elapsedTime = SDL_GetTicks() - player2.reloadStartTime;
                int dotIndex = (elapsedTime / 300) % 3;

                SDL_Color textColor = {255, 255, 255, 255}; // White

                SDL_Surface *surface = TTF_RenderText_Solid(font, reloadingText[dotIndex], textColor);
                SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_Rect textRect = {
                    player2.x + (CHARACTER_SIZE - surface->w) / 2,
                    player2.y - 20,
                    surface->w,
                    surface->h};
                SDL_RenderCopy(renderer, texture, NULL, &textRect);
                SDL_FreeSurface(surface);
                SDL_DestroyTexture(texture);

                if (player2.weapon == WEAPON_PISTOL)
                {
                    player2.fireRate = 500;
                }
                else if (player2.weapon == WEAPON_RIFLE)
                {
                    player2.fireRate = 200;
                }
                else if (player2.weapon == WEAPON_SHOTGUN)
                {
                    player2.fireRate = 1000;
                }
            }
        }
    }
}

void GameOverEvents(SDL_Event *event, GameStatus *status)
{
    if (event->type == SDL_KEYDOWN)
    {
        switch (event->key.keysym.sym)
        {
        case SDLK_RETURN:
            PlayMusicChunk(MainMenuFx);
            SaveGameResults(playersCount);
            break;
        case SDLK_ESCAPE:
            PlayMusicChunk(MainMenuFx);
            *status = STATE_MAIN_MENU;
            break;
        default:
            break;
        }
    }
}