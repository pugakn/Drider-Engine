#include "dr_resourceManager.h"

namespace driderSDK {

void
ResourceManager::loadResource(std::string nameResource,
                              std::string pathFile) {
 ResourceType::E resourceType;
 if(isCompatible(pathFile + nameResource, resourceType)) {
  if(!existInResourceContent(nameResource)) {
   createResource(resourceType, nameResource);
  } else {
   getReference(nameResource);
  }
 }
}

void
ResourceManager::createResource(ResourceType::E resourceType, std::string resourceName) {
 resourceContent.insert({ resourceName , std::shared_ptr<Resource>()});
}

bool
ResourceManager::isCompatible(std::string resource,
                              ResourceType::E& resourceTypeOut) {
  return false;
}

bool
ResourceManager::existInResourceContent(std::string key) {
 return resourceContent.find(key) != resourceContent.end();
}

std::shared_ptr<Resource>
ResourceManager::getReference(std::string key) {
 return resourceContent.find(key)->second;
}

}