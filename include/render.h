#pragma once

#include <raylib.h>

typedef struct Render
{
  int scale;
  RenderTexture2D texture;
} Render;

void updateScale();

extern Render render;
