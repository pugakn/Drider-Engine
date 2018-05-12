#pragma once

#include <dr_module.h>

#include "dr_network_prerequisites.h"
#include "dr_socket.h"

namespace driderSDK {

class DR_NETWORK_EXPORT NetworkManager : public Module<NetworkManager>
{
 public:
  static bool 
  isSocketValid(SocketHandle handle);

  static SocketHandle
  getInvalidHandle();

  static SocketHandle
  createSocket(SOCKET_TYPE::E type);

  static sockaddr_in
  getAddress(UInt32 address, UInt16 port);
  
  static sockaddr_in
  getAddress(const String& address, UInt16 port);

  static void
  getAddrPort(sockaddr_in address, String& outAddress, UInt16& outPort);
  
  static TString
  getNetworkErrorStr();

  static Int32
  getNetworkError();

  static SOCKET_ERR::E
  getSocketError();

  static void
  setSocketBlockMode(SocketHandle socket, bool block);

  static void
  closeSocket(SocketHandle socket);
 private:
  void
  onStartUp();

  void
  onShutDown();
};

}
