#pragma once
#include <dr_resource.h>
#include <dr_engine_prerequisites.h>

#include <memory>
#include <vector>

namespace driderSDK {

class DR_ENGINE_EXPORT TextureResource : public Resource {
 public:
  /**
  * TEST::constructor
  * Default constructor
  */
  TextureResource() {};

  /**
  * TEST::destructor
  * Default destructor
  */
  virtual ~TextureResource() {};

  void 
  init(void * resourceData) override;
  
public:
  std::vector<unsigned char> data;
  Int32 width;
  Int32 height;
  Int32 channels;

};

}