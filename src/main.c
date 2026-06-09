#include "game.h"
#include "control.h"

int main(void)
{
  init();

  while (!WindowShouldClose())
  {
    controlUpdate();

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