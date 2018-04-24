#include "dr_network_manager.h"

#include <dr_logger.h>

#ifdef DR_PLATFORM_WINDOWS
#pragma comment(lib,"ws2_32.lib") //Winsock Library
#include <WinSock2.h>
#endif

namespace driderSDK {

void 
NetworkManager::onStartUp() {
  #ifdef DR_PLATFORM_WINDOWS
  WSADATA wsadata;
  auto status = WSAStartup(MAKEWORD(2,2), &wsadata);

  if (status != 0) {
    Logger::addLog(_T("WSAStartup() failed!");
  }
  #endif
}

void 
NetworkManager::onShutDown() {
  #ifdef DR_PLATFORM_WINDOWS
  WSACleanup();
  #endif
}

}