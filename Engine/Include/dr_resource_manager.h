#pragma once
#include <memory>
#include <map>
#include <dr_resource.h>
#include "dr_engine_prerequisites.h"
#include "dr_codec.h"

namespace driderSDK {

/**
* Load resources.
*
* Sample usage:
*	loadResource("name.png", "resorces/");
*/
class DR_ENGINE_EXPORT ResourceManager {
 public:
  /**
  * TEST::resourceMConstructor
  *	Default constructor.
  */
  ResourceManager() {}

  /**
  * TEST::resourceMDestructor
  *	Default destructor.
  */
  ~ResourceManager() {}
 
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
  createResource(const TString& resourceKey);

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
  
  std::map<TString, std::shared_ptr<Resource>> resourceContent;


};

}