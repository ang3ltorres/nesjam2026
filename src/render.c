#include "render.h"

Render render = {
  .width  = 256,
  .height = 240,
  .scale  = 1,
  .texture = {0}
};

void renderInit()
{
  render.texture = LoadRenderTexture(render.width, render.height);
}

void updateScale()
{
  int scaleX = GetScreenWidth()  / 256;
  int scaleY = GetScreenHeight() / 240;
  
  render.scale = (scaleX < scaleY) ? scaleX : scaleY;
  if (render.scale < 1) render.scale = 1;
}