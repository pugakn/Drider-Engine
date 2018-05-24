#pragma once

#include <vector>

#include <dr_application.h>
#include <dr_udp_socket.h>
#include <dr_packet.h>
#include <dr_timer.h>
#include <dr_packet_handler.h>

namespace driderSDK {

class Packet;

class LobbyServerApp : public Application, public PacketHandler
{
 public: 
  LobbyServerApp();

  ~LobbyServerApp();

  virtual void 
  postInit() override;

  virtual void
  postUpdate() override;

  virtual void 
  postRender() override;

  virtual void 
  postDestroy() override;

 private:

  struct GameServerData
  {

  };
  
  virtual void 
  processPacket(UDPSocket& socket, 
                Packet& packet, 
                SizeT recvLen, 
                UInt32 senderIP, 
                UInt16 senderPort) override;

 private:
};

}
