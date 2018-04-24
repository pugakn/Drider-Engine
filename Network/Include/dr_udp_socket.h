#pragma once

#include <vector>

#include "dr_network_prerequisites.h"
#include "dr_socket_handle.h"

namespace driderSDK {

class Packet;

namespace SOCKET_STATUS
{
  enum E
  {
    Good,
    Error
  };
}

class DR_NETWORK_EXPORT UDPSocket
{
 public:
   using DataBuffer = std::vector<Int8>;

   UDPSocket();

   ~UDPSocket();

   void 
   init();

   void
   destroy();

   void 
   setBlocking(bool blocking);

   void 
   bind(UInt16 port, const String& ipAddress);

   void
   send(const DataBuffer& data, UInt16 port, const String& ipAddress);

   void 
   send(const Packet& packet, UInt16 port, const String& ipAddress);

   void
   receive(DataBuffer& buffer, UInt16 port, const String& ipAddress);

   void
   receive(Packet& packet, UInt16 port, const String& ipAddress);
 private:
   bool m_blocking;
   SocketHandle m_handle;
};

}
