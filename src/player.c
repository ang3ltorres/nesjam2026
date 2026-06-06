#include "player.h"

#include "global.h"

Player player = {0};

void playerInit()
{
  player = (Player){
    .rect = {32.0f, 32.0f, 6.0f, 15.0f}
  };
}

void playerUpdate()
{

}

void playerDraw()
{
  
  DrawRectangleRec(player.rect, MAGENTA);
  DrawTexturePro(
    texture,
    (Rectangle){4.0f, 33.0f, 8.0f, 15.0f},
    (Rectangle){player.rect.x - 1.0f, player.rect.y, 8.0f, 15.0f},
    (Vector2){},
    0.0f,
    WHITE
  );

}
