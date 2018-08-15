#include "dr_game_server_app.h"

#include <algorithm>
#include <iostream>

#include <dr_connection_enums.h>
#include <dr_network_manager.h>
#include <dr_string_utils.h>

#include <dr_graph.h>
#include <dr_time.h>
#include <dr_context_manager.h>
#include <dr_gameObject.h>
#include <dr_vector3d.h>

#include <dr_aabb_collider.h>

namespace driderSDK {

NetworkValue::NetworkValue() {

}

void 
GameServer::postInit() {
  
  Logger::startUp();
  NetworkManager::startUp();
  Time::startUp();
  ContextManager::startUp();
  ScriptEngine::startUp();
  SceneGraph::startUp();

  SceneGraph::start();

  auto getPublicIP = []() -> TString
  {
    auto sa = NetworkManager::getAddress(_T(""), 0);
    TString ip;
    UInt16 port;
    NetworkManager::getAddrPort(sa, ip, port);
    return std::move(ip);
  };

  m_inGame = false;

  m_publicIP = NetworkManager::ipAddrStrToUInt(getPublicIP());
  m_localIP = NetworkManager::ipAddrStrToUInt(_T("127.0.0.1"));

  m_publicIP = m_localIP;

  m_commands.emplace_back(REQUEST_ID::kClientJoin, &GameServer::clientJoin);
  m_commands.emplace_back(REQUEST_ID::kClientLeave, &GameServer::clientLeave);
  m_commands.emplace_back(REQUEST_ID::kChatMsg, &GameServer::chatMsg);
  m_commands.emplace_back(REQUEST_ID::kNotifyActive, &GameServer::notifyActive);
  m_commands.emplace_back(REQUEST_ID::kServerAccepted, &GameServer::serverAccepted);
  m_commands.emplace_back(REQUEST_ID::kRequestActive, &GameServer::requestNotify);
  m_commands.emplace_back(REQUEST_ID::kExecuteFunction, &GameServer::executeFunction);

  m_functions.emplace_back(FUNCTION_TYPE::RegisterVar, &GameServer::registerVar);
  m_functions.emplace_back(FUNCTION_TYPE::Instantiate, &GameServer::instantiate);

  m_localSocket = std::make_shared<UDPSocket>();

  m_localSocket->create();
  
  m_localSocket->setBlockMode(false);

  m_sockets.push_back(m_localSocket);
  
  m_sockets.push_back(std::make_shared<UDPSocket>());

  m_publicSocket = m_sockets.back();

  Packet createdPacket;
  createdPacket << VER_NUM::kN;
  createdPacket << REQUEST_ID::kPortRequest;
  createdPacket << m_publicIP;

  m_localSocket->send(createdPacket, m_localIP, Ports::LOBBY_SERVER);

  const float timeOut = 5.f;

  Timer responseTimeOut;

  responseTimeOut.init();

  while(responseTimeOut.getSeconds() < timeOut) {
    
    checkIncomingPackets();

    if (m_publicSocket->isValid()) {

      std::cout << "Established Connection" << std::endl;

      m_notifyTimer.init();

      return;
    }
  }

  std::cout << "Connection Failed" << std::endl;

  m_running = false;
}

void 
GameServer::postUpdate() {
  /*if (m_notifyTimer.getSeconds() >= m_maxTimeOut) {
    m_running = false;
    return;
  }*/
  checkIncomingPackets();
  checkClientsActiveState();
  Time::update();
  SceneGraph::update();
}

void 
GameServer::postRender() {
}

void 
GameServer::postDestroy() {
  removeSockets();
  NetworkManager::shutDown();
  SceneGraph::shutDown();
  Time::shutDown();
}

void 
GameServer::processPacket(MessageData& msg) {
 
  UInt32 request = 0;

  msg.packet >> request;

  auto it = std::find_if(m_commands.begin(), m_commands.end(), 
                         [request](const CommandList::value_type& pair) 
                         { return request == pair.first; });
  
  if (it != m_commands.end()) {

    auto accepted = m_publicSocket->isValid();

    if ((request != REQUEST_ID::kServerAccepted && accepted) ||
         request == REQUEST_ID::kServerAccepted && !accepted) {
      (this->*(*it).second)(msg);
    }

  }
}

void 
GameServer::clientJoin(MessageData& msg) {

  //Check for capacity and disponibility

  Packet response;
  response << VER_NUM::kN;
  
  if (!m_inGame && m_clients.size() < m_maxClients) {
    response << REQUEST_ID::kJoinAccepted;

    ClientData client;

    /*msg.packet >> client.ip;
    msg.packet >> client.port;*/
    msg.packet >> client.name;

    client.ip = msg.senderIP;
    client.port = msg.senderPort;
    client.ignoredRequests = 0;

    std::cout << "New client joined: " << client.ip << " " << client.port << std::endl;
   
    client.timeOut.init();

    m_clients.push_back(std::move(client));

    if (m_clients.size() == m_maxClients) {
      notifyServerCapacityFull(true);
    }
  }
  else {
    response << REQUEST_ID::kJoinDenied;
    notifyServerCapacityFull(true);
  }
  
  msg.socket.send(response, msg.senderIP, msg.senderPort);
}

void 
GameServer::clientLeave(MessageData& msg) {
 
  auto client = findClient(msg.senderIP, msg.senderPort);

  if (client != m_clients.end()) {

    std::cout << "Client leaved: " << client->ip << " " << client->port << std::endl;

    m_clients.erase(client);

    //In chat lobby
    if(!m_inGame) {
      notifyServerCapacityFull(false);
    }
  }
}

void 
GameServer::serverAccepted(MessageData& msg) {

  //Bytes count 
  //
  m_port = 0;

  msg.packet >> m_port;

  if (m_port) {
        
    m_publicSocket->create();
    m_publicSocket->bind(m_publicIP, m_port);
    m_publicSocket->setBlockMode(false);

    std::cout << "Server open on ip: " 
              << StringUtils::toString(NetworkManager::ipAddrUIntToStr(m_publicIP))
              << " port:" << m_port << std::endl;
  }
}

void 
GameServer::requestNotify(MessageData& msg) {
  
  Packet notify;

  notify << VER_NUM::kN;
  notify << REQUEST_ID::kNotifyActive;
  //notify << m_port;

  m_localSocket->send(notify, m_localIP, Ports::LOBBY_SERVER);

  m_notifyTimer.init();
}

void 
GameServer::chatMsg(MessageData& msg) {

  WString data;
  WString who;

  msg.packet >> who;
  msg.packet >> data;

  Packet pack;

  pack << VER_NUM::kN;
  pack << REQUEST_ID::kChatMsg;
  pack << who;
  pack << data;

  broadcastMessage(pack);
}

void
GameServer::executeFunction(MessageData& msg) {
  //std::cout << "Execute function" << std::endl;
  
  UInt32 functionType = 0;

  msg.packet >> functionType;

  auto it = std::find_if(m_functions.begin(), m_functions.end(),
                         [functionType](const FunctionList::value_type& pair)
  { return functionType == pair.first; });

  //std::cout << "FunctionType:" << functionType << std::endl;

  if (it != m_functions.end()) {
    (this->*(*it).second)(msg);
  } else {
    std::cout << "Error function type not found" << std::endl;
  }

}
void 
GameServer::notifyActive(MessageData& msg) {

  auto client = findClient(msg.senderIP, msg.senderPort);

  if (client != m_clients.end()) {
    client->timeOut.init();
    client->ignoredRequests = 0;
  }

}

void 
GameServer::notifyServerCapacityFull(bool full) {

  Packet pack;
  pack << VER_NUM::kN;
  pack << (full ? REQUEST_ID::kServerFull : REQUEST_ID::kServerNotFull);

  m_localSocket->send(pack, m_localIP, Ports::LOBBY_SERVER);

}

void 
GameServer::broadcastMessage(Packet& pack) {
  for (auto& client : m_clients) {
    m_publicSocket->send(pack, client.ip, client.port);
  }
}

void 
GameServer::checkClientsActiveState() {

  for (auto client = m_clients.begin(); client != m_clients.end();) {

    float time = client->timeOut.getSeconds();

    //Remove server
    if (client->ignoredRequests >= m_maxIgnoredRequests) {
      std::cout << "Client loss connection: " << 
                   client->ip << " port:" << client->port << std::endl;

      client = m_clients.erase(client);

      if(!m_inGame) {
        notifyServerCapacityFull(false);
      }
      
      continue;
    }
    //After m_requestActiveRate this will spam the server
    if (time >= m_requestActiveRate) { 

      client->ignoredRequests++;
      client->timeOut.init();

      /*std::cout << "Requested active status to: " << 
                   client->ip << " port:" << client->port << std::endl;*/

      Packet pack;

      pack << VER_NUM::kN;
      pack << REQUEST_ID::kRequestActive;
      
      m_publicSocket->send(pack, client->ip, client->port);
    }

    ++client;
  }

}

GameServer::ClientList::iterator
GameServer::findClient(UInt32 ip, UInt16 port) {
  return std::find_if(m_clients.begin(), m_clients.end(),
                      [ip, port](const ClientData& client) 
                      { return ip == client.ip && port == client.port; });
}

void
GameServer::registerVar(MessageData& msg) {
  //std::cout << "RegisterVar" << std::endl;
  
  TString objName;
  msg.packet >> objName;

  PARAM_TYPE::E type;
  msg.packet >> type;

  TString varName;
  msg.packet >> varName;

  NetworkValue netValue;
  if(type == PARAM_TYPE::FLOAT) {
    float value;
    msg.packet >> value;
    netValue.m_value = &value;
  }
  else if(type == PARAM_TYPE::INT) {
    UInt32 value;
    msg.packet >> value;
    netValue.m_value = &value;
  }
  else if (type == PARAM_TYPE::STRING) {
    TString value;
    msg.packet >> value;
    netValue.m_value = &value;
  }

  netValue.m_name = varName;
  netValue.m_valueType = type;

  m_values.emplace(objName, netValue);

  //std::cout << " Value: " << netValue.getValueCasted<float>() << std::endl;
}

void
GameServer::instantiate(MessageData& msg) {

  TString name;
  msg.packet >> name;
  
  OBJ_TYPE::E objType;
  msg.packet >> objType;
  
  Vector3D position;
  msg.packet >> position;
  
  Vector3D direction;
  msg.packet >> direction;

  if(objType == OBJ_TYPE::kPlayer) {
    instantiatePlayer(position, direction);
  }
  
}

void
GameServer::instantiatePlayer(const Vector3D& pos,
                              const Vector3D& dir) {

  auto newPlayer = SceneGraph::createObject(_T("Player" + m_players.size()));
  newPlayer->getTransform().setPosition(pos);
  newPlayer->getTransform().setRotation(dir);
  SceneGraph::start();

  /*Packet pack;

  pack << VER_NUM::kN;
  pack << REQUEST_ID::kExecuteFunction;
  pack << FUNCTION_TYPE::Instantiate;
  pack << newPlayer->getName();
  pack << OBJ_TYPE::kPlayer;
  pack << pos;
  pack << dir;*/

  m_players.push_back(newPlayer); 

  for(int i = 0; i < m_players.size(); i++) {
    
    Packet pack;
    pack << VER_NUM::kN;
    pack << REQUEST_ID::kExecuteFunction;
    pack << FUNCTION_TYPE::Instantiate;
    pack << m_players[i]->getName();
    pack << OBJ_TYPE::kPlayer;
    pack << m_players[i]->getTransform().getPosition();
    pack << Vector3D(0.0,0.0,0.0);

    broadcastMessage(pack);
  }
}

}