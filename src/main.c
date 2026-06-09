#include "game.h"

int main(void)
{
  init();

  while (!WindowShouldClose())
  {
    if (gameScreen)
    {
      ////////////
      // Update //
      ////////////
      update();

      //////////
      // Draw //
      //////////
      draw();
    }
    else
    {
      /////////////////
      // Menu Update //
      /////////////////
      menuUpdate();

      ///////////////
      // Menu Draw //
      ///////////////
      menuDraw();
    }
  }

  end();

  return 0;
}