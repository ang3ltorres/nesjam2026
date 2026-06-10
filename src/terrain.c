#include "terrain.h"
#include "time_cycle.h"

Tile terrain[TERRAIN_COUNT] = {0};

void terrainInit()
{
  // World is 32 tiles wide (512 px) x 15 tiles tall (240 px)
  // Tile types: 1=dirt, 2=grass, 3=stone/brick, 4=water

  // --- Floor (rows 13-14) ---
  // Layer 1: solid ground
  for (int i = 0; i < 32; i++)
  {
    terrainTileSet(i, 13, 1);
    terrainTileSet(i, 14, 2);
  }

  // --- Section 1: Left area (tiles 0-7) ---
  // Entrance room with a pillar
  // Left wall
  terrainTileSet(0, 10, 3);
  terrainTileSet(0, 11, 3);
  terrainTileSet(0, 12, 3);
  // A pillar
  terrainTileSet(4, 10, 3);
  terrainTileSet(4, 11, 3);
  terrainTileSet(4, 12, 3);
  // Small platform on pillar
  terrainTileSet(4, 9, 2);
  // A water pool on the ground
  terrainTileSet(1, 12, 4);
  terrainTileSet(2, 12, 4);
  terrainTileSet(3, 12, 4);

  // --- Section 2: Staircase up (tiles 8-12) ---
  // Ascending stairs made of brick
  terrainTileSet(8, 12, 3);
  terrainTileSet(9, 11, 3);
  terrainTileSet(10, 10, 3);
  terrainTileSet(11, 9, 3);
  // Small platform at top
  terrainTileSet(12, 8, 2);
  terrainTileSet(13, 8, 2);
  terrainTileSet(14, 8, 2);

  // --- Section 3: Floating platforms (tiles 13-18) ---
  // Stepping stones over a water gap
  for (int i = 0; i < 6; i++)
  {
    // Water gap below
    terrainTileSet(13 + i, 12, 4);
    terrainTileSet(13 + i, 13, 4);
  }
  // Platforms above water
  terrainTileSet(14, 10, 3);
  terrainTileSet(15, 9, 2);
  terrainTileSet(16, 10, 3);
  terrainTileSet(17, 9, 2);
  terrainTileSet(18, 10, 3);

  // --- Section 4: Underground tunnel (tiles 19-24) ---
  // Ceiling and walls forming a small cave
  for (int i = 19; i <= 24; i++)
  {
    terrainTileSet(i, 9, 1);  // Ceiling
    terrainTileSet(i, 10, 1); // Fill
  }
  // Tunnel entrance on left
  terrainTileSet(19, 9, 0);
  terrainTileSet(19, 10, 0);
  // Tunnel exit on right
  terrainTileSet(24, 9, 0);
  terrainTileSet(24, 10, 0);
  // Interior decoration - brick accent
  terrainTileSet(21, 10, 3);
  terrainTileSet(22, 9, 3);
  // Water drip pool inside
  terrainTileSet(20, 12, 4);
  terrainTileSet(21, 12, 4);

  // --- Section 5: End room (tiles 25-31) ---
  // Raised platform with decorative wall
  for (int i = 25; i <= 31; i++)
  {
    terrainTileSet(i, 11, 3);  // Raised floor
  }
  // Back wall
  for (int y = 5; y <= 10; y++)
  {
    terrainTileSet(30, y, 1);
  }
  terrainTileSet(30, 10, 2); // Grass top
  // Water moat at front
  terrainTileSet(25, 12, 4);
  terrainTileSet(26, 12, 4);
  terrainTileSet(27, 12, 4);
  terrainTileSet(28, 12, 4);
  terrainTileSet(29, 12, 4);
  // Decorative pillar
  terrainTileSet(27, 9, 3);
  terrainTileSet(27, 10, 3);
  // Step up to platform
  terrainTileSet(25, 12, 3);
  terrainTileSet(26, 11, 3);
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
