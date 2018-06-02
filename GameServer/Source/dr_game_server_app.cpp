#include "dr_game_server_app.h"

#include <algorithm>
#include <iostream>

#include <dr_connection_enums.h>
#include <dr_network_manager.h>
#include <dr_string_utils.h>

namespace driderSDK {

void 
GameServer::postInit() {

  NetworkManager::startUp();

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

  m_commands.emplace_back(REQUEST_ID::kClientJoin, &GameServer::clientJoin);
  m_commands.emplace_back(REQUEST_ID::kClientLeave, &GameServer::clientLeave);
  m_commands.emplace_back(REQUEST_ID::kChatMsg, &GameServer::chatMsg);
  m_commands.emplace_back(REQUEST_ID::kNotifyActive, &GameServer::notifyActive);
  m_commands.emplace_back(REQUEST_ID::kServerAccepted, &GameServer::serverAccepted);
  m_commands.emplace_back(REQUEST_ID::kRequestActive, &GameServer::requestNotify);

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
  if (m_notifyTimer.getSeconds() >= m_maxTimeOut) {
    m_running = false;
    return;
  }
  checkIncomingPackets();
}

void 
GameServer::postRender() {
}

void 
GameServer::postDestroy() {
  NetworkManager::shutDown();
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

    msg.packet >> client.ip;
    msg.packet >> client.port;
    msg.packet >> client.name;

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

  String data;
  String who;

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
GameServer::notifyActive(MessageData& msg) {

  auto client = findClient(msg.senderIP, msg.senderPort);

  if (client != m_clients.end()) {
    client->timeOut.init();
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
    if (time >= m_maxTimeOut) {
      std::cout << "Client disconnected: " << 
                   client->ip << " port:" << client->port << std::endl;

      m_clients.erase(client);

      if(!m_inGame) {
        notifyServerCapacityFull(false);
      }
      
      continue;
    }
    //After m_requestActiveRate this will spam the server
    else if (time >= m_requestActiveRate && 
             std::fmod(time, m_requestActiveRate) < 0.005f) { 
      std::cout << "Requested active status to: " << 
                   client->ip << " port:" << client->port << std::endl;

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

}