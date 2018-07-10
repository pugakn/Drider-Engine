#include "dr_packet.h"
#include <iterator>

namespace driderSDK {

Packet::Packet() : Packet(0)
{}

Packet::Packet(SizeT initialSize) 
  : m_isOk(true), 
    m_readPoint(0), 
    m_data(initialSize, 0)
{}

Packet::~Packet() 
{}

void 
Packet::clear() {
  m_readPoint = 0;
  m_data.clear();
  m_isOk = true;
}

void
Packet::addData(const void* data, SizeT dataSize) {

  if (data && dataSize > 0) {

    auto dataP = static_cast<const Int8*>(data);
    m_data.reserve(m_data.size() + dataSize);
    m_data.insert(m_data.end(), dataP, dataP + dataSize);
  }
}

void 
Packet::addData(std::vector<Int8>& data) {
  
  if (!data.empty()) {
    if (m_data.empty()) {
      m_data = std::move(data);
    }
    else {
      m_data.reserve(m_data.size() + data.size());
      m_data.insert(m_data.end(), data.begin(), data.end());
      data.clear();
    }
  }
}

void 
Packet::getDataPtr(Int8*& buffer, SizeT size) {
  
  buffer = nullptr;
  
  if (size && canRead(size)) {
    buffer = std::next(m_data.data(), m_readPoint);
    m_readPoint += size;
  }
}

//void 
//Packet::removeData(SizeT size) {
//  m_data.erase(m_data.begin(), m_data.begin() + size);
//}

bool
Packet::canRead(SizeT size) {

  m_isOk = m_isOk && (m_readPoint + size) <= m_data.size();

  return m_isOk;
}

}