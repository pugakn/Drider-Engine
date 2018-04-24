#include "dr_udp_socket.h"

namespace driderSDK {

UDPSocket::UDPSocket() : m_blocking(true)
{}

UDPSocket::~UDPSocket() 
{}

void 
UDPSocket::init() {

}

void 
UDPSocket::destroy() {

}

void 
UDPSocket::setBlocking(bool blocking) {
  m_blocking = blocking;
}

void UDPSocket::bind(UInt16 port, const String& ipAddress) {
  
}

void UDPSocket::send(const DataBuffer& data, 
                     UInt16 port, 
                     const String& ipAddress) {

}

void UDPSocket::send(const Packet& packet,
                     UInt16 port, 
                     const String& ipAddress) {

}

void UDPSocket::receive(DataBuffer& buffer, 
                        UInt16 port, 
                        const String& ipAddress) {

}

void UDPSocket::receive(Packet& packet, 
                        UInt16 port, 
                        const String& ipAddress) {

}

}