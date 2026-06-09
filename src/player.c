#include "player.h"
#include "control.h"
#include <raylib.h>
#include <string.h>

#include <stdlib.h>
#include "entity.h"
#include "terrain.h"
#include "game.h"

const float JUMP_SPEED     = 2.0f;
const float MOVE_SPEED     = 0.1f;
const float FRICTION       = 0.25f;
const float MAX_WALK_SPEED = 1.0f;
const int   COYOTE_FRAMES  = 4;

const int   PLAYER_MAX_HEALTH      = 5;
const float INVINCIBILITY_DURATION = 2.0f;
const float FLASH_INTERVAL         = 0.1f;
const float DOUBLE_TAP_TIME        = 0.3f;

Player player = {0};

static void jump()
{
  player.velY = -JUMP_SPEED;
  player.coyoteCounter = 0;
}

void playerTakeDamage()
{
  if (player.invincibilityTimer > 0.0f)
    return;

  player.health--;
  player.invincibilityTimer = INVINCIBILITY_DURATION;

  if (player.health <= 0)
    playerInit();
}

void playerInit()
{
  player = (Player)
  {
    .rect               = {32.0f, 32.0f + 32.0f, 6.0f, 15.0f},
    .dir                = 1,
    .velX               = 0.0f,
    .velY               = 0.0f,
    .coyoteCounter      = 0,
    .drill              = false,
    .drillUsed          = false,
    .drillDir           = 0,
    .drillRect          = {0},
    .lastTapTime        = {-100.0f, -100.0f, -100.0f, -100.0f},
    .quickDrillDir      = 0,
    .health             = PLAYER_MAX_HEALTH,
    .invincibilityTimer = 0.0f,
  };
}

void playerUpdate()
{

  /////////////////////////
  // invincibility timer //
  /////////////////////////
  if (player.invincibilityTimer > 0.0f)
    player.invincibilityTimer -= GetFrameTime();

  /////////////
  // gravity //
  /////////////
  entityApplyGravity(&player.velY);
  player.rect.y += player.velY;
  entityResolveVerticalCollision(&player.rect, &player.velY);

  ////////////
  // coyote //
  ////////////
  if (entityHasFloor(player.rect))
    player.coyoteCounter = COYOTE_FRAMES;
  else if (player.coyoteCounter > 0)
    player.coyoteCounter--;

  // Jump with coyote time
  if (control.b[1] && player.coyoteCounter > 0)
    jump();

  /////////////////////////
  // horizontal movement //
  /////////////////////////
  int moveDir = 0;

  // Disable movement if drill
  if (control.left[0])  { moveDir -= 1; player.dir = -1; }
  if (control.right[0]) { moveDir += 1; player.dir =  1; }

  if (!player.velY && control.a[0])
    moveDir = 0.0f;

  // If not moving, apply friction
  if (moveDir != 0)
    player.velX += moveDir * MOVE_SPEED;
  else
    player.velX *= (1.0f - FRICTION);

  // Clamp velX
  if (player.velX > MAX_WALK_SPEED)
    player.velX = MAX_WALK_SPEED;
  else if (player.velX < -MAX_WALK_SPEED)
    player.velX = -MAX_WALK_SPEED;

  // Move horizontally
  player.rect.x += player.velX;
  entityResolveHorizontalCollision(&player.rect, &player.velX);

  ///////////////////////////
  // quick-drill detection //
  ///////////////////////////
  float time = GetTime();

  if (control.left[1])
  {
    if (time - player.lastTapTime[0] < DOUBLE_TAP_TIME)
    {
      player.quickDrillDir = -1;
      player.drillUsed = false;
    }

    memset(player.lastTapTime, 0, sizeof(float) * 4);
    player.lastTapTime[0] = time;
  }
  if (control.right[1])
  {
    if (time - player.lastTapTime[1] < DOUBLE_TAP_TIME)
    {
      player.quickDrillDir = 1;
      player.drillUsed = false;
    }

    memset(player.lastTapTime, 0, sizeof(float) * 4);
    player.lastTapTime[1] = time;
  }
  if (control.up[1])
  {
    if (time - player.lastTapTime[2] < DOUBLE_TAP_TIME)
    {
      player.quickDrillDir = -2;
      player.drillUsed = false;
    }

    memset(player.lastTapTime, 0, sizeof(float) * 4);
    player.lastTapTime[2] = time;
  }
  if (control.down[1])
  {
    if (time - player.lastTapTime[3] < DOUBLE_TAP_TIME)
    {
      player.quickDrillDir = 2;
      player.drillUsed = false;
    }

    memset(player.lastTapTime, 0, sizeof(float) * 4);
    player.lastTapTime[3] = time;
  }

  // Cancel quick-drill if held key is released
  if (player.quickDrillDir != 0)
  {
    bool held = false;
    switch (player.quickDrillDir)
    {
      case -1: held = control.left[0];  break;
      case  1: held = control.right[0]; break;
      case -2: held = control.up[0];    break;
      case  2: held = control.down[0];  break;
    }
    if (!held)
      player.quickDrillDir = 0;
  }

  bool quickDrillActive = player.quickDrillDir != 0;

  ///////////
  // drill //
  ///////////
  player.drill = control.a[0] || quickDrillActive;

  if (quickDrillActive)
    player.drillDir = player.quickDrillDir;
  else if (control.a[0])
    player.drillDir = player.dir;

  // Re-arm drill
  if (control.a[1])
    player.drillUsed = false;

  if (control.up[0])   player.drillDir = -2;
  if (control.down[0]) player.drillDir =  2;

  if (player.drill)
  {
    switch (player.drillDir)
    {
      case  1: player.drillRect = (Rectangle){player.rect.x + 6, player.rect.y +  6, 5, 4}; break;
      case -1: player.drillRect = (Rectangle){player.rect.x - 5, player.rect.y +  6, 5, 4}; break;
      case -2: player.drillRect = (Rectangle){player.rect.x + 1, player.rect.y -  5, 4, 5}; break;
      case  2: player.drillRect = (Rectangle){player.rect.x + 1, player.rect.y + 15, 4, 5}; break;
    }
  }

  // Drill-terrain collision (same for both drill types)
  if (player.drill && !player.drillUsed)
  {
    int startX = player.drillRect.x / TILE_SIZE;
    int endX   = (int)((player.drillRect.x + player.drillRect.width)  / TILE_SIZE);
    int startY = player.drillRect.y / TILE_SIZE;
    int endY   = (int)((player.drillRect.y + player.drillRect.height) / TILE_SIZE);

    for (int y = startY; y <= endY; y++)
    for (int x = startX; x <= endX; x++)
    {
      unsigned char tile = terrainTileGet(x, y);

      if (tile != 0 && tile != 4)
      {
        if (player.drillDir == 2)
        {
          player.velY = -(player.velY * 1.14f);
          player.coyoteCounter = 0;
        }

        if (player.drillDir == -2)
          player.velY = JUMP_SPEED;

        terrainDamageAdd(x, y);
        player.drillUsed = true;

        unsigned char damage = terrainDamageGet(x, y);

        if (damage >= tile + 1)
        {
          int idx = y * TERRAIN_TILES_X + x;
          terrain[idx].tile   = 0;
          terrain[idx].damage = 0;
        }
        break;
      }
    }
  }
}

void playerDraw()
{
  // Flash when invincible
  if (player.invincibilityTimer > 0.0f)
  {
    int flashPhase = (int)(player.invincibilityTimer / FLASH_INTERVAL);
    if (flashPhase % 2 == 0)
      return;
  }
  
  DrawRectangleRec(player.rect, RED);

  // Draw drill
  if (player.drill)
    DrawRectangleRec(player.drillRect, player.drillUsed ? RED : BLUE);
}
