#pragma once
#include "dr_input_prerequisites.h"
#include <OIS\OISKeyboard.h>
#include <dr_vector2di.h>
namespace driderSDK {
  struct DR_INPUT_EXPORT KeyboardState {
    Int32 m_pressedButtons;
    Vector2DI m_cursorPosition;

  };
  class DR_INPUT_EXPORT Keyboard {

  };
}