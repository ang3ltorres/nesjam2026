#pragma once

#include <raylib.h>

typedef struct Player
{
  Rectangle rect;
  float velX;
  float velY;
  int coyoteCounter;
} Player;

extern Player player;

void playerInit();
void playerUpdate();
void playerDraw();

