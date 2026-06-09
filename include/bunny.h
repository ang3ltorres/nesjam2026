#pragma once
#include <raylib.h>

#define MAX_BUNNIES 10

typedef enum {
  BUNNY_IDLE = 0,
  BUNNY_ALERT,
  BUNNY_CHASE,
  BUNNY_CONFUSED
} BunnyState;

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
  int state;
  int stateTimer;
} Bunny;

extern Bunny bunnies[MAX_BUNNIES];

void bunnyInit();
void bunnyUpdate();
void bunnyDraw();