#include "control.h"
#include <raylib.h>
#include <string.h>

Control control = {0};

static void updateButton(bool *state, bool held)
{
  // state[0] = hold
  // state[1] = pressed (transition 0→1)
  // state[2] = released (transition 1→0)
  bool prev = state[0];
  state[0] = held;
  state[1] = held && !prev;
  state[2] = !held && prev;
}

void controlUpdate(void)
{
  const int gp = 0; // first gamepad

  bool gpAvailable = IsGamepadAvailable(gp);

  // Left
  bool leftHeld =
    IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A) ||
    (gpAvailable && (
      IsGamepadButtonDown(gp, GAMEPAD_BUTTON_LEFT_FACE_LEFT) ||
      GetGamepadAxisMovement(gp, GAMEPAD_AXIS_LEFT_X) < -0.5f
    ));
  updateButton(control.left, leftHeld);

  // Right
  bool rightHeld =
    IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D) ||
    (gpAvailable && (
      IsGamepadButtonDown(gp, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) ||
      GetGamepadAxisMovement(gp, GAMEPAD_AXIS_LEFT_X) > 0.5f
    ));
  updateButton(control.right, rightHeld);

  // Up
  bool upHeld =
    IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) ||
    (gpAvailable && (
      IsGamepadButtonDown(gp, GAMEPAD_BUTTON_LEFT_FACE_UP) ||
      GetGamepadAxisMovement(gp, GAMEPAD_AXIS_LEFT_Y) < -0.5f
    ));
  updateButton(control.up, upHeld);

  // Down
  bool downHeld =
    IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S) ||
    (gpAvailable && (
      IsGamepadButtonDown(gp, GAMEPAD_BUTTON_LEFT_FACE_DOWN) ||
      GetGamepadAxisMovement(gp, GAMEPAD_AXIS_LEFT_Y) > 0.5f
    ));
  updateButton(control.down, downHeld);

  // A (drill)
  // Keyboard: left shift, M
  // Gamepad: Circle / right face right (Xbox B, PS Circle)
  bool aHeld =
    IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_M) ||
    (gpAvailable && IsGamepadButtonDown(gp, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT));
  updateButton(control.a, aHeld);

  // B (jump)
  // Keyboard: space, N
  // Gamepad: Cross / right face down (Xbox A, PS Cross)
  bool bHeld =
    IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_N) ||
    (gpAvailable && IsGamepadButtonDown(gp, GAMEPAD_BUTTON_RIGHT_FACE_DOWN));
  updateButton(control.b, bHeld);

  // Start
  // Keyboard: Enter
  // Gamepad: Start (middle right)
  bool startHeld =
    IsKeyDown(KEY_ENTER) ||
    (gpAvailable && IsGamepadButtonDown(gp, GAMEPAD_BUTTON_MIDDLE_RIGHT));
  updateButton(control.start, startHeld);

  // Select
  // Keyboard: Left Ctrl
  // Gamepad: Back/Select (middle left)
  bool selectHeld =
    IsKeyDown(KEY_LEFT_CONTROL) ||
    (gpAvailable && IsGamepadButtonDown(gp, GAMEPAD_BUTTON_MIDDLE_LEFT));
  updateButton(control.select, selectHeld);
}

