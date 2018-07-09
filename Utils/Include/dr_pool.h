 #pragma once
#include "dr_util_prerequisites.h"
#include <vector>
#include <memory>
#include <cstddef>

#include <iostream>
namespace driderSDK {
template<class T>
class PoolAllocator
{
 public:
  typedef T* pointer;
  ~PoolAllocator() {};
  PoolAllocator() = default;
  PoolAllocator(const PoolAllocator&) = delete;
  void init(size_t poolSize) {
    m_nextObjectIndex = 0;
    m_poolSize = poolSize;
    m_pool = (byte*)malloc(m_poolSize * sizeof(T));
  }
  void destroy() {
    free(m_pool);
  }
  T* allocate(size_t numObjects, const T &va) {
    if ((m_nextObjectIndex + numObjects) > m_poolSize) {
      m_nextObjectIndex = 0;
    }
    for (size_t i= 0; i < numObjects; ++i) {
      //destroy((T*)&m_pool[((m_nextObjectIndex + i) % m_poolSize) * sizeof(T)]);
      construct ((T*)&m_pool[((m_nextObjectIndex + i) % m_poolSize) * sizeof(T)],va);
    }
    size_t tempIndex = m_nextObjectIndex;
    m_nextObjectIndex = (m_nextObjectIndex + numObjects) % m_poolSize;
    return (T*)&m_pool[tempIndex * sizeof(T)];
  }


  void construct(pointer p, const T &val) {
    new ((void*)p) T(val);
  }

  void destroy(pointer p) {
    ((T*)p)->~T();
  }
  //template <typename ...Args>
  //void* malloc(size_t numObjects, Args... args)
  //{
  //}
  void deallocate(T* p, size_t n) {
    destroy(p);
  }
  T operator [] (int i) const { return *(T*)&m_pool[i * sizeof(T)]; }
  T& operator [] (int i) { return *(T*)&m_pool[i * sizeof(T)]; }

  size_t size() { return m_poolSize; }
 private:
   size_t m_poolSize = 0;
  byte* m_pool;
  UInt32 m_nextObjectIndex = 0;
};
//template<class T>
//class Pool {
//public:
//  void init(size_t poolSize) {
//    m_allocator.init(poolSize);
//  }
//  void destroy() {
//    m_allocator.destroy();
//  }
//  void push(const T &val) {
//    T* p = m_allocator.allocate(1, val);
//    m_allocator.construct(p);
//    m_memory.push_back(p);
//  }
//private:
//  PoolAllocator<T> m_allocator;
//  std::vector<T*> m_memory;
//};
}