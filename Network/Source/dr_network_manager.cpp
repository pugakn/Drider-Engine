#include "dr_network_manager.h"
#include <dr_string_utils.h>
//#include <dr_logger.h>

namespace driderSDK {

SocketHandle
NetworkManager::createSocket(SOCKET_TYPE::E type) {

  auto handle = socket(AF_INET, 
                       type == SOCKET_TYPE::kUDP ? SOCK_DGRAM : SOCK_STREAM, 
                       IPPROTO_UDP);
  return handle;
}

bool 
NetworkManager::isSocketValid(SocketHandle handle) {
  return handle != getInvalidHandle();
}

void
NetworkManager::getAddrPort(sockaddr_in address, String& addrStr, UInt16& port) {
  
  addrStr.resize(15);
  
  inet_ntop(AF_INET, 
            &address.sin_addr, 
            const_cast<char*>(addrStr.c_str()), 
            addrStr.size());

  port = ntohs(address.sin_port);
}

}