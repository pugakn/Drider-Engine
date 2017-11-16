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

template<class T, class... Args>
std::unique_ptr<T>
dr_make_unique(Args&&... args) 
{
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

}