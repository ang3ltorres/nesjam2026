#pragma once
#include <raylib.h>

#define MAX_BUNNIES 10

typedef struct Bunny
{
  Rectangle rect;
  float velX;
  float velY;
  int dir;
  int jumpCooldown;
  bool active;
  int health;
  int flashTimer;
} Bunny;

extern Bunny bunnies[MAX_BUNNIES];

void bunnyInit();
void bunnyUpdate();
void bunnyDraw();