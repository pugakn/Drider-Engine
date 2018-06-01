#include "dr_server_application.h"
#include <dr_logger.h>
#include <dr_network_manager.h>
#include <dr_packet.h>
#include <dr_string_utils.h>
#include <iostream>

namespace driderSDK {

ServerApplication::ServerApplication(const String& ip, UInt16 port) 
: m_ip(ip),
  m_port(port) 
{}

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

  Logger::addLog(_T("Socket init!\nEnter server ip: "));

  if (!m_socket.bind(StringUtils::toTString(m_ip), m_port)) {
    Logger::addLog(_T("Could not bind socket: ") + 
                   NetworkManager::getNetworkErrorStr());
    return;
  }

  auto& bindAddr = m_socket.getBindAddress();
  auto bindPort = m_socket.getBindPort();
  
  Logger::addLog(_T("Socket binded!"));

  Logger::addLog(_T("Hosted on: ") + 
                 bindAddr + 
                 _T(" port: ") + 
                 StringUtils::toTString(bindPort));

  m_socket.setBlockMode(false);
}

void 
ServerApplication::postUpdate() {

  //Logger::addLog(_T("Waiting for data!"));

  Packet packet;
  UInt16 recPort = 0;
  TString recIp;

  Int32 recSize;
  
  auto res = m_socket.receive(packet, 256, recSize, recPort, recIp);

  if (recSize > 0) {
    String message;
    //UInt32 tick;
    //packet >> tick;
    packet >> message;
    
    Logger::addLog(_T("Message received: ") + StringUtils::toTString(message));// + 
                   //_T(" Tick: ") + StringUtils::toTString(tick));
    Logger::addLog(_T("From Ip: ") + recIp + 
                   _T(" Port: ") + StringUtils::toTString(recPort));
   
    message = "Hello client: " + message;
    packet.clear();
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