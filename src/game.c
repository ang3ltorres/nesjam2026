#include "game.h"
#include "control.h"

#include "time_cycle.h"
#include "render.h"
#include "terrain.h"
#include "player.h"
#include "bunny.h"

bool gameScreen = false;

Texture2D texture = {0};
Font      font    = {0};
Camera2D  camera  = {0};

void init()
{
  const int screenWidth  = 1280;
  const int screenHeight = 720;

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(screenWidth, screenHeight, "Till Midnight");
  SetWindowMinSize(GAME_WIDTH, GAME_HEIGHT);
  SetTargetFPS(60);

  renderTexture = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);
  texture       = LoadTexture("assets/sprite.png");
  font          = LoadFontEx("assets/PressStart2P-Regular.ttf", 8, NULL, 0);

  terrainInit();
  playerInit();
  bunnyInit();
  menuInit();

  camera = (Camera2D){
    .target   = { 0.0f, 0.0f },
    .offset   = { 0.0f, 0.0f },
    .rotation = 0.0f,
    .zoom     = 1.0f,
  };

  updateScale();
}

void end()
{
  menuEnd();
  UnloadTexture(texture);
  UnloadRenderTexture(renderTexture);

  CloseWindow();
}

void update()
{
  timeCycleUpdate();
  playerUpdate();
  bunnyUpdate();

  // Camera follows player horizontally, clamped to world bounds
  float targetCamX = player.rect.x + player.rect.width / 2.0f - GAME_WIDTH / 2.0f;
  if (targetCamX < 0.0f)
    targetCamX = 0.0f;
  if (targetCamX > WORLD_WIDTH - GAME_WIDTH)
    targetCamX = (float)(WORLD_WIDTH - GAME_WIDTH);
  camera.target.x = targetCamX;

  // Window scale
  if (IsWindowResized())
    updateScale();
}

void draw()
{
  BeginTextureMode(renderTexture);

  ClearBackground(BLACK);

  timeCycleDraw();

  BeginMode2D(camera);

  terrainDraw();
  bunnyDraw();
  playerDraw();

  EndMode2D();

  ////////
  // UI //
  ////////
  uiDraw();

  EndTextureMode();

  BeginDrawing();

  ClearBackground(RAYWHITE);
  DrawTexturePro(renderTexture.texture, (Rectangle){ 0, 0, GAME_WIDTH, -GAME_HEIGHT }, (Rectangle){ 0, 0, GAME_WIDTH * scale, GAME_HEIGHT * scale }, (Vector2){ 0, 0 }, 0, WHITE);

  EndDrawing();
}

static int selectedOption = 0;
static const char *menuOptions[] = { "START", "ABOUT", "EXIT" };
static const int numOptions = 3;

// Simple state for about screen
static bool showAbout = false;

void uiDraw()
{

  // DrawText(TextFormat("Time: %02i:%02i", hours, minutes), 0, 0, 10, RAYWHITE);
  DrawTextEx(font, TextFormat("%02i:%02i", timeCycle.hour, timeCycle.minute), (Vector2){0.0f, 0.0f}, 8, 0.0f, RAYWHITE);

  for (int i = 0; i < 5; i++)
  {
    int hx = GAME_WIDTH  - 8 - i * 8;
    int hy = 8;
    DrawRectangle(hx, hy, 6, 6, (i < player.health) ? RED : DARKGRAY);
  }

  DrawTextEx(font, TextFormat("Remaining: %02i", bunniesCount), (Vector2){0.0f, 20.0f}, 8, 0.0f, RAYWHITE);
}

void menuInit(void)
{
  selectedOption = 0;
  showAbout = false;
}

void menuEnd(void)
{
  UnloadFont(font);
}

void menuUpdate(void)
{
  if (showAbout)
  {
    if (control.b[1] || control.start[1] || IsKeyPressed(KEY_ESCAPE))
      showAbout = false;

    return;
  }

  // Navigate menu options
  if (control.down[1] || control.right[1])
    selectedOption = (selectedOption + 1) % numOptions;

  if (control.up[1] || control.left[1])
    selectedOption = (selectedOption - 1 + numOptions) % numOptions;

  // Confirm selection
  if (control.b[1] || control.start[1])
  {
    switch (selectedOption)
    {
      case MENU_OPTION_START:
        gameScreen = true; break;
      case MENU_OPTION_ABOUT:
        showAbout = true; break;
      case MENU_OPTION_EXIT:
        end(); break;
    }
  }
}

static void drawTitleScreen(void)
{
  // Placeholder rectangle for the title image (NES resolution)
  // This marks where the real PNG/title screen will go
  int titleW = 200;
  int titleH = 80;
  int titleX = (GAME_WIDTH - titleW) / 2;
  int titleY = 20;

  DrawRectangle(titleX, titleY, titleW, titleH, DARKGRAY);
  DrawRectangleLines(titleX, titleY, titleW, titleH, LIGHTGRAY);

  // "TITLE" text placeholder label
  const char *label = "TILL MIDNIGHT";
  int fontSize = 8;
  int textW = MeasureTextEx(font, label, fontSize, 1).x;
  DrawTextEx(font, label, (Vector2){ titleX + (titleW - textW) / 2, titleY + (titleH - fontSize) / 2 }, fontSize, 1, RAYWHITE);
}

static void drawMenuOptions(void)
{
  int fontSize = 8;
  int spacing = 1;
  int startY = 130; // Below the title area

  for (int i = 0; i < numOptions; i++)
  {
    Color color = (i == selectedOption) ? YELLOW : RAYWHITE;
    const char *prefix = (i == selectedOption) ? ">" : " ";
    const char *text = TextFormat("%s %s", prefix, menuOptions[i]);

    Vector2 textSize = MeasureTextEx(font, text, fontSize, spacing);
    int x = (GAME_WIDTH - (int)textSize.x) / 2;
    int y = startY + i * 20;

    DrawTextEx(font, text, (Vector2){ x, y }, fontSize, spacing, color);
  }
}

static void drawAboutScreen(void)
{
  DrawRectangle(0, 0, GAME_WIDTH, GAME_HEIGHT, BLACK);

  const char *lines[] = {
    "A NES-style game",
    "made for NESJam 2026",
    "",
    "CREDITS",
    "Program: Angel",
    "Music: Damian",
    "Art: Kike",
    "",
    "Nothing is as it seems",
    "Be careful at night",
    "Have no mercy...",
    "",
    "Press ENTER to go back"
  };
  int numLines = sizeof(lines) / sizeof(lines[0]);
  int fontSize = 8;

  for (int i = 0; i < numLines; i++)
  {
    Vector2 size = MeasureTextEx(font, lines[i], fontSize, 1);
    int x = (GAME_WIDTH - (int)size.x) / 2;
    int y = 24 + i * 14;
    DrawTextEx(font, lines[i], (Vector2){ x, y }, fontSize, 1, RAYWHITE);
  }
}

void menuDraw(void)
{
  BeginTextureMode(renderTexture);

  ClearBackground(BLACK);

  if (showAbout)
  {
    drawAboutScreen();
  }
  else
  {
    drawTitleScreen();
    drawMenuOptions();
  }

  EndTextureMode();

  // Scale up to window
  BeginDrawing();

  ClearBackground(RAYWHITE);
  DrawTexturePro(
    renderTexture.texture,
    (Rectangle){ 0, 0, GAME_WIDTH, -GAME_HEIGHT },
    (Rectangle){ 0, 0, GAME_WIDTH * scale, GAME_HEIGHT * scale },
    (Vector2)
    { 0, 0 },
    0,
    WHITE);

  EndDrawing();
}
