#pragma once
#include <unordered_map>
#include "dr_engine_prerequisites.h"
#include <dr_memory.h>
#include <dr_module.h>

namespace driderSDK {

class Resource;
class ResourceFactory;
class Codec;

/**
* Load resources.
*
* Sample usage:
*	loadResource("name.png", "resorces/");
*/
class DR_ENGINE_EXPORT ResourceManager  {
 public:

  /**
  * TEST::resourceMConstructor
  *	Default constructor.
  */
  ResourceManager();

  ResourceManager(const ResourceManager&) = delete;

  ResourceManager& operator=(const ResourceManager&) = delete;
  /**
  * TEST::resourceMDestructor
  *	Default destructor.
  */
  ~ResourceManager();

  /**
  * TEST::
  * Initialize the Resource Manager
  */
  void Init();
 
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

  std::vector<std::unique_ptr<Codec>> codecs;

  std::shared_ptr<ResourceFactory> factory;

};

}