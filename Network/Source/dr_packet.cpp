#include "dr_packet.h"
#include <iterator>

namespace driderSDK {

Packet::Packet() : m_isOk(true)
{}

Packet::~Packet() 
{}

void 
Packet::addData(const void* data, SizeT dataSize) {

  if (data && dataSize > 0) {

    auto dataP = static_cast<const Int8*>(data);

    m_data.insert(m_data.end(), dataP, dataP + dataSize);
  }
}

void 
Packet::addData(std::vector<Int8>&& data) {
  if (m_data.empty()) {
    m_data = std::move(data);
  }
  else {
    m_data.reserve(m_data.size() + data.size());
    std::move(m_data.begin(), m_data.end(), std::back_inserter(m_data));
    data.clear();
  }
}

void 
Packet::extractData(Int8*& buffer, SizeT size) {
  
  buffer = nullptr;
  
  if (canRead(size)) {
    buffer = m_data.data();
  }
}

void 
Packet::removeData(SizeT size) {
  m_data.erase(m_data.begin(), m_data.begin() + size);
}

bool
Packet::canRead(SizeT size) {
  m_isOk = m_isOk && m_data.size() >= size;

  return m_isOk;
}

}