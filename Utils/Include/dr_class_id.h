#pragma once

#include <functional>
#include "dr_util_prerequisites.h"

namespace driderSDK {

template<class T = UInt32>
class UniqueID
{
public:
  static UInt32 get()
  {
    static UInt32 id = 0;
    return id++;
  }
};

#define CLASS_NAME_ID(name) std::hash<driderSDK::String>{}(#name);


}
