#include "music.h"
#include <SDL2/SDL_mixer.h>

Mix_Music *gameMusic[3] = {NULL, NULL, NULL};
Mix_Music *mainMenuMusic = NULL;
Mix_Music *currentMusic = NULL;
Mix_Chunk *MainMenuFx = NULL;

void LoadMusic()
{
    gameMusic[0] = Mix_LoadMUS("Assets/Sounds/Rebel_Soul_1.wav");
    gameMusic[1] = Mix_LoadMUS("Assets/Sounds/Rebel_Soul_2.wav");
    gameMusic[2] = Mix_LoadMUS("Assets/Sounds/Rebel_Soul_3.wav");
    mainMenuMusic = Mix_LoadMUS("Assets/Sounds/MainMenuBackground.wav");
    MainMenuFx = Mix_LoadWAV("Assets/Sounds/MainMenuFx.wav");
}

void PlayMusic(Mix_Music *music)
{
    if (currentMusic != music)
    {
        currentMusic = music;
        if (Mix_PlayMusic(currentMusic, -1) == -1)
        {
            printf("Error playing music: %s\n", Mix_GetError());
        }
    }
}

void PlayMusicChunk(Mix_Chunk *sound)
{
    if (sound == NULL)
    {
        printf("Sound effect is NULL!\n");
        return;
    }

    if (Mix_PlayChannel(-1, sound, 0) == -1)
    {
        // printf("Failed to play hover sound! SDL_mixer Error!\n");
    }
}

void StopMusic()
{
    Mix_HaltMusic();
    currentMusic = NULL;
}

void FreeMusic()
{
    for (int i = 0; i < 3; i++)
    {
        if (gameMusic[i])
        {
            Mix_FreeMusic(gameMusic[i]);
            gameMusic[i] = NULL;
        }
    }
    if (mainMenuMusic)
    {
        Mix_FreeMusic(mainMenuMusic);
        mainMenuMusic = NULL;
    }
    if (MainMenuFx)
    {
        Mix_FreeChunk(MainMenuFx);
        MainMenuFx = NULL;
    }
}