#include "dr_resourceManager.h"
#include "dr_codec.h"

namespace driderSDK {

std::shared_ptr<Resource>
ResourceManager::loadResource(TString resourceName) {
  ResourceType::E resourceType = Codec::isCompatible(resourceName);
  if(resourceType != ResourceType::UNSUPPORTED) {
    if(existInResourceContent(resourceName)) {
      return getReference(resourceName);
    } else {
      return Codec::decode(resourceName, resourceType);
    }
  } else {
    // Enviar error al log
    return nullptr; // Cargar archivo corrupto
  }
}

void
ResourceManager::createResource(ResourceType::E resourceType, 
                                TString resourceKey) {
 resourceContent.insert({ resourceKey , std::shared_ptr<Resource>()});
}

bool
ResourceManager::existInResourceContent(TString resourceName) {
  return resourceContent.find(resourceName) != resourceContent.end();
}

std::shared_ptr<Resource>
ResourceManager::getReference(TString resourceName) {
  return resourceContent.find(resourceName)->second;
}

}