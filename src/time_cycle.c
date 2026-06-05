#include "time_cycle.h"
#include <raylib.h>
#include <raymath.h>
#include <math.h>
#include <stdio.h>

#include "render.h"

TimeCycle timeCycle = {

  .time           = 0.0f,
  .timeMultiplier = 60.0f * 10,   // 1 hour per second
  .sizeSun        = 16.0f,
  .sizeMoon       = 32.0f,
  .dayStart       = 7,
  .dayEnd         = 19
};

void timeCycleUpdate()
{
  // Use delta time to increment time
  if (timeCycle.time >= 24 * 60) // Reset after 24 hours
    timeCycle.time = 0;
  else
    timeCycle.time += GetFrameTime() * timeCycle.timeMultiplier;
}

void timeCycleDraw()
{
  // Convert time to degrees


  //
  // degrees = (180.0f / 12.0f) * (timeCycle.time - (7*60)) ;
  float degrees = 180.0f + (timeCycle.time - timeCycle.dayStart * 60.0f) * 0.25f;
  
  float angle = DEG2RAD * degrees;

  if (!isDay())
    angle += PI;

  float posX = (render.width / 2.0f) + cos(angle) * 170;
  float posY = (render.height / 2.0f + 86) + sin(angle) * 170;

  printf("Degrees: %f\n", degrees);

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
