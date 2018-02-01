#pragma once
#include "dr_core_prerequisites.h"
#include "dr_resource.h"

namespace driderSDK {

class DrSound;

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

  /**
  * Gets the DrSound object 
  */
  DrSound *
  get();
  
  TString name;
  DrSound *soundResource;

};

}