#pragma once

#include <vector>

#include <dr_application.h>
#include <dr_udp_socket.h>
#include <dr_packet.h>
#include <dr_packet_handler.h>
#include <dr_connection_enums.h>
#include <dr_timer.h>
#include <dr_message_type.h>
#include <functional>
#include <map>

namespace driderSDK {

class Vector3D;
class GameObject;

class NetworkValue {
  public: 
    NetworkValue();
       
    template<typename T> T
      getValueCasted() {
      return *((T *)m_value);
    }

  public:
    TString m_name;
    PARAM_TYPE::E m_valueType;
    void* m_value;
    
};

class GameServer : public Application, public PacketHandler
{
 public:

  GameServer() : Application(false)
  {}
  
 private:

  struct ClientData
  {
    WString name;
    Timer timeOut;
    UInt32 ip;
    UInt16 port;
    UInt8 ignoredRequests;
  };
     
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

  void 
  onResize(){}

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
  executeFunction(MessageData& msg);

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

  void
  registerVar(MessageData& msg);

  void
  instantiate(MessageData& msg);

 private: 
  void
  instantiatePlayer(const Vector3D& pos,
                    const Vector3D& dir);

  using Command = decltype(&requestNotify);      
  using CommandList = std::vector<std::pair<REQUEST_ID::E, Command>>;
  using FunctionList = std::vector<std::pair<FUNCTION_TYPE::E, Command>>;

  bool m_inGame;
  ClientList m_clients;
  CommandList m_commands;
  FunctionList m_functions;

  UInt32 m_publicIP;
  UInt32 m_localIP;
  SharedSocket m_localSocket;
  SharedSocket m_publicSocket;
  UInt16 m_port;
  Timer m_notifyTimer;
  //Max capacity for clients
  const SizeT m_maxClients = 5;
  //If after 10 secs we haven't recv any msg form the main server we disconnect
  const float m_maxTimeOut = 11.f;
  //Rate for asking active notifications to clients
  const float m_requestActiveRate = 2.5f;
  //Max request that can be ignored util close connection with client
  const UInt8 m_maxIgnoredRequests = 4;

  std::map<const TString, NetworkValue> m_values;

  std::vector<std::shared_ptr<GameObject>> m_players;
  
};

}
