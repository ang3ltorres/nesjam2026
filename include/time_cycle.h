#pragma once

typedef struct TimeCycle
{
  float time; // Time in minutes
  float timeMultiplier;
  float sizeSun;
  float sizeMoon;
  int dayStart;
  int dayEnd;

} TimeCycle;

void timeCycleUpdate();
void timeCycleDraw();

bool isDay();

extern TimeCycle timeCycle;
