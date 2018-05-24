#include "dr_packet_handler.h"

#include <algorithm>

#include <dr_connection_enums.h>

namespace driderSDK {

PacketHandler::PacketHandler(SizeT packetSize) 
  : m_packet(packetSize)
{
  DR_ASSERT(packetSize >= 128);
}

void 
PacketHandler::removeSockets() {
  m_sockets.clear();
}

void 
PacketHandler::checkIncomingPackets() {

  UInt32 senderIP;
  UInt16 senderPort;
  Int32 recvLen;

  for (auto& socket : m_sockets) {
    if ((socket->receive(m_packet, recvLen, senderIP, senderPort) == 
         SOCKET_ERR::kSuccess) && shouldUsePacket(m_packet)) {

      processPacket(*socket, 
                    m_packet, 
                    static_cast<SizeT>(recvLen),
                    senderIP, 
                    senderPort);
    }

    m_packet.resetReadPos();
  } 
}

bool 
PacketHandler::shouldUsePacket(Packet& packet) {
  
  UInt32 m = 0;

  packet >> m;

  if (m == VER_NUM::kN) {
    return true;
  }

  return false;
}

}