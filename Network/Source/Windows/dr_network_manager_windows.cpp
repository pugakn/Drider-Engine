#include "dr_network_manager.h"

#if (DR_PLATFORM == DR_PLATFORM_WINDOWS)

#include <dr_string_utils.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

namespace driderSDK {

SocketHandle 
NetworkManager::getInvalidHandle() {
  return INVALID_SOCKET;
}

sockaddr_in 
NetworkManager::getAddress(UInt32 address, UInt16 port) {

  sockaddr_in addr = {};

  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = address;//htonl(address);

  return addr;
}

sockaddr_in
NetworkManager::getAddress(const TString& address, UInt16 port) {
  
  sockaddr_in addr = {};

  if (address.empty()) {
    hostent* hp{};
    char hostName[256];
    gethostname(hostName,
                sizeof(hostName));

    hp = gethostbyname(hostName);

    if (hp) {
      addr.sin_addr.S_un.S_un_b.s_b1 = hp->h_addr_list[0][0];
      addr.sin_addr.S_un.S_un_b.s_b2 = hp->h_addr_list[0][1];
      addr.sin_addr.S_un.S_un_b.s_b3 = hp->h_addr_list[0][2];
      addr.sin_addr.S_un.S_un_b.s_b4 = hp->h_addr_list[0][3];
    }
  }
  else {
    auto str = StringUtils::toString(address);
    inet_pton(AF_INET, str.c_str(), &addr.sin_addr);
  }

  addr.sin_family = AF_INET;
  
  addr.sin_port = htons(port);

  return addr;
}

UInt32 
NetworkManager::ipAddrStrToUInt(const TString& address) {
  UInt32 result = 0;
  auto str = StringUtils::toString(address);
  inet_pton(AF_INET, str.c_str(), &result);
  return result;
}

TString
NetworkManager::ipAddrUIntToStr(UInt32 address) {

  TString res;

  String rr(15, '\0');

  inet_ntop(AF_INET, &address, &rr[0], rr.size());

  res = StringUtils::toTString(rr);

  return res;  
}

void
NetworkManager::setSocketBlockMode(SocketHandle socket, bool block) {
  u_long blocking = block ? 0 : 1;
  auto err = ioctlsocket(socket, FIONBIO, &blocking);
  if (err != 0) {
    //Error

  }
}

void
NetworkManager::closeSocket(SocketHandle socket) {
  closesocket(socket);
}

TString 
NetworkManager::getNetworkErrorStr() {
  
  TString errString(128, '\0');

  Int32 errCode = getNetworkError();

  FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 
                NULL, 
                errCode,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
                const_cast<TString::value_type*>(errString.data()),
                errString.size(), 
                NULL);

  return errString;
}

Int32
NetworkManager::getNetworkError() {
  return WSAGetLastError();
}

SOCKET_ERR::E
NetworkManager::getSocketError() {

  auto err = getNetworkError();

  switch (err) {
  case 0:
  return SOCKET_ERR::kSuccess;
  case WSAEALREADY:
  case WSAEWOULDBLOCK:
  return SOCKET_ERR::KWouldBlock;
  default:
  return SOCKET_ERR::kError;
  }

}

void
NetworkManager::onStartUp() {
  WSADATA wsadata;
  auto status = WSAStartup(MAKEWORD(2,2), &wsadata);

  if (status != 0) {
    //Logger::addLog(_T("WSAStartup() failed!"));
  }  
}

void 
NetworkManager::onShutDown() {
  WSACleanup();
}

}

#endif