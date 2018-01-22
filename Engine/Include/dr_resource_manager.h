#pragma once
#include <unordered_map>
#include <dr_resource.h>
#include "dr_engine_prerequisites.h"
#include "dr_codec.h"
#include <dr_memory.h>
#include <dr_module.h>
#include "dr_resource_factory.h"

namespace driderSDK {

/**
* Load resources.
*
* Sample usage:
*	loadResource("name.png", "resorces/");
*/
class DR_ENGINE_EXPORT ResourceManager : public Module<ResourceManager> {
 public:

  /**
  * TEST::resourceMConstructor
  *	Default constructor.
  */
  ResourceManager() { }

  /**
  * TEST::resourceMDestructor
  *	Default destructor.
  */
  ~ResourceManager() {}

  virtual void onStartUp();

  virtual void onShutDown();

  /**
  * TEST::loadResource
  * Checks if the resource is compatible, then load this
  * resource in the resourceContent.
  *
  * @param resourceName
  *   Resource's name.
  *
  * @return
  *   void.
  */
  std::shared_ptr<Resource>
  loadResource (TString resourceName);
  
  /**
  * TEST::createResource
  * Creates a resource, then puts in the contentResource and sets a key
  *
  * @param resourceType
  *   Type of the resource.
  *
  * @param resourceName
  *   The name of the resource.
  *
  * @return
  *   void.
  */
  void
  createResource(TString resourceName,
                 Codec* codec);

  void
  addResource(TString resourceName, 
              std::shared_ptr<Resource> pResource);

  /**
  * TEST::existInResourceContent
  * Checks if a resource exist.
  *
  * @param key
  *   The key thats reference a resource in the contentResource
  *
  * @return
  *   Return true if the resource exist, else return false.
  */
  bool
  existInResourceContent (TString key);

  /**
  * TEST::getReference
  * Retraves a reference of a resource with a key.
  *
  * @param key
  *   The key thats reference a resource in the contentResource
  *
  * @return
  *   Return the shared_ptr to a Resource.
  */
  std::shared_ptr<Resource>
  getReference(TString key);
  
 public:
  std::unordered_map<TString, std::shared_ptr<Resource>> resourceContent;

  std::vector<std::shared_ptr<Codec>> codecs;

  std::shared_ptr<ResourceFactory> factory;

};

}