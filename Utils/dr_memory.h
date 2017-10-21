#pragma once
#include <memory>
#include <utility>
#include "dr_prerequisites.h"

template<class T, class Alloc = std::allocator, class... Args>
static T*
dr_new(Args&&... args)
{
  return Alloc<T>::allocate(std::forward<Args>(args)...);
}

