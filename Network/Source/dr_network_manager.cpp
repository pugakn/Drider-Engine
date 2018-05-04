#include "dr_network_manager.h"
#include <dr_string_utils.h>
//#include <dr_logger.h>

#ifdef DR_PLATFORM_WINDOWS
#pragma comment(lib,"ws2_32.lib") //Winsock Library
#include <WinSock2.h>
#endif

namespace driderSDK {

bool 
NetworkManager::isSocketValid(SocketHandle handle) {
  return handle != getInvalidHandle();
}

SocketHandle NetworkManager::getInvalidHandle() {
  #ifdef DR_PLATFORM_WINDOWS
  return INVALID_SOCKET;
  #endif
}

TString 
NetworkManager::getLastError() {
  
  wchar_t errString[255];

  Int32 errCode = WSAGetLastError();

  FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, 
                NULL, 
                errCode,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
                errString,
                255, 
                NULL);

  return errString;
}

void
NetworkManager::onStartUp() {
  #ifdef DR_PLATFORM_WINDOWS
  WSADATA wsadata;
  auto status = WSAStartup(MAKEWORD(2,2), &wsadata);

  if (status != 0) {
    //Logger::addLog(_T("WSAStartup() failed!"));
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