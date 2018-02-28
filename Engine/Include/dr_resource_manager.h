#pragma once
#include <unordered_map>
#include <functional>
#include "dr_engine_prerequisites.h"
#include <dr_memory.h>
#include <dr_module.h>
#include <dr_codec.h>

namespace driderSDK {

class Resource;


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

  void
  onStartUp();

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
  std::shared_ptr<Resource>
  loadResource (const TString& resourceName);

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
  std::shared_ptr<Resource>
  loadResource(const TString& resourceName,
               void* extraData);
  
private:
  /**
  * TEST::createResource
  * Creates a resource, then puts in the contentResource and sets a key
  *
  * @param resourceName
  *   Resource's name.
  *
  * @param codec
  *   Pointer to codec.
  *
  * @return
  *   void.
  */
  void
  createResource(const TString& resourceName,
                 Codec* codec);

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
  
public:
  /**
  * Add a resource to the ResourceContent of the ResourceManager
  */
  void
  addResource(const TString& resourceName, 
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
  existInResourceContent (const TString& key);
  
 public:
  using ResorceSmartPtr = std::shared_ptr<Resource>;
  std::unordered_map<TString, std::shared_ptr<Resource>> resourceContent;
  std::unordered_map<Codec*, std::function<ResorceSmartPtr()>> resourceFactory;
  std::vector<std::unique_ptr<Codec>> codecs;
  //ResourceFactory* factory;

};

}