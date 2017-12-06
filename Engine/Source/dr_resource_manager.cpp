#include "dr_resource_manager.h"
#include "dr_codec.h"

#include "dr_codec_texture.h"
#include "dr_codec_model.h"
#include "dr_file_system.h"

namespace driderSDK {

void 
ResourceManager::onStartUp() {
  codecs.push_back(std::shared_ptr<CodecTexture>());
  codecs.push_back(std::shared_ptr<CodecModel>());
  
  factory = std::make_shared<ResourceFactory>();
}

void 
ResourceManager::onShutDown() {
}

std::shared_ptr<Resource>
ResourceManager::loadResource(TString resourceName) {
  std::shared_ptr<Resource> r;
  
  for(auto &codec : codecs) {
    TString extension = FileSystem::GetFileExtension(resourceName);
    if(codec->isCompatible(extension)) {
      if(existInResourceContent(resourceName)) {
        r = getReference(resourceName);
      } else {
        createResource(resourceName, codec.get());
        r = getReference(resourceName);
      }
    }
  }
  return r;
}

void
ResourceManager::createResource(TString resourceName,
                                Codec* codec) {
  resourceContent.insert({ resourceName, 
                           factory->CreateResource(codec->getType()) });
}

void 
ResourceManager::addResource(TString resourceName, 
                             std::shared_ptr<Resource> pResource) {
  resourceContent.insert({resourceName, pResource});
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