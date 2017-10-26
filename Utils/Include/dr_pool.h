#pragma once
#include "dr_prerequisites.h"
#include <vector>
#include <memory>

namespace driderSDK {
template<class T, UInt32 poolSize>
class Pool
{
public:
  Pool()
  {
    m_pool.reserve(poolSize);
  }
  template <typename ...Args>
  void* malloc(size_t numObjects, Args... args)
  {
    for (size_t i; i < numObjects; ++i) {
      new (m_pool[(m_nextObjectIndex + i + 1) % poolSize]) T();
    }
    size_t tempIndex = m_nextObjectIndex;
    m_nextObjectIndex = (m_nextObjectIndex + numObjects + 1) % numObjects;
    return &m_pool[tempIndex];
  }
  void free(void* object) { }
private:
  std::vector<T> m_pool;
  UInt32 m_nextObjectIndex;
};
}