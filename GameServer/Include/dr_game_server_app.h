#pragma once

#include <vector>

#include <dr_application.h>
#include <dr_udp_socket.h>
#include <dr_packet.h>
#include <dr_packet_handler.h>
#include <dr_connection_enums.h>
#include <dr_timer.h>

namespace driderSDK {

class GameServer : public Application, public PacketHandler
{
 public:

  GameServer() : Application(false)
  {}
  
 private:

  struct ClientData
  {
    String name;
    UInt32 ip;
    UInt16 port;
    Timer timeOut;
  };
   
  using Command = decltype(&processPacket);      
  using CommandList = std::vector<std::pair<REQUEST_ID::E, Command>>;
  using ClientList = std::vector<ClientData>;

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
  processPacket(MessageData& msg) override;

  void 
  clientJoin(MessageData& msg);

  void 
  clientLeave(MessageData& msg);

  void
  serverAccepted(MessageData& msg);

  void 
  requestNotify(MessageData& msg);

  void 
  chatMsg(MessageData& msg);

  void
  notifyActive(MessageData& msg); //Client

  void
  notifyServerCapacityFull(bool full);

  void
  broadcastMessage(Packet& pack);

  void 
  checkClientsActiveState();

  ClientList::iterator
  findClient(UInt32 ip, UInt16 port);

 private: 

   bool m_inGame;
   ClientList m_clients;
   CommandList m_commands;
   UInt32 m_publicIP;
   UInt32 m_localIP;
   SharedSocket m_localSocket;
   SharedSocket m_publicSocket;
   UInt16 m_port;
   Timer m_notifyTimer;
   //Max capacity for clients
   const SizeT m_maxClients = 10;
   //Rate for asking active notifications to clients
   const float m_requestActiveRate = 5.f;
   //Max time after disconect a client
   const float m_clientTimeOut = 11.f;
   //If after 10 secs we haven't recv any msg form the main server we disconnect
   const float m_maxTimeOut = 11.f;
};

}
