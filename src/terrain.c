#include "terrain.h"
#include "time_cycle.h"

unsigned char terrain[TERRAIN_COUNT] = {0};

void terrainInit()
{
  terrainSet(3, 3, 1);
  terrainSet(0, 4, 1);
  terrainSet(1, 4, 2);
  terrainSet(2, 4, 3);
  terrainSet(3, 4, 4);
  terrainSet(3, 5, 4);

}

void terrainDraw()
{
  for (int x = 0; x < TERRAIN_TILES_X; x++)
  for (int y = 0; y < TERRAIN_TILES_Y; y++)
  {
    int value = terrainGet(x, y);

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
