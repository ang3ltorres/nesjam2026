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

  int health;
  float invincibilityTimer;
} Player;

extern Player player;

void playerInit();
void playerUpdate();
void playerDraw();
void playerTakeDamage();

