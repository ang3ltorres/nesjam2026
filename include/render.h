#pragma once

#include <raylib.h>

typedef struct Render
{
  int width;
  int height;
  int scale;
  RenderTexture2D texture;
} Render;

void renderInit();
void updateScale();

extern Render render;
