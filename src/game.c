#include "game.h"

#include "render.h"
#include "terrain.h"
#include "player.h"

Texture2D texture = {0};


void init()
{
  renderTexture  = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);
  texture        = LoadTexture("../res/sprites.png");

  terrainInit();
  playerInit();
}

void end()
{
  UnloadTexture(texture);
  UnloadRenderTexture(renderTexture);
  
}
