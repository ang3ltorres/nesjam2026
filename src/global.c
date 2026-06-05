#include "global.h"

#include "render.h"
#include "player.h"

Texture2D texture = {0};


void init()
{
  renderTexture = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);
  texture        = LoadTexture("../res/sprites.png");

  playerInit();
}

void end()
{
  UnloadTexture(texture);
  UnloadRenderTexture(renderTexture);
  
}
