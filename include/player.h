#pragma once

#include <raylib.h>

#define PLAYER_JUMP_SPEED           2.0f
#define PLAYER_MOVE_SPEED           0.1f
#define PLAYER_FRICTION             0.25f
#define PLAYER_MAX_WALK_SPEED       1.0f
#define PLAYER_COYOTE_FRAMES            4
#define PLAYER_MAX_HEALTH               5
#define PLAYER_INVINCIBILITY_DURATION 2.0f
#define PLAYER_FLASH_INTERVAL        0.1f

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
void playerJump();
void playerBounce(float force);
void playerTakeDamage();

