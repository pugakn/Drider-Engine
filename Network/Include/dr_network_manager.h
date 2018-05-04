#pragma once

#include <dr_module.h>

#include "dr_network_prerequisites.h"
#include "dr_socket_handle.h"

namespace driderSDK {

class DR_NETWORK_EXPORT NetworkManager : public Module<NetworkManager>
{
 public:
  static bool 
  isSocketValid(SocketHandle handle);

  static SocketHandle
  getInvalidHandle();

  static TString
  getLastError();
 private:
  void
  onStartUp();

  void
  onShutDown();
};

}
