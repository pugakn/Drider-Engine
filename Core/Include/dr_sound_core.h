#pragma once
#include "dr_core_prerequisites.h"
#include "dr_resource.h"

namespace driderSDK {

class DR_CORE_EXPORT SoundCore : public Resource {
 public:
  /**
  * Default constructor
  */
  SoundCore() {}

  /**
  * Default destructor
  */
  virtual ~SoundCore() {}


  virtual void
  init(void* pData) override;
  
  TString name;

};

}