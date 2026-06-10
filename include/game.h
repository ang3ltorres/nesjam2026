#pragma once

#include <raylib.h>
#include <stddef.h>

#define TILE_SIZE   16
#define GAME_WIDTH  256
#define GAME_HEIGHT 240

#define WORLD_WIDTH  (GAME_WIDTH * 2)
#define WORLD_HEIGHT GAME_HEIGHT

#define MENU_OPTION_START 0
#define MENU_OPTION_ABOUT 1
#define MENU_OPTION_EXIT  2

extern bool gameScreen;
extern Texture2D texture;
extern Font font;
extern Camera2D camera;

// game screen
void init();
void end();

void update();
void draw();

// ui
void uiDraw();

// menu screen
void menuInit();
void menuUpdate();

void menuDraw();
void menuEnd();
