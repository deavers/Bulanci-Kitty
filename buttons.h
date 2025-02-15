#ifndef BUTTONS_H
#define BUTTONS_H

#include <SDL2/SDL_ttf.h>
#include "common.h"

#define GIF_FRAME_COUNT 85
#define GIF_FRAME_DELAY 10

extern SDL_Texture *pillowTextures[GIF_FRAME_COUNT];

extern SDL_Rect mainMenuButtonRects[4];
extern int mainMenuSelectedButton;

extern SDL_Rect optionsMenuButtonRects[3];
extern int optionsMenuSelectedButton;

extern SDL_Rect gameStartButtonRects[5];
extern int gameStartSelectedButton;

extern SDL_Rect recordsButtonRects[3];
extern int recordsSelectedButton;

extern SDL_Rect top1ButtonRects[1];
extern int top1SelectedButton;

extern SDL_Rect top10ButtonRects[1];
extern int top10SelectedButton;

void ShowMainButtons(SDL_Renderer *renderer, TTF_Font *font);
void ShowOptionsButtons(SDL_Renderer *renderer, TTF_Font *font, int *soundValue, int *brightnessValue);
void ShowGameMenuButtons(SDL_Renderer *renderer, TTF_Font *font, int *botsCount, int *difficulty, int *playersCount, char *Color1, char *Color2);
void ShowRecordsButtons(SDL_Renderer *renderer, TTF_Font *font);
void ShowTop1Buttons(SDL_Renderer *renderer, TTF_Font *font);
void ShowTop10Buttons(SDL_Renderer *renderer, TTF_Font *font);
void DrawRoundedButton(SDL_Renderer *renderer, SDL_Rect rect, SDL_Color color, int radius);

#endif