#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "buttons.h"
#include "common.h"
#include "character.h"
#include "collision.h"

SDL_Texture *pillowTextures[GIF_FRAME_COUNT] = {NULL};

SDL_Rect mainMenuButtonRects[4];
int mainMenuSelectedButton = 0;

SDL_Rect optionsMenuButtonRects[3];
int optionsMenuSelectedButton = 0;

SDL_Rect gameStartButtonRects[5];
int gameStartSelectedButton = 0;

SDL_Rect recordsButtonRects[3];
int recordsSelectedButton = 0;

SDL_Rect top1ButtonRects[1];
int top1SelectedButton;

SDL_Rect top10ButtonRects[1];
int top10SelectedButton;

int currentFrame = 0;
Uint32 lastFrameTime = 0;

SDL_Color textColor = {49, 24, 130, 255};
SDL_Color buttonColor = {208, 207, 192, 230};

void ShowMainButtons(SDL_Renderer *renderer, TTF_Font *font)
{
    int totalHeight = 4 * buttonHeight + 3 * 20;          // 4 buttons and 3 spaces
    int startY = (SCREEN_HEIGHT - totalHeight) / 2 + 100; // Centered vertically + 100 pixels down

    for (int i = 0; i < 3; i++)
    {
        mainMenuButtonRects[i].w = buttonWidth;
        mainMenuButtonRects[i].h = buttonHeight;
        mainMenuButtonRects[i].x = (SCREEN_WIDTH - buttonWidth) / 2;
        mainMenuButtonRects[i].y = startY + i * (buttonHeight + 20);
    }

    // Quit button
    mainMenuButtonRects[3].w = buttonWidth;
    mainMenuButtonRects[3].h = buttonHeight;
    mainMenuButtonRects[3].x = (SCREEN_WIDTH - buttonWidth) / 2;
    mainMenuButtonRects[3].y = startY + 3 * (buttonHeight + 60);

    // Button properties
    const char *buttonText[4] = {"START", "OPTIONS", "RECORDS", "QUIT"};

    for (int i = 0; i < 4; i++)
    {
        DrawRoundedButton(renderer, mainMenuButtonRects[i], buttonColor, cornerRadius);

        // Button text
        SDL_Surface *textSurface = TTF_RenderText_Solid(font, buttonText[i], textColor);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = {
            mainMenuButtonRects[i].x + (mainMenuButtonRects[i].w - textSurface->w) / 2,
            mainMenuButtonRects[i].y + (mainMenuButtonRects[i].h - textSurface->h) / 2,
            textSurface->w,
            textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }

    // Pillow animation
    if (mainMenuSelectedButton >= 0 && mainMenuSelectedButton < 4)
    {
        SDL_Rect pillowRect;
        pillowRect.w = 70;
        pillowRect.h = 70;
        pillowRect.x = mainMenuButtonRects[mainMenuSelectedButton].x - pillowRect.w - 10; // 10 pixel from the left
        pillowRect.y = mainMenuButtonRects[mainMenuSelectedButton].y + (buttonHeight - pillowRect.h) / 2;

        // Update pillow frame
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime > lastFrameTime + GIF_FRAME_DELAY)
        {
            currentFrame = (currentFrame + 1) % GIF_FRAME_COUNT;
            lastFrameTime = currentTime;
        }

        if (pillowTextures[currentFrame])
            SDL_RenderCopy(renderer, pillowTextures[currentFrame], NULL, &pillowRect);
    }
}

void ShowOptionsButtons(SDL_Renderer *renderer, TTF_Font *font, int *soundValue, int *brightnessValue)
{
    int totalHeight = 3 * buttonHeight + 2 * 20;          // 3 buttons and 2 spaces
    int startY = (SCREEN_HEIGHT - totalHeight) / 2 + 100; // Centered vertically

    for (int i = 0; i < 2; i++)
    {
        optionsMenuButtonRects[i].w = buttonWidth;
        optionsMenuButtonRects[i].h = buttonHeight;
        optionsMenuButtonRects[i].x = (SCREEN_WIDTH - buttonWidth) / 2;
        optionsMenuButtonRects[i].y = startY + i * (buttonHeight + 20);
    }

    // Back button
    optionsMenuButtonRects[2].w = buttonWidth;
    optionsMenuButtonRects[2].h = buttonHeight;
    optionsMenuButtonRects[2].x = (SCREEN_WIDTH - buttonWidth) / 2;
    optionsMenuButtonRects[2].y = startY + 2 * (buttonHeight + 60);

    const char *optionTexts[3] = {
        "SOUND",
        "BRIGHTNESS",
        "BACK"};

    for (int i = 0; i < 3; i++)
    {
        DrawRoundedButton(renderer, optionsMenuButtonRects[i], buttonColor, cornerRadius);

        // Render text
        SDL_Surface *textSurface = TTF_RenderText_Solid(font, optionTexts[i], textColor);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect;

        if (strcmp(optionTexts[i], "BACK") == 0)
        {
            textRect = (SDL_Rect){
                optionsMenuButtonRects[i].x + (optionsMenuButtonRects[i].w - textSurface->w) / 2,
                optionsMenuButtonRects[i].y + (buttonHeight - textSurface->h) / 2,
                textSurface->w,
                textSurface->h};
        }
        else
        {
            textRect = (SDL_Rect){
                optionsMenuButtonRects[i].x + 40,
                optionsMenuButtonRects[i].y + (buttonHeight - textSurface->h) / 2,
                textSurface->w,
                textSurface->h};
        }

        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);

        // If not BACK, draw squares to the right of the button
        if (i != 2)
        {
            int squareSize = 15;
            int padding = 10;
            int numSquares;

            if (i == 0)
                numSquares = *soundValue;
            else
                numSquares = *brightnessValue;

            int startX = optionsMenuButtonRects[i].x + (buttonWidth / 2) + padding; // Start to the right of the button

            SDL_SetRenderDrawColor(renderer, 49, 24, 130, 255);
            for (int j = 0; j < numSquares; j++)
            {
                SDL_Rect squareRect;
                squareRect.w = squareSize;
                squareRect.h = squareSize;
                squareRect.x = startX + 20 + j * (squareSize + padding);
                squareRect.y = textRect.y + (textRect.h - squareSize) / 2; // Center vertically

                SDL_RenderFillRect(renderer, &squareRect);
            }
        }
    }

    // Pillow animation
    if (optionsMenuSelectedButton >= 0 && optionsMenuSelectedButton < 3)
    {
        SDL_Rect pillowRect;
        pillowRect.w = 70;
        pillowRect.h = 70;
        pillowRect.x = optionsMenuButtonRects[optionsMenuSelectedButton].x - pillowRect.w - 10; // 10 from the left
        pillowRect.y = optionsMenuButtonRects[optionsMenuSelectedButton].y + (buttonHeight - pillowRect.h) / 2;

        // Update pillow frame
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime > lastFrameTime + GIF_FRAME_DELAY)
        {
            currentFrame = (currentFrame + 1) % GIF_FRAME_COUNT;
            lastFrameTime = currentTime;
        }

        if (pillowTextures[currentFrame])
            SDL_RenderCopy(renderer, pillowTextures[currentFrame], NULL, &pillowRect);
    }
}

void ShowGameMenuButtons(SDL_Renderer *renderer, TTF_Font *font, int *botsCount, int *difficulty, int *playersCount, char *Color1, char *Color2)
{
    SDL_Color circleColor = {255, 255, 255, 90};

    char frame_Path1[256];
    sprintf(frame_Path1, "Assets/Characters/pillow%s29.png", Color1);
    SDL_Texture *mainCharacterPillowTexture1 = IMG_LoadTexture(renderer, frame_Path1);

    char frame_Path2[256];
    sprintf(frame_Path2, "Assets/Characters/pillow%s29.png", Color2);
    SDL_Texture *mainCharacterPillowTexture2 = IMG_LoadTexture(renderer, frame_Path2);

    if (*playersCount == 2)
    {
        int circleRadius2 = 100;
        int circleX2 = SCREEN_WIDTH / 2 + 150;
        int circleY2 = (SCREEN_HEIGHT / 2) - 200;

        DrawCircle(renderer, circleX2, circleY2, circleRadius2, circleColor);

        // Main Character pillow texture 2
        if (mainCharacterPillowTexture2)
        {
            SDL_Rect pillowRect2 = {
                circleX2 - circleRadius2,
                circleY2 - circleRadius2 - 20,
                circleRadius2 * 2,
                circleRadius2 * 2};
            SDL_RenderCopy(renderer, mainCharacterPillowTexture2, NULL, &pillowRect2);
        }

        int circleRadius1 = 100;
        int circleX1 = SCREEN_WIDTH / 2 - 150;
        int circleY1 = (SCREEN_HEIGHT / 2) - 200;

        DrawCircle(renderer, circleX1, circleY1, circleRadius1, circleColor);

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
    }
    else
    {
        // Circle properties
        int circleRadius1 = 100;
        int circleX1 = SCREEN_WIDTH / 2;
        int circleY1 = (SCREEN_HEIGHT / 2) - 200;

        DrawCircle(renderer, circleX1, circleY1, circleRadius1, circleColor);

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
    }

    int totalHeight = 3 * buttonHeight + 2 * 20;          // 5 buttons and 4 spaces
    int startY = (SCREEN_HEIGHT - totalHeight) / 2 + 100; // Centered vertically

    for (int i = 0; i < 5; i++)
    {
        if (i == 0)
        {
            // AI button
            gameStartButtonRects[i].w = buttonWidth / 3 - 50;
            gameStartButtonRects[i].h = buttonHeight;
            gameStartButtonRects[i].x = (SCREEN_WIDTH - buttonWidth) / 2;
            gameStartButtonRects[i].y = startY;
        }
        else if (i == 1)
        {
            // Difficulty button
            gameStartButtonRects[i].w = buttonWidth / 2 + 100;
            gameStartButtonRects[i].h = buttonHeight;
            gameStartButtonRects[i].x = SCREEN_WIDTH / 2 - 100;
            gameStartButtonRects[i].y = startY;
        }
        else if (i == 2)
        {
            gameStartButtonRects[i].w = buttonWidth;
            gameStartButtonRects[i].h = buttonHeight;
            gameStartButtonRects[i].x = (SCREEN_WIDTH - buttonWidth) / 2;
            gameStartButtonRects[i].y = startY + buttonHeight + 20;
        }
        else if (i == 3)
        {
            gameStartButtonRects[i].w = buttonWidth;
            gameStartButtonRects[i].h = buttonHeight;
            gameStartButtonRects[i].x = (SCREEN_WIDTH - buttonWidth) / 2;
            gameStartButtonRects[i].y = startY + (2 * (buttonHeight + 20));
        }
        else
        {
            // Back button
            gameStartButtonRects[4].w = buttonWidth;
            gameStartButtonRects[4].h = buttonHeight;
            gameStartButtonRects[4].x = (SCREEN_WIDTH - buttonWidth) / 2;
            gameStartButtonRects[4].y = startY + (3 * (buttonHeight + 60));
        }
    }

    // Button text
    const char *buttonText[5] = {"AI: 1", "Difficulty: Challenge 20", "Players: 1", "Start Game", "Back"};

    for (int i = 0; i < 5; i++)
    {
        if (i == 0 || i == 1)
        {
            DrawRoundedButton(renderer, gameStartButtonRects[i], buttonColor, cornerRadius);
        }
        else
        {
            DrawRoundedButton(renderer, gameStartButtonRects[i], buttonColor, cornerRadius);
        }

        // Text
        char displayText[50];
        if (i == 0) // AI:1
        {
            sprintf(displayText, "AI: %d", *botsCount);
        }
        else if (i == 1) // Difficulty: Challenge 20
        {
            const char *difficultyStr;
            switch (*difficulty)
            {
            case 1:
                difficultyStr = "Challenge 20";
                break;
            case 2:
                difficultyStr = "Medium";
                break;
            case 3:
                difficultyStr = "Hard";
                break;
            default:
                difficultyStr = "Challenge 20";
                break;
            }
            sprintf(displayText, "Difficulty: %s", difficultyStr);
        }
        else if (i == 2) // Players: 1
            sprintf(displayText, "Players: %d", *playersCount);
        else
            strcpy(displayText, buttonText[i]);

        // Surface and texture Text
        SDL_Surface *textSurface = TTF_RenderText_Solid(font, displayText, textColor);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = {
            gameStartButtonRects[i].x + (gameStartButtonRects[i].w - textSurface->w) / 2,
            gameStartButtonRects[i].y + (gameStartButtonRects[i].h - textSurface->h) / 2,
            textSurface->w,
            textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }

    // Pillow animation
    if (gameStartSelectedButton >= 0 && gameStartSelectedButton < 5)
    {
        SDL_Rect pillowRect;
        pillowRect.w = 70;
        pillowRect.h = 70;

        if (gameStartSelectedButton == 0)
        {
            // Position the pillow to the left of the AI button with a 10 px gap
            pillowRect.x = gameStartButtonRects[0].x - pillowRect.w - 10;
            pillowRect.y = gameStartButtonRects[0].y + (buttonHeight - pillowRect.h) / 2;
        }
        else
        {
            pillowRect.x = gameStartButtonRects[gameStartSelectedButton].x + gameStartButtonRects[gameStartSelectedButton].w + 10; // 10 пикселей отступ
            pillowRect.y = gameStartButtonRects[gameStartSelectedButton].y + (buttonHeight - pillowRect.h) / 2;
        }

        // Update pillow frame
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime > lastFrameTime + GIF_FRAME_DELAY)
        {
            currentFrame = (currentFrame + 1) % GIF_FRAME_COUNT;
            lastFrameTime = currentTime;
        }

        if (pillowTextures[currentFrame])
            SDL_RenderCopy(renderer, pillowTextures[currentFrame], NULL, &pillowRect);
    }
}

void ShowRecordsButtons(SDL_Renderer *renderer, TTF_Font *font)
{
    int totalHeight = 3 * buttonHeight + 2 * 20;          // 3 buttons and 2 spaces
    int startY = (SCREEN_HEIGHT - totalHeight) / 2 + 100; // Centered vertically

    for (int i = 0; i < 2; i++)
    {
        recordsButtonRects[i].w = buttonWidth;
        recordsButtonRects[i].h = buttonHeight;
        recordsButtonRects[i].x = (SCREEN_WIDTH - buttonWidth) / 2;
        recordsButtonRects[i].y = startY + i * (buttonHeight + 20);
    }

    // Back button
    recordsButtonRects[2].w = buttonWidth;
    recordsButtonRects[2].h = buttonHeight;
    recordsButtonRects[2].x = (SCREEN_WIDTH - buttonWidth) / 2;
    recordsButtonRects[2].y = startY + 2 * (buttonHeight + 60);

    const char *optionTexts[3] = {
        "Top 1",
        "Top 10",
        "Back"};

    for (int i = 0; i < 3; i++)
    {
        DrawRoundedButton(renderer, recordsButtonRects[i], buttonColor, cornerRadius);

        // Render text
        SDL_Surface *textSurface = TTF_RenderText_Solid(font, optionTexts[i], textColor);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = {
            recordsButtonRects[i].x + (recordsButtonRects[i].w - textSurface->w) / 2,
            recordsButtonRects[i].y + (recordsButtonRects[i].h - textSurface->h) / 2,
            textSurface->w,
            textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }

    // Pillow animation
    if (recordsSelectedButton >= 0 && recordsSelectedButton < 3)
    {
        SDL_Rect pillowRect;
        pillowRect.w = 70;
        pillowRect.h = 70;
        pillowRect.x = recordsButtonRects[recordsSelectedButton].x - pillowRect.w - 10; // 10 from the left
        pillowRect.y = recordsButtonRects[recordsSelectedButton].y + (buttonHeight - pillowRect.h) / 2;

        // Update pillow frame
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime > lastFrameTime + GIF_FRAME_DELAY)
        {
            currentFrame = (currentFrame + 1) % GIF_FRAME_COUNT;
            lastFrameTime = currentTime;
        }

        if (pillowTextures[currentFrame])
            SDL_RenderCopy(renderer, pillowTextures[currentFrame], NULL, &pillowRect);
    }
}

void ShowTop1Buttons(SDL_Renderer *renderer, TTF_Font *font)
{
    int startY = (SCREEN_HEIGHT - buttonHeight) / 2 + 100;

    // Back button properties
    top1ButtonRects[0].w = buttonWidth;
    top1ButtonRects[0].h = buttonHeight;
    top1ButtonRects[0].x = (SCREEN_WIDTH - buttonWidth) / 2;
    top1ButtonRects[0].y = startY + 2 * (buttonHeight + 60);

    // Button text
    const char *buttonText[1] = {"Back"};

    for (int i = 0; i < 1; i++)
    {
        DrawRoundedButton(renderer, top1ButtonRects[i], buttonColor, cornerRadius);

        // Text
        SDL_Surface *textSurface = TTF_RenderText_Solid(font, buttonText[i], textColor);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = {
            top1ButtonRects[i].x + (top1ButtonRects[i].w - textSurface->w) / 2,
            top1ButtonRects[i].y + (top1ButtonRects[i].h - textSurface->h) / 2,
            textSurface->w,
            textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }

    // Back button pillow animation
    if (top1SelectedButton >= 0 && top1SelectedButton < 1)
    {
        SDL_Rect pillowRect;
        pillowRect.w = 70;
        pillowRect.h = 70;
        pillowRect.x = top1ButtonRects[top1SelectedButton].x - pillowRect.w - 10; // left
        pillowRect.y = top1ButtonRects[top1SelectedButton].y + (buttonHeight - pillowRect.h) / 2;

        // Update pillow
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime > lastFrameTime + GIF_FRAME_DELAY)
        {
            currentFrame = (currentFrame + 1) % GIF_FRAME_COUNT;
            lastFrameTime = currentTime;
        }

        if (pillowTextures[currentFrame])
            SDL_RenderCopy(renderer, pillowTextures[currentFrame], NULL, &pillowRect);
    }
}

void ShowTop10Buttons(SDL_Renderer *renderer, TTF_Font *font)
{
    int startY = (SCREEN_HEIGHT - buttonHeight) / 2 + 100;

    // Back button
    top10ButtonRects[0].w = buttonWidth;
    top10ButtonRects[0].h = buttonHeight;
    top10ButtonRects[0].x = (SCREEN_WIDTH - buttonWidth) / 2;
    top10ButtonRects[0].y = startY + 2 * (buttonHeight + 60);

    // Text
    const char *buttonText[1] = {"Back"};

    for (int i = 0; i < 1; i++)
    {
        DrawRoundedButton(renderer, top10ButtonRects[i], buttonColor, cornerRadius);

        // Text on the button
        SDL_Surface *textSurface = TTF_RenderText_Solid(font, buttonText[i], textColor);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = {
            top10ButtonRects[i].x + (top10ButtonRects[i].w - textSurface->w) / 2,
            top10ButtonRects[i].y + (top10ButtonRects[i].h - textSurface->h) / 2,
            textSurface->w,
            textSurface->h};
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }

    // Back button pillow animation
    if (top10SelectedButton >= 0 && top10SelectedButton < 1)
    {
        SDL_Rect pillowRect;
        pillowRect.w = 70;
        pillowRect.h = 70;
        pillowRect.x = top10ButtonRects[top10SelectedButton].x - pillowRect.w - 10; // 10 from the left
        pillowRect.y = top10ButtonRects[top10SelectedButton].y + (buttonHeight - pillowRect.h) / 2;

        // Update pillow
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime > lastFrameTime + GIF_FRAME_DELAY)
        {
            currentFrame = (currentFrame + 1) % GIF_FRAME_COUNT;
            lastFrameTime = currentTime;
        }

        if (pillowTextures[currentFrame])
            SDL_RenderCopy(renderer, pillowTextures[currentFrame], NULL, &pillowRect);
    }
}

void DrawRoundedButton(SDL_Renderer *renderer, SDL_Rect rect, SDL_Color color, int radius)
{
    // Set the drawing color
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // Draw filled rectangles for the sides
    SDL_Rect middle = {rect.x + radius, rect.y, rect.w - 2 * radius, rect.h};
    SDL_RenderFillRect(renderer, &middle);

    // Draw filled rectangles for the top and bottom
    middle.x = rect.x;
    middle.y = rect.y + radius;
    middle.w = rect.w;
    middle.h = rect.h - 2 * radius;
    SDL_RenderFillRect(renderer, &middle);

    // Draw filled circles for the corners
    // Top-left
    DrawCircle(renderer, rect.x + radius, rect.y + radius, radius, color);
    // Top-right
    DrawCircle(renderer, rect.x + rect.w - radius, rect.y + radius, radius, color);
    // Bottom-left
    DrawCircle(renderer, rect.x + radius, rect.y + rect.h - radius, radius, color);
    // Bottom-right
    DrawCircle(renderer, rect.x + rect.w - radius, rect.y + rect.h - radius, radius, color);
}