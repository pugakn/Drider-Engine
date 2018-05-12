#pragma once

#include "dr_network_prerequisites.h"
#include "dr_socket.h"

namespace driderSDK {

class Packet;

class DR_NETWORK_EXPORT UDPSocket : public Socket
{
 public:
   UDPSocket();

   ~UDPSocket();

   void 
   create();
      
   /**
   * Binds the socket to a specific port and address. 
   * Used only by the server receiver socket.
   * 
   * @param port
   *  The port of the server.
   *
   * @param ipAddress 
   *  The address of the server.
   * 
   * @return
   *  Returns the result of the bind operation, true if it was
   *  successful, false otherwise.
   */
   bool 
   bind(const String& ipAddress, UInt16 port);

   SOCKET_ERR::E
   send(const DataBuffer& data, UInt16 port, const String& ipAddress);

   SOCKET_ERR::E
   send(const Packet& packet, UInt16 port, const String& ipAddress);

   SOCKET_ERR::E
   receive(DataBuffer& buffer, 
           Int32& recievedLen, 
           UInt16& port, 
           String& ipAddress);

   SOCKET_ERR::E
   receive(Packet& packet, 
           Int32 maxBuffSize, 
           Int32& recievedLen, 
           UInt16& port, 
           String& ipAddress);
};

}
