#include "dr_client.h"

#include <algorithm>

#include <dr_network_manager.h>

namespace driderSDK {

void
Client::init(const TString& globalServer) {
  m_socket = std::make_shared<UDPSocket>();
  m_socket->create();
  m_socket->setBlockMode(false);

  m_globalServer.ip = NetworkManager::ipAddrStrToUInt(globalServer);
  m_globalServer.port = Ports::LOBBY_SERVER;

  m_sockets.push_back(m_socket);

  m_lobbyCommands.emplace_back(REQUEST_ID::kLobbiesList, &Client::lobbiesList);
  m_lobbyCommands.emplace_back(REQUEST_ID::kJoinAccepted, &Client::joinAccepted);
  m_lobbyCommands.emplace_back(REQUEST_ID::kJoinDenied, &Client::joinDenied);

  //m_gameCommands.emplace_back(REQUEST_ID::kConnectionLoss, &Client::connectionLoss);
  m_gameCommands.emplace_back(REQUEST_ID::kRequestActive, &Client::requestActive);
  m_gameCommands.emplace_back(REQUEST_ID::kReceiveGameStatus, &Client::receiveGameStatus);

  m_gameCommands.emplace_back();

  //Filter commands to receive only lobby commands 
  m_commands = &m_lobbyCommands;

}

void
Client::update() {
  checkIncomingPackets();
  checkConnectionStatus();
}

void
Client::quit() {
  removeSockets();
}

void
Client::processPacket(MessageData& msg) {

  UInt32 request = 0;

  msg.packet >> request;

  auto command = std::find_if(m_commands->begin(), m_commands->end(),
                              [request](const CommandList::value_type& command)
  { return command.first == request; });

  if (command != m_commands->end()) {
    (this->*command->second)(msg);
  }
}

void
Client::joinAccepted(MessageData& msg) {
  m_commands = &m_gameCommands;

  m_currentServer.ip = msg.senderIP;
  m_currentServer.port = msg.senderPort;

  m_currentServer.timeOut.init();

  onJoinAccepted();
}

void
Client::joinDenied(MessageData& msg) {
  onJoinDenied();
}

void
Client::lobbiesList(MessageData& msg) {

  UInt32 numLobbies = 0;

  msg.packet >> numLobbies;

  Lobby lobby;

  LobbiesList lobbies;

  for (UInt32 i = 0; i < numLobbies; ++i) {
    msg.packet >> lobby.ip;
    msg.packet >> lobby.port;
    lobbies.push_back(lobby);
  }

  onLobbiesListReceived(std::move(lobbies));
}

void
Client::requestActive(MessageData& msg) {

  Packet pack;
  pack << VER_NUM::kN;
  pack << REQUEST_ID::kNotifyActive;

  m_currentServer.timeOut.init();

  m_socket->send(pack, m_currentServer.ip, m_currentServer.port);
}


void
Client::receiveGameStatus(MessageData& msg) {
  WString data;
  msg.packet >> data;

  onGameStatusReceived(std::move(data));
}

void
Client::checkConnectionStatus() {
  if (m_currentServer.ip) {
    if (m_currentServer.timeOut.getSeconds() >= m_maxTimeOut) {
      closeConnection();
    }
  }
}

void
Client::closeConnection() {
  m_currentServer.ip = 0;
  m_commands = &m_lobbyCommands;
  onConnectionLoss();
}

void
Client::requestConnection(UInt32 ip, UInt16 port) {
  if (!m_currentServer.ip) {
    Packet pack;
    pack << VER_NUM::kN;
    pack << REQUEST_ID::kClientJoin;
    pack << m_userName;

    m_socket->send(pack, ip, port);
  }
}

void
Client::requestDisconnection() {
  if (m_currentServer.ip) {
    Packet pack;
    pack << VER_NUM::kN;
    pack << REQUEST_ID::kClientLeave;
    m_socket->send(pack, m_currentServer.ip, m_currentServer.port);
    m_commands = &m_lobbyCommands;
    m_currentServer.ip = 0;
  }
}

void
Client::requestLobbies() {

  Packet pack;
  pack << VER_NUM::kN;
  pack << REQUEST_ID::kLobbiesRequest;

  m_socket->send(pack, m_globalServer.ip, m_globalServer.port);
}


void
Client::sendFunction(Packet packet) {
  if (m_currentServer.ip) {
    m_socket->send(packet, m_currentServer.ip, m_currentServer.port);
  }
}

}