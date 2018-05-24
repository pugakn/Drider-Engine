#pragma once

#include <dr_application.h>
#include <dr_udp_socket.h>
#include <dr_packet.h>
#include <dr_packet_handler.h>

namespace driderSDK {

class GameServer : public Application, public PacketHandler
{
 public:

  GameServer() : Application(false)
  {}
    
  // Inherited via Application
  virtual void
  postInit() override;
  
  virtual void
  postUpdate() override;
  
  virtual void
  postRender() override;
  
  virtual void
  postDestroy() override;

  virtual void 
  processPacket(UDPSocket& socket, 
                Packet& packet, 
                SizeT recvLen, 
                UInt32 senderIP, 
                UInt16 senderPort) override;
 private: 
   
   // Inherited via PacketHandler

};

}
