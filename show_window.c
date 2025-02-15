#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "show_window.h"
#include "character.h"
#include "buttons.h"
#include "bullet.h"
#include "bots.h"
#include "collision.h"

extern void RenderBots(SDL_Renderer *renderer);
extern Settings settings;

extern int playersCount;
extern Bot bots[MAX_BOTS];

extern Player player1;
extern Player player2;
int playerCount = 2;

void ShowMainWindow(SDL_Renderer *renderer, TTF_Font *font, SDL_Texture *backgroundTexture)
{
    // Background
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    // Buttons
    ShowMainButtons(renderer, font);
}

void ShowGameStartWindow(SDL_Renderer *renderer, TTF_Font *font, SDL_Texture *levelmap, Player *player1, Player *player2, int *botsCount, int *difficulty)
{
    // Background
    SDL_RenderCopy(renderer, levelmap, NULL, NULL);

    RenderPlayer(renderer, player1);

    if (playersCount == 2)
        RenderPlayer(renderer, player2);

    RenderBullets(renderer);

    RenderBots(renderer);
}

void ShowGameMenuWindow(SDL_Renderer *renderer, TTF_Font *font, SDL_Texture *backgroundTexture, int *botCount, int *playersCount, int *botDifficulty, char *color1, char *color2)
{
    // Background
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    // Buttons
    ShowGameMenuButtons(renderer, font, botCount, botDifficulty, playersCount, color1, color2);
}

void ShowOptionsWindow(SDL_Renderer *renderer, TTF_Font *font, SDL_Texture *backgroundTexture, int *soundValue, int *brightnessValue)
{
    // Background
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    // Buttons
    ShowOptionsButtons(renderer, font, soundValue, brightnessValue);
}

void ShowRecordsWindow(SDL_Renderer *renderer, TTF_Font *font, SDL_Texture *backgroundTexture)
{
    // Background
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    // Buttons
    ShowRecordsButtons(renderer, font);
}

void ShowTop1Window(SDL_Renderer *renderer, TTF_Font *font, SDL_Texture *backgroundTexture)
{
    // Background
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    SDL_Color textColor = {49, 24, 130, 255};

    char nameRecord[50];
    int killRecord, timeRecord, timeSpent;

    const char *filePath = "Records/top_1.txt";
    FILE *file = fopen(filePath, "r");

    if (fscanf(file, "%s %d %d %d", nameRecord, &killRecord, &timeRecord, &timeSpent) != 4)
        fclose(file);

    // Records png "Records/top1_pillow.png"
    SDL_Texture *mainCharacterPillowTexture1 = IMG_LoadTexture(renderer, "Records/top1_pillow.png");

    SDL_Color squareColor = {255, 255, 255, 170};

    int circleRadius1 = 100;
    int circleX1 = SCREEN_WIDTH / 2;
    int circleY1 = (SCREEN_HEIGHT / 2) - 200;

    DrawCircle(renderer, circleX1, circleY1, circleRadius1, squareColor);

    // Main Character pillow texture
    if (mainCharacterPillowTexture1)
    {
        SDL_Rect pillowRect1 = {
            circleX1 - circleRadius1,
            circleY1 - circleRadius1 - 20,
            circleRadius1 * 2,
            circleRadius1 * 2};
        SDL_RenderCopy(renderer, mainCharacterPillowTexture1, NULL, &pillowRect1);
    }

    // Square for record information
    int squareWidth = 400;
    int squareHeight = 200;
    SDL_Rect infoSquare = {
        (SCREEN_WIDTH - squareWidth) / 2,
        circleY1 + circleRadius1 + 50,
        squareWidth,
        squareHeight};
    SDL_SetRenderDrawColor(renderer, 208, 207, 192, 255);
    SDL_RenderFillRect(renderer, &infoSquare);

    // Record Name
    int centerX = infoSquare.x + infoSquare.w / 2;
    char nameText[100];
    sprintf(nameText, "%s", nameRecord);
    SDL_Surface *surfaceName = TTF_RenderText_Solid(font, nameText, textColor);
    SDL_Texture *textureName = SDL_CreateTextureFromSurface(renderer, surfaceName);
    SDL_Rect rectName = {centerX - surfaceName->w / 2, infoSquare.y + 20, surfaceName->w, surfaceName->h};
    SDL_RenderCopy(renderer, textureName, NULL, &rectName);
    SDL_FreeSurface(surfaceName);
    SDL_DestroyTexture(textureName);

    // Record Kills Label
    const char *killsLabel = "Kills:";
    SDL_Surface *surfaceKillsLabel = TTF_RenderText_Solid(font, killsLabel, textColor);
    SDL_Texture *textureKillsLabel = SDL_CreateTextureFromSurface(renderer, surfaceKillsLabel);
    SDL_Rect rectKillsLabel = {infoSquare.x + 20, infoSquare.y + 60, surfaceKillsLabel->w, surfaceKillsLabel->h};
    SDL_RenderCopy(renderer, textureKillsLabel, NULL, &rectKillsLabel);
    SDL_FreeSurface(surfaceKillsLabel);
    SDL_DestroyTexture(textureKillsLabel);

    // Record Kills Value
    char killsValue[100];
    sprintf(killsValue, "%d", killRecord);
    SDL_Surface *surfaceKillsValue = TTF_RenderText_Solid(font, killsValue, textColor);
    SDL_Texture *textureKillsValue = SDL_CreateTextureFromSurface(renderer, surfaceKillsValue);
    SDL_Rect rectKillsValue = {infoSquare.x + infoSquare.w - surfaceKillsValue->w - 20, infoSquare.y + 60, surfaceKillsValue->w, surfaceKillsValue->h};
    SDL_RenderCopy(renderer, textureKillsValue, NULL, &rectKillsValue);
    SDL_FreeSurface(surfaceKillsValue);
    SDL_DestroyTexture(textureKillsValue);

    // Record Time Label
    const char *timeLabel = "Time:";
    SDL_Surface *surfaceTimeLabel = TTF_RenderText_Solid(font, timeLabel, textColor);
    SDL_Texture *textureTimeLabel = SDL_CreateTextureFromSurface(renderer, surfaceTimeLabel);
    SDL_Rect rectTimeLabel = {infoSquare.x + 20, infoSquare.y + 100, surfaceTimeLabel->w, surfaceTimeLabel->h};
    SDL_RenderCopy(renderer, textureTimeLabel, NULL, &rectTimeLabel);
    SDL_FreeSurface(surfaceTimeLabel);
    SDL_DestroyTexture(textureTimeLabel);

    // Record Time Value
    char timeValue[100];
    sprintf(timeValue, "%d", timeRecord);
    SDL_Surface *surfaceTimeValue = TTF_RenderText_Solid(font, timeValue, textColor);
    SDL_Texture *textureTimeValue = SDL_CreateTextureFromSurface(renderer, surfaceTimeValue);
    SDL_Rect rectTimeValue = {infoSquare.x + infoSquare.w - surfaceTimeValue->w - 20, infoSquare.y + 100, surfaceTimeValue->w, surfaceTimeValue->h};
    SDL_RenderCopy(renderer, textureTimeValue, NULL, &rectTimeValue);
    SDL_FreeSurface(surfaceTimeValue);
    SDL_DestroyTexture(textureTimeValue);

    ShowTop1Buttons(renderer, font);
}

void ShowTop10Window(SDL_Renderer *renderer, TTF_Font *font, SDL_Texture *backgroundTexture)
{
    // Background
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    // Box for top 10
    int rectWidth = SCREEN_WIDTH / 2;
    int rectHeight = 600;
    SDL_Rect top10Rect = {
        SCREEN_WIDTH / 4,
        100,
        rectWidth,
        rectHeight};
    SDL_SetRenderDrawColor(renderer, 208, 207, 192, 255);
    SDL_RenderFillRect(renderer, &top10Rect);

    // Top 10 text
    const char *headerText = "Top 10";
    SDL_Color textColor = {49, 24, 130, 255};
    SDL_Surface *headerSurface = TTF_RenderText_Solid(font, headerText, textColor);
    if (headerSurface)
    {
        SDL_Texture *headerTexture = SDL_CreateTextureFromSurface(renderer, headerSurface);
        SDL_Rect headerRect = {
            top10Rect.x + (top10Rect.w - headerSurface->w) / 2,
            top10Rect.y + 10,
            headerSurface->w,
            headerSurface->h};
        SDL_RenderCopy(renderer, headerTexture, NULL, &headerRect);
        SDL_FreeSurface(headerSurface);
        SDL_DestroyTexture(headerTexture);
    }

    const char *columnHeaders[4] = {"#", "NAME", "KILLS", "TIME RECORD"};

    int startX = top10Rect.x + 40;
    int startY = top10Rect.y + 80;
    int columnSpacing = (top10Rect.w - 40) / 4;

    for (int i = 0; i < 4; i++)
    {
        SDL_Surface *textSurface = TTF_RenderText_Solid(font, columnHeaders[i], textColor);
        if (textSurface)
        {
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            if (i == 0)
            {
                SDL_Rect textRect = {
                    startX + 40,
                    startY,
                    textSurface->w,
                    textSurface->h};
                SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            }
            else if (i == 3)
            {
                SDL_Rect textRect = {
                    startX + i * columnSpacing - 50,
                    startY,
                    textSurface->w,
                    textSurface->h};
                SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            }
            else
            {
                SDL_Rect textRect = {
                    startX + i * columnSpacing,
                    startY,
                    textSurface->w,
                    textSurface->h};
                SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            }
            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }
    }

    const char *filePath = "Records/top_10.txt";
    FILE *file = fopen(filePath, "r");
    if (file)
    {
        char line[256];
        int lineNumber = 0;
        while (fgets(line, sizeof(line), file) && lineNumber < 10)
        {
            int position;
            char name[50];
            int kills;
            int timeRecord;

            if (sscanf(line, "%d %s %d %d", &position, name, &kills, &timeRecord) == 4)
            {
                int entryX = startX;
                int entryY = startY + 40 + lineNumber * 40;

                // Number
                char posText[10];
                sprintf(posText, "%d", position);
                SDL_Surface *posSurface = TTF_RenderText_Solid(font, posText, textColor);
                if (posSurface)
                {
                    SDL_Texture *posTexture = SDL_CreateTextureFromSurface(renderer, posSurface);
                    SDL_Rect posRect = {
                        entryX + 40,
                        entryY,
                        posSurface->w,
                        posSurface->h};
                    SDL_RenderCopy(renderer, posTexture, NULL, &posRect);
                    SDL_FreeSurface(posSurface);
                    SDL_DestroyTexture(posTexture);
                }

                // Name
                SDL_Surface *nameSurface = TTF_RenderText_Solid(font, name, textColor);
                if (nameSurface)
                {
                    SDL_Texture *nameTexture = SDL_CreateTextureFromSurface(renderer, nameSurface);
                    SDL_Rect nameRect = {
                        entryX + columnSpacing,
                        entryY,
                        nameSurface->w,
                        nameSurface->h};
                    SDL_RenderCopy(renderer, nameTexture, NULL, &nameRect);
                    SDL_FreeSurface(nameSurface);
                    SDL_DestroyTexture(nameTexture);
                }

                // Kills Record
                char killsText[10];
                sprintf(killsText, "%d", kills);
                SDL_Surface *killsSurface = TTF_RenderText_Solid(font, killsText, textColor);
                if (killsSurface)
                {
                    SDL_Texture *killsTexture = SDL_CreateTextureFromSurface(renderer, killsSurface);
                    SDL_Rect killsRect = {
                        entryX + (2 * columnSpacing) + 25,
                        entryY,
                        killsSurface->w,
                        killsSurface->h};
                    SDL_RenderCopy(renderer, killsTexture, NULL, &killsRect);
                    SDL_FreeSurface(killsSurface);
                    SDL_DestroyTexture(killsTexture);
                }

                // Time Record
                char timeRecordText[20];
                sprintf(timeRecordText, "%d", timeRecord);
                SDL_Surface *timeRecordSurface = TTF_RenderText_Solid(font, timeRecordText, textColor);
                if (timeRecordSurface)
                {
                    SDL_Texture *timeRecordTexture = SDL_CreateTextureFromSurface(renderer, timeRecordSurface);
                    SDL_Rect timeRecordRect = {
                        entryX + (3 * columnSpacing) + 40,
                        entryY,
                        timeRecordSurface->w,
                        timeRecordSurface->h};
                    SDL_RenderCopy(renderer, timeRecordTexture, NULL, &timeRecordRect);
                    SDL_FreeSurface(timeRecordSurface);
                    SDL_DestroyTexture(timeRecordTexture);
                }

                lineNumber++;
            }
        }
        fclose(file);
    }
    ShowTop10Buttons(renderer, font);
}

void ShowGameOverWindow(SDL_Renderer *renderer, TTF_Font *font)
{
    SDL_Color buttonColor = {208, 207, 192, 230};
    SDL_Color textColor = {49, 24, 130, 255};

    SDL_Surface *textSurface = TTF_RenderText_Solid(font, "GAME OVER", textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = {(SCREEN_WIDTH - textSurface->w) / 2, SCREEN_HEIGHT / 4 - 50, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    int yOffset = SCREEN_HEIGHT / 4 + 50;
    // Display player 1 name and kills
    char player1Text[100];
    sprintf(player1Text, "%s: %d", player1.name, player1.kills);
    textSurface = TTF_RenderText_Solid(font, player1Text, textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    textRect.x = (SCREEN_WIDTH - textSurface->w) / 2;
    textRect.y = yOffset;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    yOffset += 40;

    // Display player 2 name and kills if exists
    if (playersCount == 2)
    {
        char player2Text[100];
        sprintf(player2Text, "%s: %d", player2.name, player2.kills);
        textSurface = TTF_RenderText_Solid(font, player2Text, textColor);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        textRect.x = (SCREEN_WIDTH - textSurface->w) / 2;
        textRect.y = yOffset;
        textRect.w = textSurface->w;
        textRect.h = textSurface->h;
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
        yOffset += 40;
    }

    for (int i = 0; i < settings.botCount; i++)
    {
        char botText[100];
        sprintf(botText, "(Bot) %s: %d", bots[i].name, bots[i].kills);
        textSurface = TTF_RenderText_Solid(font, botText, textColor);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        textRect.x = (SCREEN_WIDTH - textSurface->w) / 2;
        textRect.y = yOffset;
        textRect.w = textSurface->w;
        textRect.h = textSurface->h;
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
        yOffset += 40;
    }

    // Instructions for SAVE and BACK
    textSurface = TTF_RenderText_Solid(font, "Press ENTER to SAVE, ESC to BACK", textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    textRect.x = (SCREEN_WIDTH - textSurface->w) / 2;
    textRect.y = yOffset + 20;
    textRect.w = textSurface->w;
    textRect.h = textSurface->h;
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    SDL_Rect saveButtonRect = {SCREEN_WIDTH / 2 - 175, yOffset + 100, 150, 50};
    SDL_Rect backButtonRect = {SCREEN_WIDTH / 2 + 25, yOffset + 100, 150, 50};

    DrawRoundedButton(renderer, saveButtonRect, buttonColor, 10);
    DrawRoundedButton(renderer, backButtonRect, buttonColor, 10);

    const char *buttonTexts[2] = {"SAVE", "BACK"};
    for (int i = 0; i < 2; i++)
    {
        textSurface = TTF_RenderText_Solid(font, buttonTexts[i], textColor);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        textRect.x = (i == 0 ? saveButtonRect.x : backButtonRect.x) + (saveButtonRect.w - textSurface->w) / 2;
        textRect.y = (i == 0 ? saveButtonRect.y : backButtonRect.y) + (saveButtonRect.h - textSurface->h) / 2;
        textRect.w = textSurface->w;
        textRect.h = textSurface->h;
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }
}
