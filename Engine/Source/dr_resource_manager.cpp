#include "dr_resource_manager.h"
#include "dr_codec.h"

namespace driderSDK {

std::shared_ptr<Resource>
ResourceManager::loadResource(TString resourceName) {
  return nullptr;
}

void
ResourceManager::createResource(TString resourceKey) {
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