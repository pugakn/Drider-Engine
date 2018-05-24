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
UDPSocket::bind(UInt32 ipAddress, UInt16 port) {
  
  if (!isValid()) {
    return false;
  }

  //m_bindAddress.clear();
  m_bindAddress = 0;
  m_bindPort = 0;

  sockaddr_in server = NetworkManager::getAddress(ipAddress, port);
  
  if (::bind(m_handle, (sockaddr*)&server, sizeof(server)) != 0) {
    m_handle = NetworkManager::getInvalidHandle();
    m_bindAddress = 0;
    m_bindPort = 0;
    return false;
  }

  NetworkManager::getAddrPort(server, m_bindAddress, m_bindPort);

  return true;
}

SOCKET_ERR::E
UDPSocket::send(const DataBuffer& data,
                UInt32 ipAddress, 
                UInt16 port) {

  if (!isValid()) {
    return SOCKET_ERR::kUninitialized;
  }

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
                UInt32 ipAddress,
                UInt16 port) {
  return send(packet.getData(), ipAddress, port);
}

SOCKET_ERR::E
UDPSocket::receive(DataBuffer& buffer, 
                   Int32& receivedLen,
                   UInt32& ipAddress,
                   UInt16& port) {

  if (!isValid()) {
    return SOCKET_ERR::kUninitialized;
  }

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
                   UInt32& ipAddress, 
                   UInt16& port) {

  DataBuffer buff(maxBuffSize, 0);

  auto result = receive(buff, receivedLen, ipAddress, port);

  if (receivedLen != -1) {

    buff.resize(receivedLen);

    packet.addData(buff);
  }
  
  return result;
}

SOCKET_ERR::E 
UDPSocket::receive(Packet& packet, 
                   Int32& receivedLen, 
                   UInt32& ipAddress, 
                   UInt16& port) {
      
  return receive(packet.m_data, receivedLen, ipAddress, port);
}

}