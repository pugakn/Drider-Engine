#pragma once

#include <vector>

#include "dr_network_prerequisites.h"
#include "dr_packet.h"
#include "dr_udp_socket.h"

namespace driderSDK {

class PacketHandler
{
 public:

  using SharedSocket = std::shared_ptr<UDPSocket>;

  PacketHandler(SizeT packetSize = 256);

  virtual ~PacketHandler()
  {}

  /*void 
  addSocket(SharedSocket socket);

  void 
  removeSocket(SharedSocket socket);*/

  void
  removeSockets();

  void 
  checkIncomingPackets();

  virtual bool
  shouldUsePacket(Packet& packet);

  virtual void 
  processPacket(UDPSocket& socket,
                Packet& packet,
                SizeT recvLen,
                UInt32 senderIP, 
                UInt16 senderPort) = 0;

  
 protected:
  std::vector<SharedSocket> m_sockets;
  Packet m_packet;
};

}