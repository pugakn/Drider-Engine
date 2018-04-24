#pragma once

#include <dr_application.h>

#include "dr_network_prerequisites.h"

namespace driderSDK {

class ClientApplication : public Application
{
public:
private:

  // Inherited via Application
  virtual void postInit() override;
  virtual void postUpdate() override;
  virtual void postRender() override;
  virtual void postDestroy() override;
};

}