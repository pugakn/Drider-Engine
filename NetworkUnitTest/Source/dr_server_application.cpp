#include "dr_server_application.h"
#include <dr_logger.h>
#include <dr_network_manager.h>
#include <dr_packet.h>
#include <dr_string_utils.h>

namespace driderSDK {

void 
ServerApplication::postInit() {
  Logger::startUp(_T("../Docs/Logger/serverLog.html"));
  NetworkManager::startUp();

  
  m_socket.create();
  if (!m_socket.isValid()) {
    Logger::addLog(_T("Could not initliaze socket: ") + 
                   NetworkManager::getNetworkErrorStr());
    return;
  }

  Logger::addLog(_T("Socket init!"));

  if (!m_socket.bind("127.0.0.1", 8888)) {
    Logger::addLog(_T("Could not bind socket!") + 
                   NetworkManager::getNetworkErrorStr());
    return;
  }
  
  Logger::addLog(_T("Socket binded!"));

  m_socket.setBlockMode(false);
}

void 
ServerApplication::postUpdate() {

  //Logger::addLog(_T("Waiting for data!"));

  Packet packet;
  UInt16 recPort = 0;
  String recIp;

  Int32 recSize;
  
  auto res = m_socket.receive(packet, 256, recSize, recPort, recIp);

  if (recSize > 0) {
    String message;
    UInt32 tick;
    packet >> tick;
    packet >> message;

    Logger::addLog(_T("Message received: ") + StringUtils::toTString(message) + 
                   _T(" Tick: ") + StringUtils::toTString(tick));
    Logger::addLog(_T("From Ip: ") + StringUtils::toTString(recIp) + 
                   _T(" Port: ") + StringUtils::toTString(recPort));
    
    message = "Hello client!";
    packet << message;
    m_socket.send(packet, recPort, recIp);
  }
  else if (res == SOCKET_ERR::kError) {
    Logger::addLog(_T("Error while receiving data!") +
                   NetworkManager::getNetworkErrorStr());   
  }

}

void 
ServerApplication::postRender() {
}

void 
ServerApplication::postDestroy() {
  m_socket.close();
  NetworkManager::shutDown();
  Logger::shutDown();
}

}