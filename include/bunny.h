#pragma once
#include <raylib.h>

#define MAX_BUNNIES 32
#define BUNNY_SIZE_X  8.0f
#define BUNNY_SIZE_Y  8.0f
#define BUNNY_MOVE_SPEED      0.04f
#define BUNNY_MAX_WALK_SPEED  0.5f
#define BUNNY_JUMP_SPEED      1.8f
#define BUNNY_CHASE_SPEED     0.08f
#define BUNNY_MAX_CHASE_SPEED 1.0f
#define BUNNY_JUMP_COOLDOWN   30
#define BUNNY_DAMAGE_COOLDOWN 20
#define BUNNY_DETECT_RADIUS   80.0f
#define BUNNY_CONFUSED_DURATION   45
#define BUNNY_ALERT_DURATION       12
#define BUNNY_INVINCIBILITY_DURATION 1.0f
#define BUNNY_FLASH_INTERVAL        0.1f

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
  int health;
  float invincibilityTimer;
  int state;
  int stateTimer;
} Bunny;

extern int bunniesCount;
extern Bunny bunnies[MAX_BUNNIES];

void bunnyInit();
void bunnyUpdate();
void bunnyDraw();