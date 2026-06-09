#pragma once

#include "game.h"

typedef struct Control
{
  // 0=hold, 1=pressed, 2=released
  bool up[3];
  bool down[3];
  bool left[3];
  bool right[3];
  bool a[3];
  bool b[3];
  bool start[3];
  bool select[3];
} Control;

extern Control control;

void controlUpdate();

