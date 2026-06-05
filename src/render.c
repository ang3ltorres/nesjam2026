#include "render.h"

#include "global.h"

int scale = 1;

RenderTexture2D renderTexture = {0};

void updateScale()
{
  int scaleX = GetScreenWidth()  / GAME_WIDTH;
  int scaleY = GetScreenHeight() / GAME_HEIGHT;
  
  scale = (scaleX < scaleY) ? scaleX : scaleY;
  if (scale < 1) scale = 1;
}