#pragma once

#include <functional>
#include <unordered_map>

#include <dr_memory.h>
#include <dr_module.h>

#include "dr_engine_prerequisites.h"

namespace driderSDK {

class Resource;
class Codec;
class Material;
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
  
   using SharedResource = std::shared_ptr<Resource>;

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
  static SharedResource
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
  static SharedResource
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
  static SharedResource
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

  /**
  * Renames an existing resource.
  */
  static void
  renameResource(const TString& lastName, 
                 const TString& newName);

  /**
  * Creates a material using default properties or empty.
  * 
  * @param empty
  *  Specifies if the material should be empty.
  * 
  * @return 
  *   The dummy material if empty is false, an empty material otherwise.
  */
  static std::shared_ptr<Material>
  createMaterial(const TString& materialName, bool empty = false);
  
  template<class T>
  static std::shared_ptr<T>
  getReferenceT(const TString& resourceName)
  {
    return std::dynamic_pointer_cast<T>(getReference(resourceName));
  }

 private:
  void
  onStartUp();

  void
  createDefaultResources();

  void
  createDummyTexture();

  void
  createDummyNormalTexture();

  void
  createDummyWhiteTexture();
  
  void
  createDummyBlackTexture();

  void
  createDummyMaterial();

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
  addResource(SharedResource pResource,
              const TString& resourceName);

  
 private:
  using ResourceFactory = std::function<SharedResource()>;

  std::unordered_map<TString, SharedResource> m_resources;
  std::unordered_map<Codec*, ResourceFactory> m_resourceFactories;
  std::vector<std::unique_ptr<Codec>> m_codecs;
};

}