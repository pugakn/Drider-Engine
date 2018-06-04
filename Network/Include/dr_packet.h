#pragma once

#include <vector>

#include "dr_network_prerequisites.h"

namespace driderSDK {

class DR_NETWORK_EXPORT Packet
{
 public:
  
   Packet();

   Packet(SizeT initialSize);

  ~Packet();

  /**
  * Resets the internal buffer.
  */
  void
  clear();

  void 
  addData(const void* data, SizeT dataSize);

  void
  addData(std::vector<Int8>& data);
  
  template<class T>
  Packet& 
  operator<<(const T& data)
  {
    addData(&data, sizeof(T));
    return *this;
  }
  
  template<>
  Packet& 
  operator<<<String>(const String& data)
  {
    UInt32 strLen = static_cast<UInt32>(data.size());
    *this << strLen;
    addData(data.data(), strLen * sizeof(String::value_type));

    return *this;
  }

  template<>
  Packet& 
  operator<<<WString>(const WString& data)
  {
     UInt32 strLen = static_cast<UInt32>(data.size());
    *this << strLen;

    for (auto& wc : data) {
      *this << static_cast<UInt32>(wc);
    }

    return *this;
  }
  /**
  * Read operator
  */
  template<class T>
  Packet&
  operator>>(T& data)
  {
    
    Int8* buffer;

    getDataPtr(buffer, sizeof(T));

    if (buffer) {
      data = *reinterpret_cast<T*>(buffer);

      //removeData(sizeof(T));
    }

    return *this;
  }

  template<>
  Packet& 
  operator>><String>(String& data)
  {
    UInt32 size = 0;
    *this >> size;
    
    data.clear();

    if (size > 0) {
      Int8* buffer;
      getDataPtr(buffer, size);
      if (buffer) {
        data.assign(buffer, buffer + size);
        //removeData(size);
      }
    }

    return *this;
  }

  template<>
  Packet& 
  operator>><WString>(WString& data)
  {
    UInt32 size = 0;

    *this >> size;

    data.clear();

    data.resize(size);

    size *= sizeof(UInt32);
    
    if (size > 0) {
      Int8* buffer;
      getDataPtr(buffer, size);
      if (buffer) {
        for (auto& wc : data) {
          UInt32 v = *reinterpret_cast<UInt32*>(buffer);
          wc = static_cast<WString::value_type>(v);
          buffer += sizeof(UInt32);
        }
        //removeData(size);
      }
    }

    return *this;
  }


  FORCEINLINE SizeT 
  getSize() const
  {
    return m_data.size();
  }

  FORCEINLINE const std::vector<Int8>&
  getData() const
  {
    return m_data;
  }

  FORCEINLINE bool 
  isOk() const
  {
    return m_isOk;
  }

  /**
  * Tests if the packet has no further information to read.
  */
  FORCEINLINE bool
  isEmpty() const
  {
    return m_data.empty() || m_readPoint == m_data.size();
  }

  FORCEINLINE void
  resetReadPos()
  {
    m_readPoint = 0;
  }
 

 private:

  friend class UDPSocket;

  void
  getDataPtr(Int8*& buffer, SizeT size);

  /*void
  removeData(SizeT size);*/

  bool
  canRead(SizeT size);

  std::vector<Int8> m_data;

  UInt32 m_readPoint;

  bool m_isOk;
};

}
