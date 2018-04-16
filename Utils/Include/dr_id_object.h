#pragma once

#include <functional>
#include "dr_util_prerequisites.h"

#define CLASS_NAME_ID(name) std::hash<driderSDK::String>{}(#name);

namespace driderSDK {

template<class T>
class IDDispatcher
{
public:
  static UInt32 get()
  {
    static UInt32 id = 0;
    return id++;
  }
};

class DR_UTIL_EXPORT IDObject
{
public:

  IDObject();

  FORCEINLINE UInt32 getID() const
  {
    return m_id;
  }
private:
  const UInt32 m_id;
};

}
