#include "dr_lobby_server_app.h"

#include <algorithm>
#include <iostream>

#include <dr_connection_enums.h>
#include <dr_network_manager.h>
#include <dr_packet.h>
#include <dr_string_utils.h>

namespace driderSDK {

LobbyServerApp::LobbyServerApp() 
  : Application(false)
{}

LobbyServerApp::~LobbyServerApp() 
{}

void 
LobbyServerApp::postInit() {

  NetworkManager::startUp();

  }

void 
LobbyServerApp::postUpdate() {
  checkIncomingPackets();
}

void 
LobbyServerApp::postRender() {
}

void 
LobbyServerApp::postDestroy() {
  NetworkManager::shutDown();
}

void 
LobbyServerApp::processPacket(UDPSocket& socket, 
                              Packet& packet, 
                              SizeT recvLen, 
                              UInt32 senderIP, 
                              UInt16 senderPort) {
  UInt32 request = 0;

  m_packet >> request;

  switch (request) {
  case REQUEST_ID::kServerCreated:
  {
  }
  break;
  case REQUEST_ID::kActive:
  {
  }
  break;
  case REQUEST_ID::kLobbiesList:
  {
  }
  break;
  default:
  break;
  }

}

}