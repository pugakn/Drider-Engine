#include "dr_game_server_app.h"

#include <iostream>

#include <dr_connection_enums.h>
#include <dr_network_manager.h>
#include <dr_timer.h>


namespace driderSDK {

void 
GameServer::postInit() {

  NetworkManager::startUp();


}

void 
GameServer::postUpdate() {
  checkIncomingPackets();
}

void 
GameServer::postRender() {
}

void 
GameServer::postDestroy() {
  //Time::shutDown();
  NetworkManager::shutDown();
}

void 
GameServer::processPacket(UDPSocket& socket, 
                          Packet& packet, 
                          SizeT recvLen, 
                          UInt32 senderIP, 
                          UInt16 senderPort) {
  UInt32 request = 0;

  packet >> request;

  switch (request) {
  
  }
}

}