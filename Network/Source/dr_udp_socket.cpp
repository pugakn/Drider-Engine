#include "dr_udp_socket.h"
#include "dr_network_manager.h"
#include "dr_packet.h"

namespace driderSDK {

UDPSocket::UDPSocket() 
{}

UDPSocket::~UDPSocket() 
{}

void
UDPSocket::create() {
  m_handle = NetworkManager::createSocket(SOCKET_TYPE::kUDP);
}

bool
UDPSocket::bind(const String& ipAddress, UInt16 port) {
  
  if (!isValid()) {
    return false;
  }

  sockaddr_in server = NetworkManager::getAddress(ipAddress, port);
  
  if (::bind(m_handle, (sockaddr*)&server, sizeof(server)) != 0) {
    m_handle = NetworkManager::getInvalidHandle();

    return false;
  }

  return true;
}

SOCKET_ERR::E
UDPSocket::send(const DataBuffer& data,
                UInt16 port,
                const String& ipAddress) {

  sockaddr_in server = NetworkManager::getAddress(ipAddress, port);

  Int32 sendBytes = 0;

  sendBytes = sendto(m_handle, 
                     reinterpret_cast<const char*>(data.data()), 
                     static_cast<int>(data.size()), 
                     0,
                     (sockaddr*)&server,
                     sizeof(server));

  return sendBytes != -1 ? SOCKET_ERR::kSuccess : SOCKET_ERR::kError;
}

SOCKET_ERR::E
UDPSocket::send(const Packet& packet,
                UInt16 port, 
                const String& ipAddress) {
  return send(packet.getData(), port, ipAddress);
}

SOCKET_ERR::E
UDPSocket::receive(DataBuffer& buffer, 
                   Int32& receivedLen,
                   UInt16& port, 
                   String& ipAddress) {

  sockaddr_in sender{};
  Int32 senderSAS = sizeof(sender);
  receivedLen = 0;

  receivedLen = recvfrom(m_handle, 
                         reinterpret_cast<char*>(buffer.data()), 
                         static_cast<int>(buffer.size()),
                         0,
                         (sockaddr*)&sender,
                         &senderSAS);

  auto result = SOCKET_ERR::kSuccess;

  if (receivedLen != SOCKET_ERR::kError) {
    
    NetworkManager::getAddrPort(sender, ipAddress, port);

    if (static_cast<Int32>(buffer.size()) < receivedLen) {
      //DR_ASSERT(0);
      //Int32 x = 0;
    }
  }
  else {
    result = NetworkManager::getSocketError();  
  }
      
  return result;
}

SOCKET_ERR::E
UDPSocket::receive(Packet& packet,
                   Int32 maxBuffSize,
                   Int32& receivedLen,
                   UInt16& port, 
                   String& ipAddress) {

  DataBuffer buff(maxBuffSize, 0);

  auto result = receive(buff, receivedLen, port, ipAddress);

  if (receivedLen != -1) {

    buff.resize(receivedLen);

    packet.addData(std::move(buff));
  }
  
  return result;
}

}