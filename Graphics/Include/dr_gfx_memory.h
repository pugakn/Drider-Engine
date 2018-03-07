#pragma once
#include "dr_graphics_prerequisites.h"
#include <dr_memory.h>
#include <memory>
#include <functional>
namespace driderSDK {
  template<class T>
  using CustomDeleter = std::function<void(T*)>;

  template<class T>
  using GFXUnique = std::unique_ptr<T, CustomDeleter<T>>;

  template<class T>
  using GFXShared = std::shared_ptr<T>;

  template<class T>
  GFXUnique<T>
    dr_gfx_unique(T* ptr)
  {
    return std::unique_ptr<T, CustomDeleter<T>>(ptr, &dr_gfx_deleter<T>);
  }

  template<class T>
  GFXShared<T>
    dr_gfx_shared(T* ptr)
  {
    return std::shared_ptr<T>(ptr, &dr_gfx_deleter<T>);
  }
}