#pragma once
#include "render.h"
#include "global.h"

#define TERRAIN_COUNT ((GAME_WIDTH * GAME_HEIGHT) / (TILE_SIZE * TILE_SIZE))
#define TERRAIN_TILES_X (GAME_WIDTH / TILE_SIZE)
#define TERRAIN_TILES_Y (GAME_HEIGHT / TILE_SIZE)

extern unsigned char terrain[TERRAIN_COUNT];

static inline unsigned char terrainGet(int posX, int posY)
{
  int tx = posX / TILE_SIZE;
  int ty = posY / TILE_SIZE;

  if (tx >= 0 && tx < TERRAIN_TILES_X && ty >= 0 && ty < TERRAIN_TILES_Y)
    return terrain[ty * TERRAIN_TILES_X + tx];

  return 0;
}

static inline void terrainSet(int posX, int posY, unsigned char value)
{
  int tx = posX / TILE_SIZE;
  int ty = posY / TILE_SIZE;

  if (tx >= 0 && tx < TERRAIN_TILES_X && ty >= 0 && ty < TERRAIN_TILES_Y)
    terrain[ty * TERRAIN_TILES_X + tx] = value;
}
