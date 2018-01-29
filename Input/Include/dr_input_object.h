#pragma once
#include "dr_input_prerequisites.h"
namespace driderSDK {
namespace InputObjectType {
  enum E {
    kUnknown = 0,
    kKeyboard = 1,
    kMouse = 2,
    kJoystick = 3
  };
}
class DR_INPUT_EXPORT InputObject {
public:
  virtual void capture() = 0;
  virtual void internalInit(OIS::Object* obj) = 0;
  OIS::Object* m_obj;
  InputObjectType::E m_type;
};
}