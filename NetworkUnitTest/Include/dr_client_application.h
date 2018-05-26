#pragma once

#include <dr_application.h>
#include <dr_udp_socket.h>
#include "dr_network_prerequisites.h"

namespace driderSDK {

class ClientApplication : public Application
{
public:
  ClientApplication(const String& ip, UInt16 port);
private:
  // Inherited via Application
  virtual void postInit() override;
  virtual void postUpdate() override;
  virtual void postRender() override;
  virtual void postDestroy() override;
  UDPSocket m_socket;
  UInt32 m_tick;
  String m_ip;
  UInt16 m_port;
};

}