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
#define BUNNY_DETECT_RADIUS   80.0f
#define BUNNY_CONFUSED_DURATION 45
#define BUNNY_ALERT_DURATION    12

Bunny bunnies[MAX_BUNNIES] = {0};

static bool bunnyCanSeePlayer(const Bunny *b)
{
  float cx = b->rect.x + b->rect.width  / 2.0f;
  float cy = b->rect.y + b->rect.height / 2.0f;
  float px = player.rect.x + player.rect.width  / 2.0f;
  float py = player.rect.y + player.rect.height / 2.0f;

  float dx = px - cx;
  float dy = py - cy;
  float dist = sqrtf(dx * dx + dy * dy);

  if (dist > BUNNY_DETECT_RADIUS)
    return false;

  // Ray-march from bunny to player checking for solid terrain
  int steps = (int)(dist / 4.0f);
  if (steps < 1) steps = 1;
  float stepX = dx / steps;
  float stepY = dy / steps;

  for (int i = 0; i < steps; i++)
  {
    cx += stepX;
    cy += stepY;
    int tileX = (int)(cx / TILE_SIZE);
    int tileY = (int)(cy / TILE_SIZE);
    if (terrainTileGet(tileX, tileY) != 0)
      return false;
  }

  return true;
}

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
  b->state = BUNNY_IDLE;
  b->stateTimer = 0;
}

void bunnyInit()
{
  for (int i = 0; i < MAX_BUNNIES; i++)
    bunnies[i].active = false;

  // Spawn a few bunnies across the wider level
  bunnySpawn(&bunnies[0], 5, 0);
  bunnySpawn(&bunnies[1], 14, 0);
  bunnySpawn(&bunnies[2], 22, 0);
}

void bunnyUpdate()
{
  bool hostile = !isDay();

  for (int i = 0; i < MAX_BUNNIES; i++)
  {
    Bunny *b = &bunnies[i];
    if (!b->active)
      continue;

    if (b->flashTimer > 0)  b->flashTimer--;
    if (b->jumpCooldown > 0) b->jumpCooldown--;
    if (b->stateTimer > 0)   b->stateTimer--;

    // Gravity
    entityApplyGravity(&b->velY);
    b->rect.y += b->velY;
    entityResolveVerticalCollision(&b->rect, &b->velY);

    bool onFloor = entityHasFloor(b->rect);

    if (!hostile)
    {
      // ---- DAYTIME: peaceful idle ----
      b->state = BUNNY_IDLE;

      if (onFloor && GetRandomValue(0, 60) == 0)
        b->dir = -b->dir;

      b->velX += b->dir * BUNNY_MOVE_SPEED;
      if (b->velX > BUNNY_MAX_WALK_SPEED)
        b->velX = BUNNY_MAX_WALK_SPEED;
      else if (b->velX < -BUNNY_MAX_WALK_SPEED)
        b->velX = -BUNNY_MAX_WALK_SPEED;

      if (onFloor && b->jumpCooldown == 0 && GetRandomValue(0, 90) == 0)
      {
        b->velY = -BUNNY_JUMP_SPEED;
        b->jumpCooldown = BUNNY_JUMP_COOLDOWN;
      }
    }
    else
    {
      // ---- NIGHT: state-driven behaviour ----
      bool canSee = bunnyCanSeePlayer(b);

      // --- State transitions ---
      if (canSee)
      {
        if (b->state == BUNNY_IDLE || b->state == BUNNY_CONFUSED)
        {
          b->state = BUNNY_ALERT;
          b->stateTimer = BUNNY_ALERT_DURATION;
        }
      }
      else
      {
        if (b->state == BUNNY_CHASE || b->state == BUNNY_ALERT)
        {
          b->state = BUNNY_CONFUSED;
          b->stateTimer = BUNNY_CONFUSED_DURATION;
        }
      }

      // --- State timeouts ---
      if (b->stateTimer == 0)
      {
        if (b->state == BUNNY_ALERT)
          b->state = BUNNY_CHASE;
        else if (b->state == BUNNY_CONFUSED)
          b->state = BUNNY_IDLE;
      }

      // --- Per-state movement ---
      switch (b->state)
      {
        case BUNNY_ALERT:
          b->velX *= 0.8f;
          if (onFloor)
          {
            b->velY = -BUNNY_JUMP_SPEED * 1.2f;
            b->jumpCooldown = BUNNY_JUMP_COOLDOWN;
          }
          break;

        case BUNNY_CHASE:
        {
          float dx = player.rect.x - b->rect.x;
          b->dir = (dx > 0) ? 1 : -1;
          b->velX += b->dir * BUNNY_CHASE_SPEED;
          if (b->velX > BUNNY_MAX_CHASE_SPEED)
            b->velX = BUNNY_MAX_CHASE_SPEED;
          else if (b->velX < -BUNNY_MAX_CHASE_SPEED)
            b->velX = -BUNNY_MAX_CHASE_SPEED;

          if (onFloor && b->jumpCooldown == 0)
          {
            b->velY = -BUNNY_JUMP_SPEED;
            b->jumpCooldown = BUNNY_JUMP_COOLDOWN;
          }
          break;
        }

        case BUNNY_CONFUSED:
          b->velX *= 0.85f;
          if (onFloor && GetRandomValue(0, 15) == 0)
          {
            b->dir = -b->dir;
            b->velX += b->dir * BUNNY_MOVE_SPEED * 2.0f;
            b->velY = -BUNNY_JUMP_SPEED * 0.6f;
          }
          break;

        case BUNNY_IDLE:
        default:
          if (onFloor && GetRandomValue(0, 40) == 0)
            b->dir = -b->dir;
          b->velX += b->dir * BUNNY_MOVE_SPEED;
          if (b->velX > BUNNY_MAX_WALK_SPEED)
            b->velX = BUNNY_MAX_WALK_SPEED;
          else if (b->velX < -BUNNY_MAX_WALK_SPEED)
            b->velX = -BUNNY_MAX_WALK_SPEED;
          if (onFloor && b->jumpCooldown == 0 && GetRandomValue(0, 60) == 0)
          {
            b->velY = -BUNNY_JUMP_SPEED;
            b->jumpCooldown = BUNNY_JUMP_COOLDOWN;
          }
          break;
      }

      // Check collision with player
      if (CheckCollisionRecs(b->rect, player.rect))
      {
        if (player.drill && player.velY > 0 && b->flashTimer == 0)
        {
          b->health--;
          b->flashTimer = 1;
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

    // Move horizontally
    b->rect.x += b->velX;
    entityResolveHorizontalCollision(&b->rect, &b->velX);

    if (b->velX == 0.0f)
      b->dir = -b->dir;

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
    if (isDay() || bunnies[i].flashTimer > 0)
      color = WHITE;
    else
    {
      switch (bunnies[i].state)
      {
        case BUNNY_ALERT:    color = ORANGE;  break;
        case BUNNY_CHASE:    color = RED;     break;
        case BUNNY_CONFUSED: color = YELLOW;  break;
        case BUNNY_IDLE:
        default:             color = (Color){80, 0, 0, 200}; break;
      }
    }

    DrawRectangleRec(bunnies[i].rect, color);
  }
}