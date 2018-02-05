#include "dr_resource_manager.h"
#include "dr_codec.h"

namespace driderSDK {

std::shared_ptr<Resource>
ResourceManager::loadResource(const TString& resourceName) {
  return nullptr;
}

void
ResourceManager::createResource(const TString& resourceKey) {
 resourceContent.insert({ resourceKey , std::shared_ptr<Resource>()});
}

bool
ResourceManager::existInResourceContent(const TString& resourceName) {
  return resourceContent.find(resourceName) != resourceContent.end();
}

std::shared_ptr<Resource>
ResourceManager::getReference(const TString& resourceName) {
  return resourceContent.find(resourceName)->second;
}

}