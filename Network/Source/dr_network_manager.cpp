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
NetworkManager::getAddrPort(sockaddr_in address, 
                            TString& addrStr, 
                            UInt16& port) {
  
  String addr(15, '\0');

  inet_ntop(AF_INET, 
            &address.sin_addr, 
            const_cast<char*>(addr.c_str()), 
            addr.size());

  addrStr = StringUtils::toTString(addr);

  port = ntohs(address.sin_port);
}

void
NetworkManager::getAddrPort(sockaddr_in address, 
                            UInt32& addrStr, 
                            UInt16& port) {
  
  addrStr = address.sin_addr.s_addr;

  port = ntohs(address.sin_port);
}

}