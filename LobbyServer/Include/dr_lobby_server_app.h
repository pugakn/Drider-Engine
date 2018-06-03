#pragma once

#include <stack>
#include <vector>

#include <dr_application.h>
#include <dr_connection_enums.h>
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
    Timer timeOut;
    UInt32 publicIP;
    UInt16 assignedPort;
    UInt16 localPort;
    UInt8 ignoredRequests;
    bool canJoin;
  };
  
  virtual void 
  processPacket(MessageData& msg) override;

  void 
  portRequest(MessageData& msg);

  void 
  closeServer(MessageData& msg);

  void
  serverFull(MessageData& msg);

  void
  serverNotFull(MessageData& msg);

  void
  lobbiesRequest(MessageData& msg);

  void
  notifyActive(MessageData& msg);

  void 
  setServerJoinStatus(UInt16 port, bool canJoin);

  void 
  checkActiveServerStatus();

  void 
  requestActiveNotify(const GameServerData& gsd);

 private:

  using Command = decltype(&processPacket);
  using CommandList = std::vector<std::pair<REQUEST_ID::E, Command>>;
  using ServerList = std::vector<GameServerData>;

  //Find by ip and port
  ServerList::iterator
  findServer(UInt16 port);

  UInt32 m_localIP;
  CommandList m_commands;
  std::stack<UInt16> m_unusedPorts;
  ServerList m_servers;
  const float m_requestActiveRate = 3.5f;
  //Max number of requests ignored until disconnect
  const UInt8 m_maxIgnoredRequest = 3;
};

}
