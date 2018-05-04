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

  
  m_socket.init();
  if (!m_socket.isValid()) {
    Logger::addLog(_T("Could not initliaze socket!"));
    NetworkManager::getLastError();
    return;
  }

  Logger::addLog(_T("Socket init!"));

  m_socket.bind(8888, "127.0.0.1");

  if (!m_socket.isValid()) {
    Logger::addLog(_T("Could not bind socket!"));
    NetworkManager::getLastError();
    return;
  }

  Logger::addLog(_T("Socket binded!"));
  
}

void 
ServerApplication::postUpdate() {

  Logger::addLog(_T("Waiting for data!"));

  Packet packet;
  UInt16 recPort = 0;
  String recIp;

  Int32 recSize = m_socket.receive(packet, 256, recPort, recIp);

  if (recSize != -1) {
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
  else {
    Logger::addLog(_T("Error while receiving data!"));
    Logger::addLog(NetworkManager::getLastError());
  }

}

void 
ServerApplication::postRender() {
}

void 
ServerApplication::postDestroy() {
  Logger::shutDown();
  NetworkManager::shutDown();
}

}