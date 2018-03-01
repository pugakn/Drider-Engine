#pragma once

#include <memory>

namespace driderSDK {

template<class T>
void 
dr_void_deleter(void* pData)
{
  T* pInfo = static_cast<T*>(pData);
  delete pInfo;
}

template<class T>
void 
dr_gfx_deleter(T* obj)
{
  obj->release();
}

template<class T, class Deleter>
std::shared_ptr<T>
dr_shared_custom(T* ptr, Deleter deleter)
{
  return std::shared_ptr<T>(ptr, deleter);
}

template<class T, class Deleter>
std::unique_ptr<T, Deleter>
dr_unique_custom(T* ptr, Deleter deleter)
{
  return std::unique_ptr<T, Deleter>(ptr, deleter);
}

template<class T, class... Args>
std::unique_ptr<T>
dr_make_unique(Args&&... args) 
{
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

}