#include "terrain.h"
#include "time_cycle.h"

Tile terrain[TERRAIN_COUNT] = {0};

void terrainInit()
{
  terrainTileSet(3, 3 +4, 1);
  terrainTileSet(0, 4 +4, 1);
  terrainTileSet(1, 4 +4, 2);
  terrainTileSet(2, 4 +4, 3);
  terrainTileSet(3, 4 +4, 4);
  terrainTileSet(3, 5 +4, 4);
}

void terrainDraw()
{
  for (int x = 0; x < TERRAIN_TILES_X; x++)
  for (int y = 0; y < TERRAIN_TILES_Y; y++)
  {
    int value = terrainTileGet(x, y);

    if (value != 0)
      DrawTexturePro(
        texture,
        (Rectangle){(value - 1) * TILE_SIZE, (!isDay() * 16), TILE_SIZE, TILE_SIZE},
        (Rectangle){x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE},
        (Vector2){0.0f, 0.0f},
        0.0f,
        WHITE);
  }
}

Collision terrainCollision(Rectangle rect)
{
  Collision collision = {0};

  int startX = rect.x / TILE_SIZE;
  int endX   = (int)((rect.x + rect.width  - 0.0001f) / TILE_SIZE);
  int startY = rect.y / TILE_SIZE;
  int endY   = (int)((rect.y + rect.height - 0.0001f) / TILE_SIZE);

  for (int y = startY; y <= endY; y++)
  for (int x = startX; x <= endX; x++)
  {
    unsigned char value = terrainTileGet(x, y);

    // If water
    if (value == 4)
      collision.water = true;
    
      // If solid
    else if (value)
      collision.solid = true;

    if (collision.water && collision.solid)
      return collision;
  }

  return collision;
}
