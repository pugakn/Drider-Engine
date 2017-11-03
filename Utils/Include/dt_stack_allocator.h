#pragma once

#include "dr_prerequisites.h"

namespace driderSDK {

template<class T, SizeT N = 1024>
class StackAllocator 
{
public:

  StackAllocator() : m_ptr(m_buffer)
  {}

  T* 
  allocate(SizeT elements)
  {
    assert(pointerInBuffer(m_ptr));

    SizeT bytes = elements * sizeof(T);

    Int8* r(m_ptr);

    m_ptr += bytes;

    return reinterpret_cast<T*>(r);
  }

  void 
  deallocate(T* p, SizeT elements)
  {
    SizeT bytes = elements * sizeof(T);

    int8* bytePtr = reinterpret_cast<Int8*>(p);
    
    assert(bytePtr + bytes == m_ptr);
    
    m_ptr = bytePtr;
  }

  SizeT size() const 
  {
    return N;
  }

private:

  bool
  pointerInBuffer(int8* p)
  {
    return (m_buffer <= p && p <= m_buffer + BufferSize);
  }

  Int8 m_buffer[BufferSize];
  Int8* m_ptr;
};

}

