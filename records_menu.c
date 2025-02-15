#include <SDL2/SDL.h>
#include "options_menu.h"
#include "buttons.h"
#include "init.h"
#include "music.h"

extern Mix_Chunk *MainMenuFx;

void RecordsEvents(SDL_Event *event, GameStatus *status)
{
    static int previousHoveredButton = -1;

    if (event->type == SDL_MOUSEMOTION)
    {
        int mouseX = event->motion.x;
        int mouseY = event->motion.y;
        int buttonHovered = -1;

        for (int i = 0; i < 3; i++)
        {
            if (mouseX >= recordsButtonRects[i].x && mouseX <= recordsButtonRects[i].x + recordsButtonRects[i].w &&
                mouseY >= recordsButtonRects[i].y && mouseY <= recordsButtonRects[i].y + recordsButtonRects[i].h)
            {
                recordsSelectedButton = i;
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
                if (mouseX >= recordsButtonRects[i].x && mouseX <= recordsButtonRects[i].x + recordsButtonRects[i].w &&
                    mouseY >= recordsButtonRects[i].y && mouseY <= recordsButtonRects[i].y + recordsButtonRects[i].h)
                {
                    recordsSelectedButton = i;
                    switch (i)
                    {
                    case 0: // Top 1
                        *status = STATE_TOP_1;
                        break;
                    case 1: // Top 10
                        *status = STATE_TOP_10;
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
            recordsSelectedButton = (recordsSelectedButton + 1) % 3;
            PlayMusicChunk(MainMenuFx);
            break;
        case SDLK_UP:
            recordsSelectedButton = (recordsSelectedButton + 2) % 3;
            PlayMusicChunk(MainMenuFx);
            break;
        case SDLK_RETURN: // Enter
            switch (recordsSelectedButton)
            {
            case 0: // Top 1
                *status = STATE_TOP_1;
                break;
            case 1: // Top 10
                *status = STATE_TOP_10;
                break;
            case 2: // Back
                *status = STATE_MAIN_MENU;
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

void Top1Events(SDL_Event *event, GameStatus *status)
{
    static int previousHoveredButton = -1;

    if (event->type == SDL_MOUSEMOTION)
    {
        int mouseX = event->motion.x;
        int mouseY = event->motion.y;
        int buttonHovered = -1;

        // If mouse is over the "Back" button
        if (mouseX >= top1ButtonRects[0].x && mouseX <= top1ButtonRects[0].x + top1ButtonRects[0].w &&
            mouseY >= top1ButtonRects[0].y && mouseY <= top1ButtonRects[0].y + top1ButtonRects[0].h)
        {
            top1SelectedButton = 0;
            buttonHovered = 0;
        }

        if (!buttonHovered)
            top1SelectedButton = 0;

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

            // if "Back" button is clicked
            if (mouseX >= top1ButtonRects[0].x && mouseX <= top1ButtonRects[0].x + top1ButtonRects[0].w &&
                mouseY >= top1ButtonRects[0].y && mouseY <= top1ButtonRects[0].y + top1ButtonRects[0].h)
            {
                *status = STATE_RECORDS;
            }
        }
    }
    else if (event->type == SDL_KEYDOWN)
    {
        switch (event->key.keysym.sym)
        {
        case SDLK_BACKSPACE:
        case SDLK_RETURN:
        case SDLK_ESCAPE:
            *status = STATE_RECORDS;
            PlayMusicChunk(MainMenuFx);
            break;
        default:
            break;
        }
    }
}

void Top10Events(SDL_Event *event, GameStatus *status)
{
    static int previousHoveredButton = -1;

    if (event->type == SDL_MOUSEMOTION)
    {
        int mouseX = event->motion.x;
        int mouseY = event->motion.y;
        int buttonHovered = -1;

        // if mouse is over the "Back" button
        if (mouseX >= top10ButtonRects[0].x && mouseX <= top10ButtonRects[0].x + top10ButtonRects[0].w &&
            mouseY >= top10ButtonRects[0].y && mouseY <= top10ButtonRects[0].y + top10ButtonRects[0].h)
        {
            top10SelectedButton = 0;
            buttonHovered = 0;
        }

        if (!buttonHovered)
            top10SelectedButton = 0;

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

            // if "Back" button is clicked
            if (mouseX >= top10ButtonRects[0].x && mouseX <= top10ButtonRects[0].x + top10ButtonRects[0].w &&
                mouseY >= top10ButtonRects[0].y && mouseY <= top10ButtonRects[0].y + top10ButtonRects[0].h)
            {
                *status = STATE_RECORDS;
            }
        }
    }
    else if (event->type == SDL_KEYDOWN)
    {
        switch (event->key.keysym.sym)
        {
        case SDLK_BACKSPACE:
        case SDLK_RETURN:
        case SDLK_ESCAPE:
            *status = STATE_RECORDS;
            PlayMusicChunk(MainMenuFx);
            break;
        default:
            break;
        }
    }
}