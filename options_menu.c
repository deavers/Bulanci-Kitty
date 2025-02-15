#include <SDL2/SDL.h>
#include "options_menu.h"
#include "buttons.h"
#include "init.h"
#include "music.h"

extern Mix_Chunk *MainMenuFx;

extern Settings settings;

void OptionsMenuEvents(SDL_Renderer *renderer, SDL_Event *event, GameStatus *status)
{
    static int previousHoveredButton = -1;

    if (event->type == SDL_MOUSEMOTION)
    {
        int mouseX = event->motion.x;
        int mouseY = event->motion.y;
        int buttonHovered = -1;

        for (int i = 0; i < 3; i++)
        {
            if (mouseX >= optionsMenuButtonRects[i].x && mouseX <= optionsMenuButtonRects[i].x + optionsMenuButtonRects[i].w &&
                mouseY >= optionsMenuButtonRects[i].y && mouseY <= optionsMenuButtonRects[i].y + optionsMenuButtonRects[i].h)
            {
                optionsMenuSelectedButton = i;
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
    else if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            int mouseX = event->button.x;
            int mouseY = event->button.y;

            for (int i = 0; i < 3; i++)
            {
                if (mouseX >= optionsMenuButtonRects[i].x && mouseX <= optionsMenuButtonRects[i].x + optionsMenuButtonRects[i].w &&
                    mouseY >= optionsMenuButtonRects[i].y && mouseY <= optionsMenuButtonRects[i].y + optionsMenuButtonRects[i].h)
                {
                    optionsMenuSelectedButton = i;
                    switch (i)
                    {
                    case 0: // Sound
                        break;
                    case 1: // Brightness
                        break;
                    case 2: // Back
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
    else if (event->type == SDL_KEYDOWN)
    {
        switch (event->key.keysym.sym)
        {
        case SDLK_DOWN:
            optionsMenuSelectedButton = (optionsMenuSelectedButton + 1) % 3;
            PlayMusicChunk(MainMenuFx);
            break;
        case SDLK_UP:
            optionsMenuSelectedButton = (optionsMenuSelectedButton + 2) % 3;
            PlayMusicChunk(MainMenuFx);
            break;
        case SDLK_RETURN:
        case SDLK_SPACE:
            switch (optionsMenuSelectedButton)
            {
            case 0: // Sound
                if (settings.soundValue < 10)
                    settings.soundValue++;
                break;
            case 1: // Brightness
                if (settings.brightnessValue < 10)
                    settings.brightnessValue++;
                break;
            case 2: // Back
                *status = STATE_MAIN_MENU;
                break;
            default:
                break;
            }
            PlayMusicChunk(MainMenuFx);
            break;
        case SDLK_LEFT:
            if (optionsMenuSelectedButton == 0 && settings.soundValue > 0)
            {
                settings.soundValue--;
                UpdateSound();
            }
            else if (optionsMenuSelectedButton == 1 && settings.brightnessValue > 0)
            {
                settings.brightnessValue--;
                UpdateBrightness(renderer);
            }
            PlayMusicChunk(MainMenuFx);
            break;
        case SDLK_RIGHT:
            if (optionsMenuSelectedButton == 0 && settings.soundValue < 10)
            {
                settings.soundValue++;
                UpdateSound();
            }
            else if (optionsMenuSelectedButton == 1 && settings.brightnessValue < 10)
            {
                settings.brightnessValue++;
                UpdateBrightness(renderer);
            }
            PlayMusicChunk(MainMenuFx);
            break;
        default:
            break;
        }
    }
}

void UpdateBrightness(SDL_Renderer *renderer)
{
    // Backround black color
    if (settings.brightnessValue < 0)
        settings.brightnessValue = 0;
    if (settings.brightnessValue > 10)
        settings.brightnessValue = 10;

    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, (10 - settings.brightnessValue) * 25);
    SDL_Rect fullScreen = {
        0, 0,
        width, height};
    SDL_RenderFillRect(renderer, &fullScreen);
}

void UpdateSound()
{
    if (settings.soundValue < 0)
        settings.soundValue = 0;
    if (settings.soundValue > 10)
        settings.soundValue = 10;

    Mix_Volume(-1, settings.soundValue * 12.8);

    Mix_VolumeMusic(settings.soundValue * 12.8);
}