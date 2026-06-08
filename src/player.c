#include "player.h"
#include <raylib.h>
#include "terrain.h"
#include "game.h"

const float GRAVITY        = 0.085f;
const float MAX_FALL_SPEED = 2.0f;
const float JUMP_SPEED     = 2.0f;
const float MOVE_SPEED     = 0.1f;
const float FRICTION       = 0.25f;
const float MAX_WALK_SPEED = 1.0f;
const int   COYOTE_FRAMES  = 4;

Player player = {0};

static void jump()
{
  player.velY = -JUMP_SPEED;
  player.coyoteCounter = 0;
}

void playerInit()
{
  player = (Player)
  {
    .rect           = {32.0f, 32.0f, 6.0f, 15.0f},
    .dir            = 1,
    .velX           = 0.0f,
    .velY           = 0.0f,
    .coyoteCounter  = 0,
    .drill          = false,
    .drillUsed      = false,
    .drillDir       = 0,
    .drillRect = {0}
  };
}

void playerUpdate()
{
  //////////////
  // controls //
  //////////////
  bool button_left  = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);
  bool button_right = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);
  bool button_up    = IsKeyDown(KEY_UP)   || IsKeyDown(KEY_W);
  bool button_down  = IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S);

  bool button_left_pressed  = IsKeyPressed(KEY_LEFT)  || IsKeyPressed(KEY_A);
  bool button_right_pressed = IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D);
  bool button_up_pressed    = IsKeyPressed(KEY_UP)    || IsKeyPressed(KEY_W);
  bool button_down_pressed  = IsKeyPressed(KEY_DOWN)  || IsKeyPressed(KEY_S);

  bool button_a = IsKeyDown(KEY_LEFT_SHIFT);
  bool button_b = IsKeyDown(KEY_SPACE);

  bool button_a_pressed = IsKeyPressed(KEY_LEFT_SHIFT);
  bool button_b_pressed = IsKeyPressed(KEY_SPACE);

  /////////////
  // gravity //
  /////////////
  player.velY += GRAVITY;
  if (player.velY > MAX_FALL_SPEED)
    player.velY = MAX_FALL_SPEED;

  // Move vertically
  player.rect.y += player.velY;

  // Vertical collision resolution
  if (terrainCollision(player.rect).solid)
  {
    // Snap
    int tileY     = (int)((player.rect.y + player.rect.height) / TILE_SIZE);
    player.rect.y = (float)(tileY * TILE_SIZE) - player.rect.height;
    player.velY   = 0.0f;
  }

  ////////////
  // coyote //
  ////////////
  Rectangle foot = player.rect;
  foot.y += 1;
  
  if (terrainCollision(foot).solid)
    player.coyoteCounter = COYOTE_FRAMES;
  else if (player.coyoteCounter > 0)
    player.coyoteCounter--;

  // Jump with coyote time
  if (button_b_pressed && player.coyoteCounter > 0)
    jump();

  /////////////////////////
  // horizontal movement //
  /////////////////////////
  int moveDir = 0;

  // Disable movement if drill
  if (button_left)  { moveDir -= 1; player.dir = -1; }
  if (button_right) { moveDir += 1; player.dir =  1; }

  if (!player.velY && button_a)
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

  // Horizontal collision resolution
  if (terrainCollision(player.rect).solid)
  {
    if (player.velX > 0)
      player.rect.x = (float)((int)((player.rect.x + player.rect.width) / TILE_SIZE) * TILE_SIZE) - player.rect.width;
    else
      player.rect.x = (float)((int)(player.rect.x / TILE_SIZE) * TILE_SIZE + TILE_SIZE);

    player.velX = 0.0f;
  }

  ///////////
  // drill //
  ///////////
  player.drill    = button_a;
  player.drillDir = player.dir;

  // Reset drill on B press
  if (button_a_pressed)
    player.drillUsed = false;

  if (player.velY)
  {
    if (button_up)   player.drillDir = -2;
    if (button_down) player.drillDir =  2;
  }

  if (player.drill)
  {
    switch (player.drillDir)
    {
      // right
      case  1: player.drillRect = (Rectangle){player.rect.x + 6, player.rect.y +  6, 5, 4}; break;

      // left
      case -1: player.drillRect = (Rectangle){player.rect.x - 5, player.rect.y +  6, 5, 4}; break;

      // up
      case -2: player.drillRect = (Rectangle){player.rect.x + 1, player.rect.y -  5, 4, 5}; break;

      // down
      case  2: player.drillRect = (Rectangle){player.rect.x + 1, player.rect.y + 15, 4, 5}; break;
    }
  }

  // Drill-terrain collision
  if (player.drill && !player.drillUsed)
  {
    int startX = player.drillRect.x / TILE_SIZE;
    int endX   = (int)((player.drillRect.x + player.drillRect.width)  / TILE_SIZE);
    int startY = player.drillRect.y / TILE_SIZE;
    int endY   = (int)((player.drillRect.y + player.drillRect.height) / TILE_SIZE);

    for (int y = startY; y <= endY && !player.drillUsed; y++)
    for (int x = startX; x <= endX && !player.drillUsed; x++)
    {
      if (terrainTileGet(x, y) != 0)
      {
        // drill jump
        if (player.drillDir == 2)
          jump();

        terrainDamageAdd(x, y);
        player.drillUsed = true;
      }
    }
  }
}

void playerDraw()
{
  // DrawTexturePro(
  //   texture,
  //   (Rectangle){4.0f, 33.0f, 8.0f, 15.0f},
  //   (Rectangle){player.rect.x - 1.0f, player.rect.y, 8.0f, 15.0f},
  //   (Vector2){},
  //   0.0f,
  //   WHITE
  // );
  
  DrawRectangleRec(player.rect, MAGENTA);

  // Draw drill
  if (player.drill)
    DrawRectangleRec(player.drillRect, player.drillUsed ? RED : BLUE);
}
