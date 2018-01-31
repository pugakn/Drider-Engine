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
  

  /**
  * TEST::create
  * Creates, initialize and return reference of a TextureResource
  */
  static std::shared_ptr<Resource> __stdcall
  Create(TString resourceName);

  
public:
  std::vector<unsigned char> data;
  Int32 width;
  Int32 height;
  Int32 channels;

};

}