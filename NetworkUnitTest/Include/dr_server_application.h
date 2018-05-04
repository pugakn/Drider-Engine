#pragma once

#include <dr_application.h>
#include <dr_udp_socket.h>
#include "dr_network_prerequisites.h"

namespace driderSDK {

class ServerApplication : public Application
{
public:
private:
  // Inherited via Application
  virtual void postInit() override;
  virtual void postUpdate() override;
  virtual void postRender() override;
  virtual void postDestroy() override;
  UDPSocket m_socket;
};

}
