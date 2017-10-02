#pragma once
#include "dr_prerequisites.h"
#include <vector>
#include <memory>

namespace driderSDK 
{
template<class T, UInt32 poolSize>
class Pool
{
 public:
  Pool() 
  {
    m_pool.resize(poolSize);
  }
  T* aquire()
  {
    if (m_nextObjectIndex == poolSize) {
      m_nextObjectIndex = 0;
    }
    return m_pool[m_nextObjectIndex++].get();
  }
 private:
  std::vector<std::unique_ptr<T>> m_pool;
  UInt32 m_nextObjectIndex;
};
}
