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

   /**
   * Tests if the socket is on a valid state.
   *
   * @return 
   *   True if the socket is valid, false otherwise.
   */
   bool
   isValid();

   /**
   * Retreives the handle for the socket
   * 
   * @return
   *   The handle of the socket.
   */
   SocketHandle
   getHandle();
  
   /**
   * Sets the mode of the socket.
   */
   void 
   setBlocking(bool blocking);

   /**
   * Binds the socket to a specific port and address. 
   * Used only by the server receiver socket.
   * 
   * @param port
   *  The port of the server.
   *
   * @param ipAddress 
   *  The address of the server.
   */
   void 
   bind(UInt16 port, const String& ipAddress);

   bool
   send(const DataBuffer& data, UInt16 port, const String& ipAddress);

   bool 
   send(const Packet& packet, UInt16 port, const String& ipAddress);

   Int32
   receive(DataBuffer& buffer, UInt16& port, String& ipAddress);

   Int32
   receive(Packet& packet, Int32 maxBuffSize, UInt16& port, String& ipAddress);
 private:
   bool m_blocking;
   SocketHandle m_handle;
};

}
