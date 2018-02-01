#pragma once
#include <unordered_map>
#include <functional>
#include "dr_engine_prerequisites.h"
#include <dr_memory.h>
#include <dr_module.h>
#include <dr_codec.h>

namespace driderSDK {

class Resource;
class SoundSystem;

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

  ResourceManager(const ResourceManager&) = delete;

  ResourceManager& operator=(const ResourceManager&) = delete;

  /**
  * TEST::resourceMDestructor
  *	Default destructor.
  */
  ~ResourceManager() {}

  /**
  * Initialize the resource manager 
  */
  void
  init(SoundSystem* soundSystem);

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
  
private:
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
public:
  /**
  *
  */
  void
  addResource(TString resourceName, 
              std::shared_ptr<Resource> pResource);
private:
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
  
 public:
  using ResorceSmartPtr = std::shared_ptr<Resource>;
  std::unordered_map<TString, std::shared_ptr<Resource>> resourceContent;
  std::unordered_map<Codec*, std::function<ResorceSmartPtr()>> resourceFactory;
  std::vector<std::unique_ptr<Codec>> codecs;

};

}