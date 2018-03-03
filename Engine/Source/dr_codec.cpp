#include "dr_codec.h"
#include "dr_resource_manager.h"

namespace driderSDK {

void 
Codec::addResource(std::shared_ptr<Resource> resource, 
                   const TString& resourceName) {
  if (!ResourceManager::isResourceLoaded(resourceName)) {
    ResourceManager::addResource(resource, resourceName);  
  }  
}

}