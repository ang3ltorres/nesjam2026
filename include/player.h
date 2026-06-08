#pragma once

#include <raylib.h>

typedef struct Player
{
  Rectangle rect;
  int dir;
  float velX;
  float velY;
  int coyoteCounter;
  
  bool drill;
  bool drillUsed;
  int  drillDir;
  Rectangle drillRect;
} Player;

extern Player player;

void playerInit();
void playerUpdate();
void playerDraw();

