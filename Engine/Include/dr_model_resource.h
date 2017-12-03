#pragma once
#include <dr_resource.h>
#include <dr_engine_prerequisites.h>
#include <memory>

namespace driderSDK {

class DR_ENGINE_EXPORT ModelResource : public Resource {
 public:
   ModelResource () {};

   virtual ~ModelResource() {};
   
   /**
   * TEST::
   * Creates and return reference of a ModelResource
   */
   static std::shared_ptr<Resource> __stdcall
   Create(TString resourceName);

 public:
  float info;
};

}