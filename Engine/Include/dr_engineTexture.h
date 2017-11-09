#pragma once
#include "dr_resource.h"

namespace driderSDK {

class DR_API_EXPORT EngineTexture {
 public:
  /**
  * TEST::defaultConstructor
  * Default contructor;
  */
  EngineTexture() {};

  /**
  * TEST::defaultDestructor
  * Default destructor;
  */
  ~EngineTexture() {};

  /**
  *
  */
  void Load(TString name);

 public:
  unsigned int width;
  unsigned int height;
  unsigned int chanels;

  unsigned char* data; 
};
}