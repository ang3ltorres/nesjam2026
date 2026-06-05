#include "render.h"

#include "global.h"

Render render = {
  .scale  = 1,
  .texture = {0}
};

void updateScale()
{
  int scaleX = GetScreenWidth()  / GAME_WIDTH;
  int scaleY = GetScreenHeight() / GAME_HEIGHT;
  
  render.scale = (scaleX < scaleY) ? scaleX : scaleY;
  if (render.scale < 1) render.scale = 1;
}