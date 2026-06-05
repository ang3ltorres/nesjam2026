#pragma once

#include <raylib.h>

typedef struct Player
{
  Rectangle rect;
}Player;

extern Player player;

void playerInit();
void playerUpdate();
void playerDraw();

