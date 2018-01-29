#pragma once
#include "dr_input_prerequisites.h"
namespace driderSDK {
namespace InputObjectType {
  enum E {
    kKeyboard,
    kMouse,
    kJoystick
  };
}
class DR_INPUT_EXPORT InputObject {
  virtual void capture() = 0;
};
}