#pragma once
#include "dr_core_prerequisites.h"
#include <dr_module.h>

namespace driderSDK {

class DR_CORE_EXPORT Messenger : public Module<Messenger> {

public:
  Messenger();

  ~Messenger();

  virtual void 
  onStartUp() override;

  

};

}