#pragma once

#include <vector>

#include "dr_network_prerequisites.h"
#include "dr_socket_handle.h"

namespace driderSDK {

namespace SOCKET_ERR
{
  enum E
  {
    kError = -1,
    kSuccess = 0,
    kUninitialized,
    KWouldBlock
  };
}

namespace SOCKET_TYPE {
enum E 
{
  kTCP,
  kUDP
};
}

class DR_NETWORK_EXPORT Socket {
 public:
  
  using DataBuffer = std::vector<Int8>;
  
  Socket();
  
  virtual ~Socket();
  /**
  * Sets the mode of the socket.
  */
  void
  setBlockMode(bool block);

  /**
   * Retreives the handle for the socket
   * 
   * @return
   *   The handle of the socket.
   */
  SocketHandle
  getHandle();

  /**
  * Tests if the socket is on a valid state.
  *
  * @return 
  *   True if the socket is valid, false otherwise.
  */
  bool
  isValid();

  void
  close();

 protected:
   bool m_blocking;
   SocketHandle m_handle;
};

}
