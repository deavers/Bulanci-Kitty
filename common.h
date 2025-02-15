#ifndef COMMON_H
#define COMMON_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#define TEXTURE_SIZE 64
#define CHARACTER_SIZE 54

#define PILLOW_ANIMATION_FRAMES 8
#define PILLOW_ANIMATION_FRAMES_DEAD 4

#define MAX_BULLETS 200
#define WEAPON_COUNT 3

#define ITEM_COUNT 3

#define MAP_WIDTH 30  // 1920 / 64 = 30
#define MAP_HEIGHT 17 // 1080 / 64 = 17

#define NUM_CHARACTER_TEXTURES 100

#define GIF_FRAME_COUNT 85 // 85 frames in the gif
#define GIF_FRAME_DELAY 10 // 10 ms per frame
#define GIF_FRAME_RELOAD_COUNT 27

#define TOTAL_GAME_TIME 60000

#define MAX_BOT_NAMES 50
#define MAX_NAME_LENGTH 50

#define NUM_BOT_DIRECTION 4

#define buttonWidth 600
#define buttonHeight 60
#define cornerRadius 30

#define NUM_BOT_DIRECTIONS 4
#define BOT_ANIMATION_FRAMES 91

#define NUM_COLORS 6
#define MAX_BOTS 15

typedef enum
{
    DIRECTION_UP,
    DIRECTION_UP_RIGHT,
    DIRECTION_UP_LEFT,
    DIRECTION_DOWN,
    DIRECTION_DOWN_RIGHT,
    DIRECTION_DOWN_LEFT,
    DIRECTION_LEFT,
    DIRECTION_RIGHT
} Direction;

typedef enum
{
    STATUS_IDLE,
    STATUS_WALK
} Status;

typedef enum
{
    BULLET_PLAYER,
    BULLET_PLAYER2,
    BULLET_BOT
} BulletOwner;

typedef struct
{
    int x;
    int y;
    int velX;
    int velY;
    SDL_Texture *texture;
    bool active;
    BulletOwner owner;
    int idBot;
} Bullet;

typedef enum
{
    WEAPON_PISTOL,
    WEAPON_RIFLE,
    WEAPON_SHOTGUN,
} WeaponType;

typedef enum
{
    WEAPON_PISTOL_LOCKED,
    WEAPON_RIFLE_LOCKED,
    WEAPON_SHOTGUN_LOCKED
} WeaponLockedType;

typedef struct
{
    char name[50];
    int kills;
} Leaderboard;

typedef struct
{
    WeaponType type;
    SDL_Rect rect;
    bool active;
    Uint32 spawnTime;
} Weapon;

typedef struct
{
    int botCount;
    int botDifficulty;
    int brightnessValue;
    int soundValue;
} Settings;

typedef enum
{
    STATE_MAIN_MENU,
    STATE_GAME_MENU,
    STATE_PLAY,
    STATE_OPTIONS,
    STATE_RECORDS,
    STATE_TOP_1,
    STATE_TOP_10,
    STATE_GAME_OVER,
    STATE_QUIT
} GameStatus;

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#endif