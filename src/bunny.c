#include "bunny.h"
#include "entity.h"
#include "time_cycle.h"
#include "player.h"
#include "terrain.h"
#include "game.h"
#include <raylib.h>
#include <stdlib.h>
#include <math.h>

#define BUNNY_SIZE_X  8.0f
#define BUNNY_SIZE_Y  8.0f
#define BUNNY_MOVE_SPEED      0.04f
#define BUNNY_MAX_WALK_SPEED  0.5f
#define BUNNY_JUMP_SPEED      1.8f
#define BUNNY_CHASE_SPEED     0.08f
#define BUNNY_MAX_CHASE_SPEED 1.0f
#define BUNNY_JUMP_COOLDOWN   30
#define BUNNY_DAMAGE_COOLDOWN 20

Bunny bunnies[MAX_BUNNIES] = {0};

static void bunnySpawn(Bunny *b, int tileX, int tileY)
{
  b->rect   = (Rectangle){tileX * TILE_SIZE + 5.0f, tileY * TILE_SIZE - BUNNY_SIZE_Y, BUNNY_SIZE_X, BUNNY_SIZE_Y};
  b->velX   = 0.0f;
  b->velY   = 0.0f;
  b->dir    = 1;
  b->jumpCooldown = 0;
  b->active = true;
  b->health = 2;
  b->flashTimer = 0;
}

void bunnyInit()
{
  for (int i = 0; i < MAX_BUNNIES; i++)
    bunnies[i].active = false;

  // Spawn a few bunnies on terrain surface
  bunnySpawn(&bunnies[0], 4, 0);
  bunnySpawn(&bunnies[1], 8, 0);
  bunnySpawn(&bunnies[2], 12, 0);
}

void bunnyUpdate()
{
  bool hostile = !isDay();

  for (int i = 0; i < MAX_BUNNIES; i++)
  {
    Bunny *b = &bunnies[i];
    if (!b->active)
      continue;

    // Decrement flash timer
    if (b->flashTimer > 0)
      b->flashTimer--;

    // Gravity
    entityApplyGravity(&b->velY);
    b->rect.y += b->velY;
    entityResolveVerticalCollision(&b->rect, &b->velY);

    // Check if on floor
    bool onFloor = entityHasFloor(b->rect);

    if (b->jumpCooldown > 0)
      b->jumpCooldown--;

    if (hostile)
    {
      // ---- HOSTILE (night) ----
      // Move toward player
      float dx = player.rect.x - b->rect.x;
      b->dir = (dx > 0) ? 1 : -1;

      b->velX += b->dir * BUNNY_CHASE_SPEED;
      if (b->velX > BUNNY_MAX_CHASE_SPEED)
        b->velX = BUNNY_MAX_CHASE_SPEED;
      else if (b->velX < -BUNNY_MAX_CHASE_SPEED)
        b->velX = -BUNNY_MAX_CHASE_SPEED;

      // Jump toward player if on floor and cooldown ready
      if (onFloor && b->jumpCooldown == 0)
      {
        b->velY = -BUNNY_JUMP_SPEED;
        b->jumpCooldown = BUNNY_JUMP_COOLDOWN;
      }

      // Check collision with player
      if (CheckCollisionRecs(b->rect, player.rect))
      {
        // Player drills bunny while falling → bunny takes damage
        if (player.drill && player.velY > 0 && b->flashTimer == 0)
        {
          b->health--;
          b->flashTimer = 1;

          // Player bounces upward with 1.25x force
          player.velY = -(player.velY * 1.25f);
          player.coyoteCounter = 0;

          if (b->health <= 0)
            b->active = false;
        }
        else
        {
          playerTakeDamage();
        }
      }
    }
    else
    {
      // ---- PEACEFUL (day) ----
      // Bounce around: random direction changes
      if (onFloor)
      {
        if (GetRandomValue(0, 60) == 0)
          b->dir = -b->dir;
      }

      b->velX += b->dir * BUNNY_MOVE_SPEED;
      if (b->velX > BUNNY_MAX_WALK_SPEED)
        b->velX = BUNNY_MAX_WALK_SPEED;
      else if (b->velX < -BUNNY_MAX_WALK_SPEED)
        b->velX = -BUNNY_MAX_WALK_SPEED;

      // Random jump
      if (onFloor && b->jumpCooldown == 0 && GetRandomValue(0, 90) == 0)
      {
        b->velY = -BUNNY_JUMP_SPEED;
        b->jumpCooldown = BUNNY_JUMP_COOLDOWN;
      }
    }

    // Move horizontally
    b->rect.x += b->velX;
    entityResolveHorizontalCollision(&b->rect, &b->velX);

    // Bounce off walls (reverse direction)
    if (b->velX == 0.0f)
      b->dir = -b->dir;

    // Apply friction
    b->velX *= (1.0f - 0.2f);
  }
}

void bunnyDraw()
{
  for (int i = 0; i < MAX_BUNNIES; i++)
  {
    if (!bunnies[i].active)
      continue;

    Color color;
    if (bunnies[i].flashTimer > 0)
      color = WHITE;
    else
      color = isDay() ? WHITE : RED;

    DrawRectangleRec(bunnies[i].rect, color);
  }
}