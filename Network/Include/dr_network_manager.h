#pragma once

#include <dr_module.h>
#include "dr_network_prerequisites.h"

namespace driderSDK {

class NetworkManager : public Module<NetworkManager>
{
 
 public:


 private:
  void
  onStartUp();

  void
  onShutDown();
};

}
