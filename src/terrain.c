#include "terrain.h"
#include "time_cycle.h"

Tile terrain[TERRAIN_COUNT] = {0};

void terrainInit()
{
  for (int j = 2; j < 15; j++)
  for (int i = 0; i < 16; i++)
    terrainTileSet(i, j, 2);

  terrainTileSet(2, 1, 0);
  terrainTileSet(2, 2, 0);
  terrainTileSet(2, 3, 0);
  terrainTileSet(2, 4, 0);
  terrainTileSet(2, 5, 0);
  terrainTileSet(2, 6, 0);
  terrainTileSet(2, 7, 0);
  terrainTileSet(2, 8, 0);
  terrainTileSet(2, 1, 2);
}

void terrainDraw()
{
  for (int x = 0; x < TERRAIN_TILES_X; x++)
  for (int y = 0; y < TERRAIN_TILES_Y; y++)
  {
    Tile *t = &terrain[y * TERRAIN_TILES_X + x];

    if (t->tile != 0)
    {
      DrawTexturePro(
        texture,
        (Rectangle){(t->tile - 1) * TILE_SIZE, (!isDay() * 16), TILE_SIZE, TILE_SIZE},
        (Rectangle){x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE},
        (Vector2){0.0f, 0.0f},
        0.0f,
        WHITE
      );

      if (t->damage)
      {
        DrawTexturePro(
          texture,
          (Rectangle){48.0f - ((t->tile - (t->damage - 1)) * 16), 64.0f, TILE_SIZE, TILE_SIZE},
          (Rectangle){x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE},
          (Vector2){0.0f, 0.0f},
          0.0f,
          WHITE
        );
      }
    }
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
