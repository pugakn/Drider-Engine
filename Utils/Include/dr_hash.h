#pragma once

#include <functional>

#include "dr_util_prerequisites.h"

namespace driderSDK {

FORCEINLINE void 
dr_hash_combine(SizeT& seed) 
{}

template<class A, class... Args>
FORCEINLINE void
dr_hash_combine(SizeT& seed, const A& a, Args... args)
{  
  seed ^= dr_hash_value(a) + 0x9e3779b9 + (seed<<6) + (seed>>2);

  dr_hash_combine(seed, args...);
}

template<class T> 
FORCEINLINE SizeT
dr_hash_value(const T& v)
{
  return std::hash<T>{}(v);
}

}