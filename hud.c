#include "hud.h"
#include "collision.h"
#include "character.h"
#include "init.h"
#include "bots.h"

extern Bot bots[MAX_BOTS];
extern Settings settings;
extern int playersCount;
extern SDL_Texture *weaponTextures[WEAPON_COUNT];
extern SDL_Texture *weaponTexturesLocked[WEAPON_COUNT];
extern SDL_Texture *weaponTexturesActive[WEAPON_COUNT];
extern SDL_Texture *weaponHUD;
extern SDL_Texture *deathTextures[NUM_CHARACTER_TEXTURES];

Leaderboard leaderboard[MAX_BOTS + 1];
Leaderboard temp;

void DrawHUDPlayer(SDL_Renderer *renderer, Player *player1, Player *player2)
{
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color blue = {0, 0, 255, 85};

    Player *players[] = {player1, player2};

    for (int i = 0; i < playersCount; i++)
    {
        Player *player = players[i];

        if (player->isAlive)
        {
            // Отрисовка текста "P1" или "P2"
            TTF_Font *font = LoadFont("Assets/Font/Lilita_One_Regular.ttf", 12);
            if (!font)
            {
                printf("Error loading font in DrawHUDPlayer!\n");
                return;
            }
            SDL_Surface *surfaceP = TTF_RenderText_Solid(font, player->id == 1 ? "P1" : "P2", white);
            if (!surfaceP)
            {
                printf("Error rendering text surface in DrawHUDPlayer!\n");
                TTF_CloseFont(font);
                return;
            }

            SDL_Texture *textureP = SDL_CreateTextureFromSurface(renderer, surfaceP);
            if (!textureP)
            {
                printf("Error creating texture from surface in DrawHUDPlayer!\n");
                SDL_FreeSurface(surfaceP);
                TTF_CloseFont(font);
                return;
            }

            SDL_Rect textRectP = {player->x + (TEXTURE_SIZE / 2) - 5, player->y + 15, surfaceP->w, surfaceP->h};
            SDL_RenderCopy(renderer, textureP, NULL, &textRectP);
            SDL_FreeSurface(surfaceP);
            SDL_DestroyTexture(textureP);
            TTF_CloseFont(font);

            if (player->armor > 0)
            {
                DrawCircle(renderer, player->x + (TEXTURE_SIZE - 10) / 2, player->y + (TEXTURE_SIZE - 10) / 2, (TEXTURE_SIZE - 10) / 2 + 10, blue);
            }
        }
        else
        {
            if (playersCount == 1)
            {
                SDL_Rect rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
                SDL_SetRenderDrawColor(renderer, 128, 128, 128, 100);
                SDL_RenderFillRect(renderer, &rect);

                TTF_Font *font = LoadFont("Assets/Font/Lilita_One_Regular.ttf", 40);
                if (!font)
                {
                    printf("Error loading font in DrawHUDPlayer!\n");
                    return;
                }

                Uint32 elapsedSeconds = (SDL_GetTicks() - player->deathTime) / 1000;
                Uint32 countdown = 3 - elapsedSeconds;
                if (countdown > 0)
                {
                    char countdownText[10];
                    sprintf(countdownText, "0:0%d", countdown);
                    SDL_Surface *surfaceCountdown = TTF_RenderText_Solid(font, countdownText, white);
                    if (!surfaceCountdown)
                    {
                        TTF_CloseFont(font);
                        return;
                    }
                    SDL_Texture *textureCountdown = SDL_CreateTextureFromSurface(renderer, surfaceCountdown);
                    if (!textureCountdown)
                    {
                        SDL_FreeSurface(surfaceCountdown);
                        TTF_CloseFont(font);
                        return;
                    }
                    SDL_Rect textRectCountdown = {
                        (SCREEN_WIDTH - surfaceCountdown->w) / 2,
                        (SCREEN_HEIGHT - surfaceCountdown->h) / 2,
                        surfaceCountdown->w,
                        surfaceCountdown->h};
                    SDL_RenderCopy(renderer, textureCountdown, NULL, &textRectCountdown);
                    SDL_FreeSurface(surfaceCountdown);
                    SDL_DestroyTexture(textureCountdown);
                }
            }
        }
    }
}

void DrawDeathAnimation(SDL_Renderer *renderer, Player *player, SDL_Texture *deathTextures[])
{
    int currentFrame = (SDL_GetTicks() / 100) % 4; // 4 frames for each direction

    SDL_Texture *deathTexture = NULL;
    SDL_Rect deathRect = {player->x, player->y, TEXTURE_SIZE - 10, TEXTURE_SIZE - 10};

    if (player->direction == DIRECTION_RIGHT)
    {
        deathTexture = deathTextures[67 + currentFrame];
    }
    else if (player->direction == DIRECTION_LEFT)
    {
        deathTexture = deathTextures[71 + currentFrame];
    }
    else if (player->direction == DIRECTION_UP)
    {
        deathTexture = deathTextures[75 + currentFrame];
    }
    else if (player->direction == DIRECTION_DOWN)
    {
        deathTexture = deathTextures[83 + currentFrame];
    }

    if (deathTexture)
    {
        SDL_RenderCopy(renderer, deathTexture, NULL, &deathRect);
    }
}

void DrawWeaponsHUD(SDL_Renderer *renderer, Player *player, TTF_Font *font, TTF_Font *fontNumbers, SDL_Texture *weaponTextures[], SDL_Texture *weaponTexturesLocked[], SDL_Texture *weaponTexturesActive[])
{
    SDL_Color white = {255, 255, 255, 255};

    int circleRadius = 58;
    int circleX = circleRadius + 22;
    int circleY = circleRadius + 22;

    int baseOffsetX = player->id == 2 ? SCREEN_WIDTH - circleX * 2 - 5 : 0; // Adjusted for player 2

    // Draw "Player 1" text below the circle
    TTF_Font *fontPlayer = LoadFont("Assets/Font/Lilita_One_Regular.ttf", 20);
    const char *playerText = player->id == 2 ? "Player 2" : "Player 1";

    SDL_Surface *surfacePlayer = TTF_RenderText_Solid(fontPlayer, playerText, white);
    SDL_Texture *texturePlayer = SDL_CreateTextureFromSurface(renderer, surfacePlayer);
    SDL_Rect textRectPlayer = {
        baseOffsetX + circleX - surfacePlayer->w / 2,
        circleY + circleRadius + 10,
        surfacePlayer->w,
        surfacePlayer->h};
    SDL_RenderCopy(renderer, texturePlayer, NULL, &textRectPlayer);
    SDL_FreeSurface(surfacePlayer);
    SDL_DestroyTexture(texturePlayer);
    TTF_CloseFont(fontPlayer);

    SDL_Rect rectWeapon;
    rectWeapon.w = 128;
    rectWeapon.h = 64;

    // Draw shotgun
    rectWeapon.x = baseOffsetX + 20;
    rectWeapon.y = textRectPlayer.y + textRectPlayer.h + 10;

    if (player->id == 1)
    {
        if (player->hasWeapon[WEAPON_SHOTGUN])
        {
            if (player->weapon == WEAPON_SHOTGUN)
            {
                SDL_SetTextureAlphaMod(weaponTextures[WEAPON_SHOTGUN], 255);
                SDL_RenderCopy(renderer, weaponTextures[WEAPON_SHOTGUN], NULL, &rectWeapon);
                SDL_SetTextureAlphaMod(weaponTexturesActive[WEAPON_SHOTGUN], 80);
                SDL_RenderCopy(renderer, weaponTexturesActive[WEAPON_SHOTGUN], NULL, &rectWeapon);
            }
            else
            {
                SDL_SetTextureAlphaMod(weaponTextures[WEAPON_SHOTGUN], 255);
                SDL_RenderCopy(renderer, weaponTextures[WEAPON_SHOTGUN], NULL, &rectWeapon);
            }
        }
        else
        {
            SDL_SetTextureAlphaMod(weaponTexturesLocked[WEAPON_SHOTGUN_LOCKED], 255);
            SDL_RenderCopy(renderer, weaponTexturesLocked[WEAPON_SHOTGUN_LOCKED], NULL, &rectWeapon);
        }
    }
    else
    {
        if (player->hasWeapon[WEAPON_SHOTGUN])
        {
            if (player->weapon == WEAPON_SHOTGUN)
            {
                SDL_SetTextureAlphaMod(weaponTextures[WEAPON_SHOTGUN], 255);
                SDL_RenderCopyEx(renderer, weaponTextures[WEAPON_SHOTGUN], NULL, &rectWeapon, 0, NULL, SDL_FLIP_HORIZONTAL);
                SDL_SetTextureAlphaMod(weaponTexturesActive[WEAPON_SHOTGUN], 80);
                SDL_RenderCopyEx(renderer, weaponTexturesActive[WEAPON_SHOTGUN], NULL, &rectWeapon, 0, NULL, SDL_FLIP_HORIZONTAL);
            }
            else
            {
                SDL_SetTextureAlphaMod(weaponTextures[WEAPON_SHOTGUN], 255);
                SDL_RenderCopyEx(renderer, weaponTextures[WEAPON_SHOTGUN], NULL, &rectWeapon, 0, NULL, SDL_FLIP_HORIZONTAL);
            }
        }
        else
        {
            SDL_SetTextureAlphaMod(weaponTexturesLocked[WEAPON_SHOTGUN_LOCKED], 255);
            SDL_RenderCopyEx(renderer, weaponTexturesLocked[WEAPON_SHOTGUN_LOCKED], NULL, &rectWeapon, 0, NULL, SDL_FLIP_HORIZONTAL);
        }
    }

    // Draw number 3 or 8
    const char *numberText3 = player->id == 2 ? "8" : "3";
    SDL_Surface *surfaceNumber3 = TTF_RenderText_Solid(fontNumbers, numberText3, white);
    SDL_Texture *textureNumber3 = SDL_CreateTextureFromSurface(renderer, surfaceNumber3);
    SDL_Rect rectNumber3 = {
        player->id == 1 ? rectWeapon.x - surfaceNumber3->w + 2 : rectWeapon.x + rectWeapon.w - 3,
        rectWeapon.y + (rectWeapon.h - surfaceNumber3->h) / 2 + 30,
        surfaceNumber3->w,
        surfaceNumber3->h};
    SDL_RenderCopy(renderer, textureNumber3, NULL, &rectNumber3);
    SDL_FreeSurface(surfaceNumber3);
    SDL_DestroyTexture(textureNumber3);

    SDL_Rect rectWeaponHUD;
    rectWeaponHUD.w = 40;
    rectWeaponHUD.h = 20;

    // Draw shotgun ammo
    if (player->weapon == WEAPON_SHOTGUN)
    {
        rectWeaponHUD.x = player->id == 1 ? rectWeapon.x + rectWeapon.w + 5 : rectWeapon.x - rectWeaponHUD.w - 5;
        rectWeaponHUD.y = rectWeapon.y + (rectWeapon.h - rectWeaponHUD.h) / 2;
        char ammoText[10];
        sprintf(ammoText, "%d/%d", player->ammo_left, player->full_ammo);
        SDL_Surface *surfaceAmmo = TTF_RenderText_Solid(fontNumbers, ammoText, white);
        SDL_Texture *textureAmmo = SDL_CreateTextureFromSurface(renderer, surfaceAmmo);
        SDL_Rect rectAmmo = {
            player->id == 1 ? rectWeaponHUD.x + rectWeaponHUD.w + 5 : rectWeaponHUD.x - surfaceAmmo->w - 5,
            rectWeaponHUD.y,
            surfaceAmmo->w,
            surfaceAmmo->h};
        SDL_RenderCopy(renderer, textureAmmo, NULL, &rectAmmo);
        SDL_FreeSurface(surfaceAmmo);
        SDL_DestroyTexture(textureAmmo);
    }

    // Draw rifle
    rectWeapon.y += rectWeapon.h + 10;
    if (player->id == 1)
    {
        if (player->hasWeapon[WEAPON_RIFLE])
        {
            if (player->weapon == WEAPON_RIFLE)
            {
                SDL_SetTextureAlphaMod(weaponTextures[WEAPON_RIFLE], 255);
                SDL_RenderCopy(renderer, weaponTextures[WEAPON_RIFLE], NULL, &rectWeapon);
                SDL_SetTextureAlphaMod(weaponTexturesActive[WEAPON_RIFLE], 80);
                SDL_RenderCopy(renderer, weaponTexturesActive[WEAPON_RIFLE], NULL, &rectWeapon);
            }
            else
            {
                SDL_SetTextureAlphaMod(weaponTextures[WEAPON_RIFLE], 255);
                SDL_RenderCopy(renderer, weaponTextures[WEAPON_RIFLE], NULL, &rectWeapon);
            }
        }
        else
        {
            SDL_SetTextureAlphaMod(weaponTexturesLocked[WEAPON_RIFLE_LOCKED], 255);
            SDL_RenderCopy(renderer, weaponTexturesLocked[WEAPON_RIFLE_LOCKED], NULL, &rectWeapon);
        }
    }
    else
    {
        if (player->hasWeapon[WEAPON_RIFLE])
        {
            if (player->weapon == WEAPON_RIFLE)
            {
                SDL_SetTextureAlphaMod(weaponTextures[WEAPON_RIFLE], 255);
                SDL_RenderCopyEx(renderer, weaponTextures[WEAPON_RIFLE], NULL, &rectWeapon, 0, NULL, SDL_FLIP_HORIZONTAL);
                SDL_SetTextureAlphaMod(weaponTexturesActive[WEAPON_RIFLE], 80);
                SDL_RenderCopyEx(renderer, weaponTexturesActive[WEAPON_RIFLE], NULL, &rectWeapon, 0, NULL, SDL_FLIP_HORIZONTAL);
            }
            else
            {
                SDL_SetTextureAlphaMod(weaponTextures[WEAPON_RIFLE], 255);
                SDL_RenderCopyEx(renderer, weaponTextures[WEAPON_RIFLE], NULL, &rectWeapon, 0, NULL, SDL_FLIP_HORIZONTAL);
            }
        }
        else
        {
            SDL_SetTextureAlphaMod(weaponTexturesLocked[WEAPON_RIFLE_LOCKED], 255);
            SDL_RenderCopyEx(renderer, weaponTexturesLocked[WEAPON_RIFLE_LOCKED], NULL, &rectWeapon, 0, NULL, SDL_FLIP_HORIZONTAL);
        }
    }

    // Draw number 2 or 9
    const char *numberText2 = player->id == 2 ? "9" : "2";
    SDL_Surface *surfaceNumber2 = TTF_RenderText_Solid(fontNumbers, numberText2, white);
    SDL_Texture *textureNumber2 = SDL_CreateTextureFromSurface(renderer, surfaceNumber2);
    SDL_Rect rectNumber2 = {
        player->id == 1 ? rectWeapon.x - surfaceNumber2->w + 2 : rectWeapon.x + rectWeapon.w - 3,
        rectWeapon.y + (rectWeapon.h - surfaceNumber2->h) / 2 + 30,
        surfaceNumber2->w,
        surfaceNumber2->h};
    SDL_RenderCopy(renderer, textureNumber2, NULL, &rectNumber2);
    SDL_FreeSurface(surfaceNumber2);
    SDL_DestroyTexture(textureNumber2);

    // Draw bullet
    if (player->weapon == WEAPON_RIFLE)
    {
        rectWeaponHUD.x = player->id == 1 ? rectWeapon.x + rectWeapon.w + 5 : rectWeapon.x - rectWeaponHUD.w - 5;
        rectWeaponHUD.y = rectWeapon.y + (rectWeapon.h - rectWeaponHUD.h) / 2;
        char ammoText[10];
        sprintf(ammoText, "%d/%d", player->ammo_left, player->full_ammo);
        SDL_Surface *surfaceAmmo = TTF_RenderText_Solid(fontNumbers, ammoText, white);
        SDL_Texture *textureAmmo = SDL_CreateTextureFromSurface(renderer, surfaceAmmo);
        SDL_Rect rectAmmo = {
            player->id == 1 ? rectWeaponHUD.x + rectWeaponHUD.w + 5 : rectWeaponHUD.x - surfaceAmmo->w - 5,
            rectWeaponHUD.y,
            surfaceAmmo->w,
            surfaceAmmo->h};
        SDL_RenderCopy(renderer, textureAmmo, NULL, &rectAmmo);
        SDL_FreeSurface(surfaceAmmo);
        SDL_DestroyTexture(textureAmmo);
    }

    // Draw pistol
    rectWeapon.y += rectWeapon.h + 10;
    if (player->id == 1)
    {
        if (player->weapon == WEAPON_PISTOL)
        {
            SDL_RenderCopy(renderer, weaponTextures[WEAPON_PISTOL], NULL, &rectWeapon);
            SDL_SetTextureAlphaMod(weaponTexturesActive[WEAPON_PISTOL], 80);
            SDL_RenderCopy(renderer, weaponTexturesActive[WEAPON_PISTOL], NULL, &rectWeapon);
        }
        else
        {
            SDL_SetTextureAlphaMod(weaponTextures[WEAPON_PISTOL], 255);
            SDL_RenderCopy(renderer, weaponTextures[WEAPON_PISTOL], NULL, &rectWeapon);
        }
    }
    else
    {
        if (player->weapon == WEAPON_PISTOL)
        {
            SDL_RenderCopyEx(renderer, weaponTextures[WEAPON_PISTOL], NULL, &rectWeapon, 0, NULL, SDL_FLIP_HORIZONTAL);
            SDL_SetTextureAlphaMod(weaponTexturesActive[WEAPON_PISTOL], 80);
            SDL_RenderCopyEx(renderer, weaponTexturesActive[WEAPON_PISTOL], NULL, &rectWeapon, 0, NULL, SDL_FLIP_HORIZONTAL);
        }
        else
        {
            SDL_SetTextureAlphaMod(weaponTextures[WEAPON_PISTOL], 255);
            SDL_RenderCopyEx(renderer, weaponTextures[WEAPON_PISTOL], NULL, &rectWeapon, 0, NULL, SDL_FLIP_HORIZONTAL);
        }
    }

    // Draw number 1 or 0
    const char *numberText1 = player->id == 2 ? "0" : "1";
    SDL_Surface *surfaceNumber1 = TTF_RenderText_Solid(fontNumbers, numberText1, white);
    SDL_Texture *textureNumber1 = SDL_CreateTextureFromSurface(renderer, surfaceNumber1);
    SDL_Rect rectNumber1 = {
        player->id == 1 ? rectWeapon.x - surfaceNumber1->w + 2 : rectWeapon.x + rectWeapon.w - 3,
        rectWeapon.y + (rectWeapon.h - surfaceNumber1->h) / 2 + 30,
        surfaceNumber1->w,
        surfaceNumber1->h};
    SDL_RenderCopy(renderer, textureNumber1, NULL, &rectNumber1);
    SDL_FreeSurface(surfaceNumber1);
    SDL_DestroyTexture(textureNumber1);

    // Draw bullet
    if (player->weapon == WEAPON_PISTOL)
    {
        rectWeaponHUD.x = player->id == 1 ? rectWeapon.x + rectWeapon.w + 5 : rectWeapon.x - rectWeaponHUD.w - 5;
        rectWeaponHUD.y = rectWeapon.y + (rectWeapon.h - rectWeaponHUD.h) / 2;
        char ammoText[50];
        sprintf(ammoText, "%d/**", player->ammo_left);
        SDL_Surface *surfaceAmmo = TTF_RenderText_Solid(fontNumbers, ammoText, white);
        SDL_Texture *textureAmmo = SDL_CreateTextureFromSurface(renderer, surfaceAmmo);
        SDL_Rect rectAmmo = {
            player->id == 1 ? rectWeaponHUD.x + rectWeaponHUD.w + 5 : rectWeaponHUD.x - surfaceAmmo->w - 5,
            rectWeaponHUD.y,
            surfaceAmmo->w,
            surfaceAmmo->h};
        SDL_RenderCopy(renderer, textureAmmo, NULL, &rectAmmo);
        SDL_FreeSurface(surfaceAmmo);
        SDL_DestroyTexture(textureAmmo);
    }
}

void RenderHUD(SDL_Renderer *renderer, Player *player1, Player *player2, Uint32 elapsedTime, SDL_Texture *pillowMainHUD1[], SDL_Texture *pillowMainHUD2[], SDL_Texture *pillowPlayer1Dead[], SDL_Texture *pillowPlayer2Dead[])
{
    DrawHUDPlayer(renderer, player1, player2);

    // Circle in left upper part
    int circleRadius = 60;
    int circleX = circleRadius + 20;
    int circleY = circleRadius + 20;

    SDL_Color circleColor = {255, 255, 255, 150};
    SDL_Color white = {255, 255, 255, 255};

    DrawCircle(renderer, circleX, circleY, circleRadius, circleColor);

    // Draw pillows inside circles
    SDL_Rect pillowRect1 = {circleX - TEXTURE_SIZE / 2, circleY - TEXTURE_SIZE / 2, TEXTURE_SIZE, TEXTURE_SIZE};

    // Pillow animation
    int currentFrame = (SDL_GetTicks() / 100) % PILLOW_ANIMATION_FRAMES;
    int currentFrame1 = (SDL_GetTicks() / 300) % PILLOW_ANIMATION_FRAMES_DEAD;

    if (player1->isAlive)
    {
        if (pillowMainHUD1[currentFrame])
            SDL_RenderCopy(renderer, pillowMainHUD1[currentFrame], NULL, &pillowRect1);
    }
    else if (!player1->isAlive)
    {
        DrawDeathAnimation(renderer, player1, deathTextures);
        if (pillowPlayer1Dead[currentFrame1])
            SDL_RenderCopy(renderer, pillowPlayer1Dead[currentFrame1], NULL, &pillowRect1);
    }

    if (playersCount == 2)
    {
        // Circle in right upper part
        int circleRadius2 = 60;
        int circleX2 = SCREEN_WIDTH - circleRadius2 - 20;
        int circleY2 = circleRadius2 + 20;

        DrawCircle(renderer, circleX2, circleY2, circleRadius2, circleColor);
        SDL_Rect pillowRect2 = {circleX2 - TEXTURE_SIZE / 2, circleY2 - TEXTURE_SIZE / 2, TEXTURE_SIZE, TEXTURE_SIZE};

        if (player2->isAlive)
        {
            // Main Character pillow in circle
            if (pillowMainHUD2[currentFrame])
            {
                SDL_RenderCopy(renderer, pillowMainHUD2[currentFrame], NULL, &pillowRect2);
            }
        }
        else if (!player2->isAlive)
        {
            if (pillowPlayer2Dead[currentFrame1])
            {
                SDL_RenderCopy(renderer, pillowPlayer2Dead[currentFrame1], NULL, &pillowRect2);
            }
        }

        // Down bar
        SDL_Rect rectDown = {0, SCREEN_HEIGHT - 100, SCREEN_WIDTH, 100};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
        SDL_RenderFillRect(renderer, &rectDown);

        // Draw "Top" text on the left side of the bar
        TTF_Font *fontTop = LoadFont("Assets/Font/Duepuntozero Pro Bold.otf", 35);
        TTF_Font *fontSmall = LoadFont("Assets/Font/Duepuntozero Pro Bold.otf", 20);

        // Player in bar
        strcpy(leaderboard[0].name, player1->name);
        leaderboard[0].kills = player1->kills;
        strcpy(leaderboard[1].name, player2->name);
        leaderboard[1].kills = player2->kills;

        // Bots in bar
        for (int i = 0; i < settings.botCount; i++)
        {
            strcpy(leaderboard[i + 2].name, bots[i].name);
            leaderboard[i + 2].kills = bots[i].kills;
        }

        // Sorting by kills
        int totalCount = settings.botCount + 2;
        for (int i = 0; i < totalCount - 1; i++)
        {
            for (int j = i + 1; j < totalCount; j++)
            {
                if (leaderboard[j].kills > leaderboard[i].kills)
                {
                    // Swap names and kills
                    char tempName[50];
                    int tempKills;

                    strcpy(tempName, leaderboard[i].name);
                    tempKills = leaderboard[i].kills;

                    strcpy(leaderboard[i].name, leaderboard[j].name);
                    leaderboard[i].kills = leaderboard[j].kills;

                    strcpy(leaderboard[j].name, tempName);
                    leaderboard[j].kills = tempKills;
                }
            }
        }

        int xOffset = 10;
        for (int i = 0; i < totalCount; i++)
        {
            char leaderText[100];
            char rankText[10];

            // Add rank for top 3
            if (i == 0)
                sprintf(rankText, "1.");
            else if (i == 1)
                sprintf(rankText, "2.");
            else if (i == 2)
                sprintf(rankText, "3.");
            else
                rankText[0] = '\0';

            sprintf(leaderText, "%s\n%d k.", leaderboard[i].name, leaderboard[i].kills);

            // Rank
            if (rankText[0] != '\0')
            {
                SDL_Surface *surfaceRankText = TTF_RenderText_Solid(fontTop, rankText, white);
                SDL_Texture *textureRankText = SDL_CreateTextureFromSurface(renderer, surfaceRankText);
                SDL_Rect rectRankText = {xOffset, SCREEN_HEIGHT - 80, surfaceRankText->w, surfaceRankText->h};
                SDL_RenderCopy(renderer, textureRankText, NULL, &rectRankText);
                xOffset += surfaceRankText->w + 5;
                SDL_FreeSurface(surfaceRankText);
                SDL_DestroyTexture(textureRankText);
            }

            // Leaderboard
            SDL_Surface *surfaceLeaderText = TTF_RenderText_Blended_Wrapped(fontSmall, leaderText, white, 150);
            SDL_Texture *textureLeaderText = SDL_CreateTextureFromSurface(renderer, surfaceLeaderText);
            SDL_Rect rectLeaderText = {xOffset, SCREEN_HEIGHT - 80, 150, surfaceLeaderText->h};
            SDL_RenderCopy(renderer, textureLeaderText, NULL, &rectLeaderText);

            SDL_FreeSurface(surfaceLeaderText);
            SDL_DestroyTexture(textureLeaderText);

            xOffset += 140;
        }
    }

    if (playersCount == 1)
    {
        // Top right bar
        SDL_Rect rectTopRight = {SCREEN_WIDTH - 250, 0, 250, 400};
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
        SDL_RenderFillRect(renderer, &rectTopRight);

        TTF_Font *font = LoadFont("Assets/Font/Duepuntozero Pro Bold.otf", 35);
        TTF_Font *fontSmall = LoadFont("Assets/Font/Duepuntozero Pro Bold.otf", 20);

        // Draw Top text
        SDL_Surface *surfaceLeader = TTF_RenderText_Solid(font, "Top", white);
        SDL_Texture *textureLeader = SDL_CreateTextureFromSurface(renderer, surfaceLeader);
        SDL_Rect rectLeader = {
            SCREEN_WIDTH - 250 + (250 - surfaceLeader->w) / 2,
            10,
            surfaceLeader->w,
            surfaceLeader->h};
        SDL_RenderCopy(renderer, textureLeader, NULL, &rectLeader);
        SDL_FreeSurface(surfaceLeader);
        SDL_DestroyTexture(textureLeader);

        // Player in bar
        strcpy(leaderboard[0].name, player1->name);
        leaderboard[0].kills = player1->kills;

        // Bots in bar
        for (int i = 0; i < settings.botCount; i++)
        {
            strcpy(leaderboard[i + 1].name, bots[i].name);
            leaderboard[i + 1].kills = bots[i].kills;
        }

        // Sorting by kills
        for (int i = 0; i < settings.botCount + 1; i++)
        {
            for (int j = i + 1; j < settings.botCount + 1; j++)
            {
                if (leaderboard[j].kills > leaderboard[i].kills)
                {
                    char tempName[50];
                    int tempKills;
                    strcpy(tempName, leaderboard[i].name);
                    tempKills = leaderboard[i].kills;
                    strcpy(leaderboard[i].name, leaderboard[j].name);
                    leaderboard[i].kills = leaderboard[j].kills;
                    strcpy(leaderboard[j].name, tempName);
                    leaderboard[j].kills = tempKills;
                }
            }
        }

        // Draw the leaderboard
        int yOffset = 50;
        for (int i = 0; i < settings.botCount + 1; i++)
        {
            char leaderText[50];
            sprintf(leaderText, "%s: %d k.", leaderboard[i].name, leaderboard[i].kills);

            SDL_Surface *surfaceLeaderText = TTF_RenderText_Solid(fontSmall, leaderText, white);
            SDL_Texture *textureLeaderText = SDL_CreateTextureFromSurface(renderer, surfaceLeaderText);
            SDL_Rect rectLeaderText = {SCREEN_WIDTH - 250 + 25, yOffset, surfaceLeaderText->w, surfaceLeaderText->h};
            SDL_RenderCopy(renderer, textureLeaderText, NULL, &rectLeaderText);
            SDL_FreeSurface(surfaceLeaderText);
            SDL_DestroyTexture(textureLeaderText);

            yOffset += 30;
        }

        TTF_CloseFont(fontSmall);
        TTF_CloseFont(font);
    }
}

void HUDDraw(SDL_Renderer *renderer, Player *player1, Player *player2, Uint32 elapsedTime)
{
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color green = {147, 242, 13, 255};
    SDL_Color black = {0, 0, 0, 255};

    TTF_Font *font = LoadFont("Assets/Font/Lilita_One_Regular.ttf", 35);
    TTF_Font *fontNumbers = LoadFont("Assets/Font/Lilita_One_Regular.ttf", 25);

    if (player1->reloadStartTime != 0 && player1->isAlive)
    {
        player1->fireRate = -1;
        const char *reloadingText[] = {"Reloading .", "Reloading ..", "Reloading ..."};
        Uint32 elapsedTimeReload = SDL_GetTicks() - player1->reloadStartTime;
        int dotIndex = (elapsedTimeReload / 300) % 3;

        SDL_Surface *surface = TTF_RenderText_Solid(fontNumbers, reloadingText[dotIndex], white);
        if (surface)
        {
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (texture)
            {
                SDL_Rect textRect = {
                    player1->x + (TEXTURE_SIZE / 2) - surface->w / 2,
                    player1->y - 20,
                    surface->w,
                    surface->h};
                SDL_RenderCopy(renderer, texture, NULL, &textRect);
                SDL_FreeSurface(surface);
                SDL_DestroyTexture(texture);
            }
        }

        if (player1->weapon == WEAPON_PISTOL)
        {
            player1->fireRate = 500;
        }
        else if (player1->weapon == WEAPON_RIFLE)
        {
            player1->fireRate = 200;
        }
        else if (player1->weapon == WEAPON_SHOTGUN)
        {
            player1->fireRate = 1000;
        }
    }

    if (player2->reloadStartTime != 0 && player2->isAlive)
    {
        player2->fireRate = -1;
        const char *reloadingText[] = {"Reloading .", "Reloading ..", "Reloading ..."};
        Uint32 elapsedTimeReload = SDL_GetTicks() - player2->reloadStartTime;
        int dotIndex = (elapsedTimeReload / 300) % 3;

        SDL_Surface *surface = TTF_RenderText_Solid(fontNumbers, reloadingText[dotIndex], white);
        if (surface)
        {
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (texture)
            {
                SDL_Rect textRect = {
                    player2->x + (TEXTURE_SIZE / 2) - surface->w / 2,
                    player2->y - 20,
                    surface->w,
                    surface->h};
                SDL_RenderCopy(renderer, texture, NULL, &textRect);
                SDL_FreeSurface(surface);
                SDL_DestroyTexture(texture);
            }
        }

        if (player2->weapon == WEAPON_PISTOL)
        {
            player2->fireRate = 500;
        }
        else if (player2->weapon == WEAPON_RIFLE)
        {
            player2->fireRate = 200;
        }
        else if (player2->weapon == WEAPON_SHOTGUN)
        {
            player2->fireRate = 1000;
        }
    }

    // Timer in center upper part
    Uint32 elapsedSeconds = elapsedTime / 1000;
    Uint32 remainingSeconds = TOTAL_GAME_TIME / 1000 - elapsedSeconds;

    int minutes = remainingSeconds / 60;
    int seconds = remainingSeconds % 60;
    char timeText[20];
    sprintf(timeText, "%02d:%02d", minutes, seconds);

    // Time text
    SDL_Surface *surfaceTimeNumbers = TTF_RenderText_Solid(font, timeText, white);
    // Time background
    SDL_Rect rectTimeBackground = {
        (SCREEN_WIDTH - surfaceTimeNumbers->w * 2) / 2 - 10,
        0,
        surfaceTimeNumbers->w * 2 + 20,
        surfaceTimeNumbers->h * 2 + 5};
    SDL_SetRenderDrawColor(renderer, 99, 76, 33, 150); // Brown
    SDL_RenderFillRect(renderer, &rectTimeBackground);

    SDL_Texture *textureTimeNumbers = SDL_CreateTextureFromSurface(renderer, surfaceTimeNumbers);
    SDL_Rect rectTimeNumbers = {
        (SCREEN_WIDTH - surfaceTimeNumbers->w * 2) / 2,
        0,
        surfaceTimeNumbers->w * 2,
        surfaceTimeNumbers->h * 2};
    SDL_RenderCopy(renderer, textureTimeNumbers, NULL, &rectTimeNumbers);
    SDL_FreeSurface(surfaceTimeNumbers);
    SDL_DestroyTexture(textureTimeNumbers);

    // Circle in left upper part
    int circleRadius = 58;
    int circleX = circleRadius + 22;
    int circleY = circleRadius + 22;

    // Draw circle
    DrawCircleOutline(renderer, circleX, circleY, circleRadius, black);

    // Draw weapons for player 1
    if (player1->isAlive)
    {
        DrawWeaponsHUD(renderer, player1, font, fontNumbers, weaponTextures, weaponTexturesLocked, weaponTexturesActive);
    }

    // Draw weapons for player 2
    if (playersCount == 2 && player2->isAlive)
    {
        DrawWeaponsHUD(renderer, player2, font, fontNumbers, weaponTextures, weaponTexturesLocked, weaponTexturesActive);
    }

    TTF_CloseFont(font);
    TTF_CloseFont(fontNumbers);
}
