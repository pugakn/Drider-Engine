#pragma once

#include <functional>
#include <atomic>
#include "dr_util_prerequisites.h"

class DR_UTIL_EXPORT IDBase
{
 public:
  static driderSDK::UInt32
  getNextID();
};

template<class T>
class IDClass
{
public:
  static const driderSDK::UInt32& ID()
  {
    static driderSDK::UInt32 id = IDBase::getNextID();
    return id;
  }
private:
};

#define CLASS_NAME_ID(name) IDClass<name>::ID()

namespace driderSDK {

template<class T>
class IDDispatcher
{
 public:
  static UInt32 get()
  {
    static std::atomic<UInt32> id = 0;
    return id++;
  }
};

//template<class T>
//class IDClass
//{
// public:
//  
//  IDClass() : m_id(typeid(T).name())
//  
//  FORCEINLINE UInt32 getID() const
//  {
//    return m_id;
//  }
// private:
//   UInt32 m_id;
//};

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
