#pragma once

#include "dr_input_prerequisites.h"

namespace OIS {
class Object;
}

namespace driderSDK {
namespace InputObjectType {
enum E {
  kUnknown = 0,
  kKeyboard = 1,
  kMouse = 2,
  kJoystick = 3
};
}

/**
* Base class for input device object
*/
class DR_INPUT_EXPORT InputObject {
 public:
  InputObject(InputObjectType::E _type);

  virtual void 
  capture() = 0;

  virtual void 
  internalInit(OIS::Object* _obj) = 0;

  OIS::Object* obj;
 protected:
  InputObjectType::E m_type;
};
}