#include "time_cycle.h"
#include <raylib.h>
#include <raymath.h>
#include "game.h"

TimeCycle timeCycle = {

  .time           = 1100.0f,
  .timeMultiplier = 60.0f * 1,   // 1 hour per second
  .sizeSun        = 12.0f,
  .sizeMoon       = 8.0f,
  .dayStart       = 7,
  .dayEnd         = 19,
  .hour           = 0,
  .minute         = 0
};

void timeCycleUpdate()
{
  // Use delta time to increment time
  if (timeCycle.time >= 24 * 60) // Reset after 24 hours
    timeCycle.time = 0;
  else
    timeCycle.time += GetFrameTime() * timeCycle.timeMultiplier;

  timeCycle.hour   = ((int)timeCycle.time / 60) % 24;
  timeCycle.minute = (int)timeCycle.time % 60;
}

void timeCycleDraw()
{
  // Convert time to degrees
  float degrees = 180.0f + (timeCycle.time - timeCycle.dayStart * 60.0f) * 0.25f;
  float angle = DEG2RAD * degrees;

  if (!isDay())
    angle += PI;

  float posX = (GAME_WIDTH / 2.0f) + cos(angle) * 160;

  // Not using complete circle
  float posY = 4 + ((GAME_HEIGHT / 2.0f + 86) + sin(angle) * 170) / 4.0f;
  // float posY = 32.0f;

  DrawCircle(
    posX,
    posY,
    (isDay()) ? timeCycle.sizeSun : timeCycle.sizeMoon,
    (isDay()) ? YELLOW : WHITE
  );
}

bool isDay()
{
  return (timeCycle.time >= timeCycle.dayStart * 60 && timeCycle.time <= timeCycle.dayEnd * 60);
}
