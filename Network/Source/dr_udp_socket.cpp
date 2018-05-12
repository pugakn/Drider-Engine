#include "dr_udp_socket.h"
#include "dr_network_manager.h"
#include "dr_packet.h"

#ifdef DR_PLATFORM_WINDOWS
#include <WinSock2.h>
#include <WS2tcpip.h>
#endif

namespace driderSDK {

UDPSocket::UDPSocket() 
  : m_blocking(true), 
    m_handle(NetworkManager::getInvalidHandle())
{}

UDPSocket::~UDPSocket() 
{}

void 
UDPSocket::init() {
  m_handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}

void
UDPSocket::destroy() {
  if (isValid()) {
    closesocket(m_handle);
  }
}

bool
UDPSocket::isValid() {
  return NetworkManager::isSocketValid(m_handle);
}

SocketHandle
UDPSocket::getHandle() {
  return m_handle;
}

void
UDPSocket::setBlocking(bool blocking) {
  m_blocking = blocking;
}

void
UDPSocket::bind(UInt16 port, const String& ipAddress) {
  
  if (!isValid()) {
    return;
  }

  sockaddr_in server{};

  server.sin_family = AF_INET;
  server.sin_port = htons(port);

  inet_pton(AF_INET, ipAddress.c_str(), &server.sin_addr);
  
  if (::bind(m_handle, (sockaddr*)&server, sizeof(server)) != 0) {
    m_handle = NetworkManager::getInvalidHandle();
  }
}

bool
UDPSocket::send(const DataBuffer& data,
                UInt16 port,
                const String& ipAddress) {

  sockaddr_in server{};

  server.sin_family = AF_INET;
  server.sin_port = htons(port);

  inet_pton(AF_INET, ipAddress.c_str(), &server.sin_addr);

  Int32 sendBytes = 0;

  if (m_blocking) {
    sendBytes = sendto(m_handle, 
                       reinterpret_cast<const char*>(data.data()), 
                       static_cast<int>(data.size()), 
                       0,
                       (sockaddr*)&server,
                       sizeof(server));
  }
  else {

  }

  return sendBytes != -1;
}

bool
UDPSocket::send(const Packet& packet,
                UInt16 port, 
                const String& ipAddress) {
  return send(packet.getData(), port, ipAddress);
}

Int32
UDPSocket::receive(DataBuffer& buffer, 
                   UInt16& port, 
                   String& ipAddress) {

  sockaddr_in sender{};
  Int32 senderSAS = sizeof(sender);
  Int32 received = 0;

  if (m_blocking) {
    received = recvfrom(m_handle, 
                        reinterpret_cast<char*>(buffer.data()), 
                        static_cast<int>(buffer.size()),
                        0,
                        (sockaddr*)&sender,
                        &senderSAS);
  }
  else {

  }

  if (received != -1) {
    //15 is the maximum size of characters an ip can contain
    ipAddress.resize(15);

    inet_ntop(AF_INET, 
              &sender.sin_addr, 
              const_cast<char*>(ipAddress.c_str()), 
              ipAddress.size());

    port = ntohs(sender.sin_port);

    if (static_cast<Int32>(buffer.size()) < received) {
      //DR_ASSERT(0);
      Int32 x = 0;
    }
  }
    
  return received;
}

Int32
UDPSocket::receive(Packet& packet,
                   Int32 maxBuffSize,
                   UInt16& port, 
                   String& ipAddress) {

  DataBuffer buff(maxBuffSize, 0);

  Int32 received = receive(buff, port, ipAddress);

  if (received != -1) {

    buff.resize(received);

    packet.addData(std::move(buff));
  }
  
  return received;
}

}