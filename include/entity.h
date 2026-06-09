#pragma once
#include <raylib.h>
#include "terrain.h"

#define ENTITY_GRAVITY        0.085f
#define ENTITY_MAX_FALL_SPEED 4.5f

// Shared physics helpers for player & enemies
void entityApplyGravity(float *velY);
void entityResolveVerticalCollision(Rectangle *rect, float *velY);
void entityResolveHorizontalCollision(Rectangle *rect, float *velX);
bool entityHasFloor(Rectangle rect);
