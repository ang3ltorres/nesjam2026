#include "entity.h"

void entityApplyGravity(float *velY)
{
  *velY += ENTITY_GRAVITY;
  if (*velY > ENTITY_MAX_FALL_SPEED)
    *velY = ENTITY_MAX_FALL_SPEED;
}

void entityResolveVerticalCollision(Rectangle *rect, float *velY)
{
  if (!terrainCollision(*rect).solid)
    return;

  // Hit ceiling
  if (*velY < 0)
  {
    int tileY = (int)(rect->y / TILE_SIZE);
    rect->y   = (float)((tileY + 1) * TILE_SIZE);
  }
  // Hit floor
  else
  {
    int tileY = (int)((rect->y + rect->height - 0.0001f) / TILE_SIZE);
    rect->y   = (float)(tileY * TILE_SIZE) - rect->height;
  }
  *velY = 0.0f;
}

void entityResolveHorizontalCollision(Rectangle *rect, float *velX)
{
  if (!terrainCollision(*rect).solid)
    return;

  if (*velX > 0)
    rect->x = (float)((int)((rect->x + rect->width) / TILE_SIZE) * TILE_SIZE) - rect->width;
  else
    rect->x = (float)((int)(rect->x / TILE_SIZE) * TILE_SIZE + TILE_SIZE);

  *velX = 0.0f;
}

bool entityHasFloor(Rectangle rect)
{
  rect.y += 1;
  return terrainCollision(rect).solid;
}
