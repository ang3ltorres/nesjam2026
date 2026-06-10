#pragma once
#include "render.h"
#include "game.h"

#define TERRAIN_COUNT ((WORLD_WIDTH * WORLD_HEIGHT) / (TILE_SIZE * TILE_SIZE))
#define TERRAIN_TILES_X (WORLD_WIDTH / TILE_SIZE)
#define TERRAIN_TILES_Y (WORLD_HEIGHT / TILE_SIZE)

typedef struct Tile
{
  unsigned char tile;
  unsigned char damage;
} Tile;

extern Tile terrain[TERRAIN_COUNT];

static inline unsigned char terrainTileGet(int posX, int posY)
{
  if (posX >= 0 && posX < TERRAIN_TILES_X && posY >= 0 && posY < TERRAIN_TILES_Y)
    return terrain[posY * TERRAIN_TILES_X + posX].tile;

  return 0;
}

static inline void terrainTileSet(int posX, int posY, unsigned char tile)
{
  if (posX >= 0 && posX < TERRAIN_TILES_X && posY >= 0 && posY < TERRAIN_TILES_Y)
    terrain[posY * TERRAIN_TILES_X + posX].tile = tile;
}

static inline void terrainDamageAdd(int posX, int posY)
{
  if (posX >= 0 && posX < TERRAIN_TILES_X && posY >= 0 && posY < TERRAIN_TILES_Y)
    terrain[posY * TERRAIN_TILES_X + posX].damage++;
}

static inline unsigned char terrainDamageGet(int posX, int posY)
{
  if (posX >= 0 && posX < TERRAIN_TILES_X && posY >= 0 && posY < TERRAIN_TILES_Y)
    return terrain[posY * TERRAIN_TILES_X + posX].damage;

  return 0;
}

void terrainInit();

void terrainDraw();

typedef struct Collision
{
  unsigned char solid;
  unsigned char water;
} Collision;

Collision terrainCollision(Rectangle rect);
