#pragma once

#include "dr_core_prerequisites.h"
#include "dr_gameObject.h"

namespace driderSDK {

class DR_CORE_EXPORT RootNode : public GameObject
{
public:
  RootNode();
private:
  virtual void 
  updateImpl();
};

}
