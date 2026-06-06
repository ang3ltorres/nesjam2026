#pragma once
#include "render.h"
#include "game.h"

#define TERRAIN_COUNT ((GAME_WIDTH * GAME_HEIGHT) / (TILE_SIZE * TILE_SIZE))
#define TERRAIN_TILES_X (GAME_WIDTH / TILE_SIZE)
#define TERRAIN_TILES_Y (GAME_HEIGHT / TILE_SIZE)

extern unsigned char terrain[TERRAIN_COUNT];

static inline unsigned char terrainGet(int posX, int posY)
{
  if (posX >= 0 && posX < TERRAIN_TILES_X && posY >= 0 && posY < TERRAIN_TILES_Y)
    return terrain[posY * TERRAIN_TILES_X + posX];

  return 0;
}

static inline void terrainSet(int posX, int posY, unsigned char value)
{
  if (posX >= 0 && posX < TERRAIN_TILES_X && posY >= 0 && posY < TERRAIN_TILES_Y)
    terrain[posY * TERRAIN_TILES_X + posX] = value;
}

void terrainInit();

void terrainDraw();

typedef struct Collision
{
  unsigned char solid;
  unsigned char water;
}Collision;

Collision terrainCollision(Rectangle rect);

