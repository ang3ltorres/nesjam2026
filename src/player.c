#include "player.h"
#include <raylib.h>
#include "terrain.h"
#include "game.h"

const float GRAVITY        = 0.06f;
const float MAX_FALL_SPEED = 2.0f;
const float JUMP_SPEED     = 2.0f;
const int   COYOTE_FRAMES  = 4;

Player player = {0};

void playerInit()
{
  player = (Player)
  {
    .rect          = {32.0f, 32.0f, 6.0f, 15.0f},
    .velY          = 0.0f,
    .coyoteCounter = 0
  };
}

void playerUpdate()
{
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

  // Apply gravity
  player.velY += GRAVITY;
  if (player.velY > MAX_FALL_SPEED)
    player.velY = MAX_FALL_SPEED;

  // Move vertically
  player.rect.y += player.velY;

  // Collision resolution
  if (terrainCollision(player.rect).solid)
  {
    // Snap
    int tileY     = (int)((player.rect.y + player.rect.height) / TILE_SIZE);
    player.rect.y = (float)(tileY * TILE_SIZE) - player.rect.height;
    player.velY   = 0.0f;
  }
}

void playerDraw()
{
  
  DrawRectangleRec(player.rect, MAGENTA);
  DrawTexturePro(
    texture,
    (Rectangle){4.0f, 33.0f, 8.0f, 15.0f},
    (Rectangle){player.rect.x - 1.0f, player.rect.y, 8.0f, 15.0f},
    (Vector2){},
    0.0f,
    WHITE
  );

}
