#pragma once

#include <vector>
#include <memory>
#include "dr_network_prerequisites.h"
#include "dr_packet.h"
#include "dr_udp_socket.h"

namespace driderSDK {

class DR_NETWORK_EXPORT PacketHandler
{
 public:

  struct MessageData
  {
    UDPSocket& socket;
    Packet& packet;
    SizeT size;
    UInt32 senderIP;
    UInt16 senderPort;
  };

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
  processPacket(MessageData& data) = 0;

  
 protected:
  std::vector<SharedSocket> m_sockets;
 private:
  Packet m_packet;
};

}