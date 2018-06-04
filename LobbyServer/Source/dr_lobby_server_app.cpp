#include "dr_lobby_server_app.h"

#include <algorithm>
#include <iostream>

#include <dr_network_manager.h>
#include <dr_packet.h>
#include <dr_string_utils.h>
#include <dr_udp_socket.h>

namespace driderSDK {

LobbyServerApp::LobbyServerApp() 
  : Application(false)
{}

LobbyServerApp::~LobbyServerApp() 
{}

void 
LobbyServerApp::postInit() {
  
  NetworkManager::startUp();

  const Int32 numServers = 3;

  for(Int32 i = 0; i < numServers; ++i) {
    m_unusedPorts.push(Ports::GAME_SERVER + i);
  }

  m_commands.emplace_back(REQUEST_ID::kPortRequest, &LobbyServerApp::portRequest);
  m_commands.emplace_back(REQUEST_ID::kCloseServer, &LobbyServerApp::closeServer);
  m_commands.emplace_back(REQUEST_ID::kServerFull, &LobbyServerApp::serverFull);
  m_commands.emplace_back(REQUEST_ID::kServerNotFull, &LobbyServerApp::serverNotFull);
  m_commands.emplace_back(REQUEST_ID::kLobbiesRequest, &LobbyServerApp::lobbiesRequest);
  m_commands.emplace_back(REQUEST_ID::kNotifyActive, &LobbyServerApp::notifyActive);
  
  auto getIP = [](const TString& ip) -> UInt32
  {
    return NetworkManager::ipAddrStrToUInt(ip);
  };
  
  auto getPublicIP = []() -> TString
  {
    auto sa = NetworkManager::getAddress(_T(""), 0);
    TString ip;
    UInt16 port;
    NetworkManager::getAddrPort(sa, ip, port);
    return std::move(ip);
  };
     
  m_localIP = getIP(TString(_T("127.0.0.1")));

  for (const auto& ip : {m_localIP, getIP(getPublicIP())}) {

    auto socket = std::make_shared<UDPSocket>();

    socket->create();
  
    if (!socket->bind(ip, Ports::LOBBY_SERVER)) {
      m_running = false;
    }

    socket->setBlockMode(false);
    
    m_sockets.push_back(socket);
  }
}

void 
LobbyServerApp::postUpdate() {
  checkIncomingPackets();
  checkActiveServerStatus();
}

void 
LobbyServerApp::postRender() {
}

void 
LobbyServerApp::postDestroy() {
  removeSockets();
  NetworkManager::shutDown();
}

void 
LobbyServerApp::processPacket(MessageData& msg) {

  UInt32 request = 0;

  msg.packet >> request;

  //std::cout << "New pacakage with request code: " << request << std::endl;

  auto it = std::find_if(m_commands.begin(), m_commands.end(), 
                         [request](const CommandList::value_type& pair) 
                         { return request == pair.first; });
  
  if (it != m_commands.end()) {
    (this->*(*it).second)(msg);
  }
}

void 
LobbyServerApp::portRequest(MessageData& msg) {
  
  if (!m_unusedPorts.empty() && msg.senderIP && m_localIP) {
    UInt32 host = 0;
    msg.packet >> host;
    auto str = NetworkManager::ipAddrUIntToStr(host);
    //Se asume que el mensaje le llego al server xd
    UInt16 port = m_unusedPorts.top();
    m_unusedPorts.pop();
    Packet pack;
    pack << VER_NUM::kN;
    pack << REQUEST_ID::kServerAccepted;
    pack << port;
    msg.socket.send(pack, msg.senderIP, msg.senderPort);

    GameServerData newServer;
    newServer.publicIP = host;
    newServer.assignedPort = port;
    newServer.localPort = msg.senderPort;
    newServer.canJoin = true;
    newServer.ignoredRequests = 0;
    newServer.timeOut.init();
    
    std::cout << "New server connected ip: "
              << StringUtils::toString(NetworkManager::ipAddrUIntToStr(host))
              << " port: " << port << std::endl;

    m_servers.push_back(newServer);
  }
}

void 
LobbyServerApp::closeServer(MessageData& msg) {
  
  UInt32 ip = 0;
  UInt16 port = 0;

  port = msg.senderPort;

  std::cout << "Server with ip: " 
              << StringUtils::toString(NetworkManager::ipAddrUIntToStr(ip))
              << " and port: " << port << "closed connection." << std::endl;

  auto it = findServer(port);

  //assert(it != m_servers.end());
  if (it != m_servers.end()) {

    m_unusedPorts.push(it->assignedPort);

    m_servers.erase(it);  
  }
}

void 
LobbyServerApp::serverFull(MessageData& msg) {
  setServerJoinStatus(msg.senderPort, true);  
}

void 
LobbyServerApp::serverNotFull(MessageData& msg) {
  setServerJoinStatus(msg.senderPort, false);  
}

void 
LobbyServerApp::lobbiesRequest(MessageData& msg) {
  
  Packet lobbiesMsg;

  UInt32 count = std::count_if(m_servers.begin(), m_servers.end(), 
                               [](const GameServerData& gsd){
                                  return gsd.canJoin;
                               });
  lobbiesMsg << VER_NUM::kN;
  lobbiesMsg << REQUEST_ID::kLobbiesList;
  lobbiesMsg << count;

  for (auto& server : m_servers) {
    if (server.canJoin) {
      lobbiesMsg << server.publicIP;
      lobbiesMsg << server.assignedPort;
    }
  }

  msg.socket.send(lobbiesMsg, msg.senderIP, msg.senderPort);
}

void 
LobbyServerApp::notifyActive(MessageData& msg) {

  auto it = findServer(msg.senderPort);

  if (it != m_servers.end()) {
    it->timeOut.init();
    it->ignoredRequests = 0;
  }
}

void 
LobbyServerApp::setServerJoinStatus(UInt16 port, bool canJoin) {
  auto it = findServer(port);

  if (it != m_servers.end()) {
    it->canJoin = canJoin;
  }
}

void 
LobbyServerApp::checkActiveServerStatus() {
  
  for (auto server = m_servers.begin(); server != m_servers.end();) {

    float time = server->timeOut.getSeconds();
    
    //Remove server
    if (server->ignoredRequests >= m_maxIgnoredRequest) {
      std::cout << "Server disconnected: " << 
                   server->publicIP << " port:" << server->assignedPort << std::endl;
      m_unusedPorts.push(server->assignedPort);
      server = m_servers.erase(server);
      continue;
    }

    //After m_requestActiveRate this will spam the server
    if (time >= m_requestActiveRate) {
      server->ignoredRequests++;
      server->timeOut.init();
      std::cout << "Requested active status to: " << 
                   server->publicIP << " port:" << server->assignedPort << std::endl;
      requestActiveNotify(*server);
    }  

    ++server;
  }
}

void 
LobbyServerApp::requestActiveNotify(const GameServerData& gsd) {

  Packet notifyReq;
  notifyReq << VER_NUM::kN;
  notifyReq << REQUEST_ID::kRequestActive;

  m_sockets[0]->send(notifyReq, m_localIP, gsd.localPort);
}

LobbyServerApp::ServerList::iterator 
LobbyServerApp::findServer(UInt16 port) {
  return std::find_if(m_servers.begin(), m_servers.end(), 
                      [port](const GameServerData& gsd)
                      {
                        return gsd.localPort == port;
                      });
}

}