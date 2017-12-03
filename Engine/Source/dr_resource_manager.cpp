#include "dr_resource_manager.h"

#include "dr_codec_texture.h"
#include "dr_codec_model.h"
#include "dr_file_system.h"
#include "dr_resource_factory.h"

namespace driderSDK {

ResourceManager::ResourceManager() {

}

ResourceManager::~ResourceManager() {

}

void 
ResourceManager::Init() {
  codecs.push_back(dr_make_unique<CodecTexture>());
  codecs.push_back(dr_make_unique<CodecModel>());
  
  factory = std::make_shared<ResourceFactory>();
}

std::shared_ptr<Resource>
ResourceManager::loadResource(TString resourceName) {
  std::shared_ptr<Resource> r;
  
  for(auto &codec : codecs) {
    TString extension = FileSystem::getFileExtension(resourceName);
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
  std::shared_ptr<Resource> nResource = factory->CreateResource(codec, resourceName);
  resourceContent.insert({ resourceName, nResource});
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