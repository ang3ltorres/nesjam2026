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

void playerInit()
{
  player = (Player)
  {
    .rect          = {32.0f, 32.0f, 6.0f, 15.0f},
    .velX          = 0.0f,
    .velY          = 0.0f,
    .coyoteCounter = 0
  };
}

void playerUpdate()
{
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
  if (IsKeyPressed(KEY_SPACE) && player.coyoteCounter > 0)
  {
    player.velY = -JUMP_SPEED;
    player.coyoteCounter = 0;
  }

  //////////////
  // controls //
  //////////////
  int moveDir = 0;
  if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    moveDir -= 1;
  if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    moveDir += 1;

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
}

void playerDraw()
{
  DrawTexturePro(
    texture,
    (Rectangle){4.0f, 33.0f, 8.0f, 15.0f},
    (Rectangle){player.rect.x - 1.0f, player.rect.y, 8.0f, 15.0f},
    (Vector2){},
    0.0f,
    WHITE
  );
  
  DrawRectangleRec(player.rect, MAGENTA);
}
