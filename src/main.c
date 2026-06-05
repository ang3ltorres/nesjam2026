#include <raylib.h>
#include <stdio.h>

#include "time_cycle.h"
#include "render.h"
#include "player.h"
#include "global.h"

int main(void)
{
  const int screenWidth  = 1280;
  const int screenHeight = 720;

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(screenWidth, screenHeight, "JTM - Joy Till Midnight");
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

    // Window scale
    if (IsWindowResized())
      updateScale();

    //////////
    // Draw //
    //////////
    BeginTextureMode(render.texture);

    ClearBackground(BLACK);

     // draw time in hours/minutes
    int hours = ((int)timeCycle.time / 60) % 24;
    int minutes = (int)timeCycle.time % 60;
    DrawText(TextFormat("Time: %02i:%02i", hours, minutes), 0, 0, 10, RAYWHITE);

    // draw time raw
    // DrawText(TextFormat("RAW Time: %f", timeCycle.time), 0, 32, 10, RAYWHITE);

    // DrawTexture(texture, 0, 0, WHITE);

    timeCycleDraw();
    playerDraw();

    EndTextureMode();


    BeginDrawing();

    ClearBackground(RAYWHITE);
    // DrawTexture(render.texture, 0, 0, WHITE);
    DrawTexturePro(render.texture.texture, (Rectangle){ 0, 0, GAME_WIDTH, -GAME_HEIGHT }, (Rectangle){ 0, 0, GAME_WIDTH * render.scale, GAME_HEIGHT * render.scale }, (Vector2){ 0, 0 }, 0, WHITE);

    EndDrawing();
  }

  end();
  CloseWindow();

  return 0;
}