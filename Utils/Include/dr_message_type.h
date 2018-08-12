#pragma once
#include "dr_util_prerequisites.h"

namespace driderSDK {

namespace FUNCTION_TYPE {

enum E : UInt32 {
  Instantiate,
  Move,
  SetPosition,
  SetRotation,
  RegisterObject,
  RegisterVar
};

}

namespace PARAM_TYPE {

enum E : UInt32 {
  FLOAT = 1,
  INT,
  STRING
};

}

}