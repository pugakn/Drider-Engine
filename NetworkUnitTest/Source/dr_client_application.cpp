#include "dr_client_application.h"
#include <dr_input_manager.h>
#include <dr_logger.h>
#include <dr_network_manager.h>
#include <dr_packet.h>

namespace driderSDK {

void 
ClientApplication::postInit() {
  Logger::startUp();
  NetworkManager::startUp();
  InputManager::startUp();

  m_socket.init();
  
  if (!m_socket.isValid()) {
    Logger::addLog(_T("Could not initliaze socket!"));
    return;
  }

  String message = "Hello server 01!";
  Packet packet;
  packet << message;
  UInt16 serverPort = 8888;
  String serverIP = "127.0.0.1";
  
  if (m_socket.send(packet, serverPort, serverIP)) {
    Logger::addLog(_T("Message send: ") + StringUtils::toTString(message));
  }
  else {
    Logger::addLog(_T("Error while sending the message!"));
  }
}

void 
ClientApplication::postUpdate() {}

void 
ClientApplication::postRender() {}

void 
ClientApplication::postDestroy() {

  InputManager::startUp();
  NetworkManager::shutDown();
  Logger::shutDown();
}

}