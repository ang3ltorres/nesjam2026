#include <raylib.h>
#include <stdio.h>

#include "time_cycle.h"
#include "render.h"
#include "terrain.h"
#include "player.h"
#include "bunny.h"
#include "game.h"

int main(void)
{
  const int screenWidth  = 1280;
  const int screenHeight = 720;

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(screenWidth, screenHeight, "Till Midnight");
  SetWindowMinSize(GAME_WIDTH, GAME_HEIGHT);
  SetTargetFPS(60);

  init();
  
  updateScale();

  while (!WindowShouldClose())
  {
    ////////////
    // Update //
    ////////////
    timeCycleUpdate();
    playerUpdate();
    bunnyUpdate();

    // Window scale
    if (IsWindowResized())
      updateScale();

    //////////
    // Draw //
    //////////
    BeginTextureMode(renderTexture);

    ClearBackground(BLACK);

    timeCycleDraw();
    terrainDraw();
    bunnyDraw();
    playerDraw();

    // draw time in hours/minutes
    int hours = ((int)timeCycle.time / 60) % 24;
    int minutes = (int)timeCycle.time % 60;
    DrawText(TextFormat("Time: %02i:%02i", hours, minutes), 0, 0, 10, RAYWHITE);

    // draw health hearts
    for (int i = 0; i < 5; i++)
    {
      int hx = GAME_WIDTH  - 8 - i * 8;
      int hy = GAME_HEIGHT - 8;
      DrawRectangle(hx, hy, 6, 6, (i < player.health) ? RED : DARKGRAY);
    }

    EndTextureMode();


    BeginDrawing();

    ClearBackground(RAYWHITE);
    // DrawTexture(renderTexture, 0, 0, WHITE);
    DrawTexturePro(renderTexture.texture, (Rectangle){ 0, 0, GAME_WIDTH, -GAME_HEIGHT }, (Rectangle){ 0, 0, GAME_WIDTH * scale, GAME_HEIGHT * scale }, (Vector2){ 0, 0 }, 0, WHITE);

    EndDrawing();
  }

  end();
  CloseWindow();

  return 0;
}