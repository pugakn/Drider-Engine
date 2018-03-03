#pragma once

#include <functional>
#include <unordered_map>

#include <dr_memory.h>
#include <dr_module.h>

#include "dr_engine_prerequisites.h"

namespace driderSDK {

class Resource;
class Codec;

/**
* Load resources.
*
* Sample usage:
*	loadResource("name.png", "resorces/");
*/
class DR_ENGINE_EXPORT ResourceManager : public Module<ResourceManager> 
{

  friend Codec;

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
  * TEST::getReference
  * Retraves a reference of a resource with a key.
  *
  * @param key
  *   The key thats reference a resource in the contentResource
  *
  * @return
  *   Return the shared_ptr to a Resource.
  */
  static std::shared_ptr<Resource>
  getReference(const TString& key);

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
  static std::shared_ptr<Resource>
  loadResource(const TString& resourceName);

  /**
  * TEST::loadResource
  * Checks if the resource is compatible, then load this
  * resource in the resourceContent.
  *
  * @param resourceName
  *   Resource's name.
  *
  * @param extraInfo
  *   Extra data to initialize the resource
  *
  * @return
  *   void.
  */
  static std::shared_ptr<Resource>
  loadResource(const TString& resourceName,
               void* extraData);

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
  static bool
  isResourceLoaded(const TString& key);
  
private:
  void
  onStartUp();

  void
  createDefaultResources();
  /**
  * TEST::createResource
  * Creates a resource, then puts in the contentResource and sets a key
  *
  * @param resourceName
  *   Type of the resource.
  *
  * @param codec
  *   Pointer to codec.
  *
  * @param extraInfo
  *   Extra info to inicialize the resource.
  *
  * @return
  *   void.
  */
  void
  createResource(const TString& resourceName,
                 Codec* codec,
                 void* extraInfo);
  /**
  * Add a resource to the ResourceContent of the ResourceManager
  */
  static void
  addResource(std::shared_ptr<Resource> pResource,
              const TString& resourceName);

  
 private:
  using SharedResource = std::shared_ptr<Resource>;
  using ResourceFactory = std::function<SharedResource()>;

  std::unordered_map<TString, SharedResource> m_resources;
  std::unordered_map<Codec*, ResourceFactory> m_resourceFactories;
  std::vector<std::unique_ptr<Codec>> m_codecs;
};

}