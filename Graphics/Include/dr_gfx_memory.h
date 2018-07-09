#pragma once
#include "dr_graphics_prerequisites.h"
#include <dr_memory.h>
#include <memory>
#include <functional>
namespace driderSDK {
  
  template<class T>
  using GFXUnique = std::unique_ptr<T, std::function<void(T*)>>;

  template<class T>
  using GFXShared = std::shared_ptr<T>;
  
  template<class T>
  void 
  dr_gfx_deleter(T* obj)
  {
    obj->release();
  }
  
  template<class T>
  GFXUnique<T>
  dr_gfx_unique(T* ptr)
  {
    return dr_unique_custom(ptr, &dr_gfx_deleter<T>);
  }

  template<class T>
  GFXShared<T>
  dr_gfx_shared(T* ptr)
  {
    return dr_shared_custom(ptr, &dr_gfx_deleter<T>);
  }
}