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
class DR_API_EXPORT ResourceManager {
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
  * @param nameResource
  *   Resource's name.
  *
  * @param pathFile
  *   Resource's path.
  *
  * @return
  *   void.
  */
  void
  loadResource (std::string nameResource,
                std::string pathFile);
  
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
  createResource(ResourceType::E resourceType, 
                 std::string resourceName);
  
  /**
  * TEST::isCompatible
  * Checks if a resource is suported for Drider Engine
  *
  * @param resource
  *   Name of the resource (include path and file extention).
  *
  * @param resourceTypeOut
  *   Output of resource's type.
  *
  * @return
  *   void.
  */
  bool
  isCompatible(std::string resource,
               ResourceType::E &resourceTypeOut);

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
  existInResourceContent (std::string key);

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
  getReference(std::string key);
  
  std::map<std::string, std::shared_ptr<Resource>> resourceContent;


};

}