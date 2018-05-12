#include "dr_client_application.h"

#include <functional>

#include <dr_input_manager.h>
#include <dr_keyboard.h>
#include <dr_logger.h>
#include <dr_network_manager.h>
#include <dr_packet.h>
#include <dr_string_utils.h>

namespace driderSDK {

void 
ClientApplication::postInit() {
  Logger::startUp(_T("../Docs/Logger/clientLog.html"));
  NetworkManager::startUp();
  InputManager::startUp(reinterpret_cast<SizeT>(m_hwnd));

  m_tick = 0;

  m_socket.create();
  
  if (!m_socket.isValid()) {
    Logger::addLog(_T("Could not initliaze socket!"));
    return;
  }

  auto msgCallback = [this](const String& msg)
  {
    Packet packet;
    packet << m_tick;
    packet << msg;
    UInt16 serverPort = 8888;
    String serverIP = "127.0.0.1";
    ++m_tick;

    if (m_socket.send(packet, serverPort, serverIP) == 
        SOCKET_ERR::kSuccess) {
      Logger::addLog(_T("Message send: ") + StringUtils::toTString(msg));
    }
    else {
      Logger::addLog(_T("Error while sending the message!"));
    }
  };

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed, 
                        KEY_CODE::kA, 
                        std::bind(msgCallback, "A Pressed!"));

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed, 
                        KEY_CODE::kB, 
                        std::bind(msgCallback, "B Pressed!"));

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed, 
                        KEY_CODE::kC, 
                        std::bind(msgCallback, "C Pressed!"));

  Keyboard::addCallback(KEYBOARD_EVENT::kKeyPressed, 
                        KEY_CODE::kD, 
                        std::bind(msgCallback, "D Pressed!"));

  //msgCallback("Hello server!");

  /*Packet packet;
  UInt16 serverPort;
  String serverIP;*/
  
  msgCallback("Hello server!!");

  /*Int32 recSize = m_socket.receive(packet, 256, serverPort, serverIP);

  if (recSize != -1) {
    String msg;
    packet >> msg;
    Logger::addLog(_T("Received from server: ") + StringUtils::toTString(msg));
    Logger::addLog(_T("From Ip: ") + StringUtils::toTString(serverIP) + 
                   _T(" Port: ") + StringUtils::toTString(serverPort));
  }
  else {
    Logger::addLog(_T("Error while receiving msg: ") + 
                   NetworkManager::getLastError());
  }*/
}

void 
ClientApplication::postUpdate() {
  
  InputManager::update();
}

void 
ClientApplication::postRender() {}

void 
ClientApplication::postDestroy() {
  m_socket.close();
  InputManager::shutDown();
  NetworkManager::shutDown();
  Logger::shutDown();
}

}