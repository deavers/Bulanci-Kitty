#ifndef MUSIC_H
#define MUSIC_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

extern Mix_Music *gameMusic[3];
extern Mix_Music *mainMenuMusic;
extern Mix_Music *currentMusic;
extern Mix_Chunk *MainMenuFx;

void LoadMusic();
void PlayMusic(Mix_Music *music);
void PlayMusicChunk(Mix_Chunk *sound);
void StopMusic();
void FreeMusic();

#endif