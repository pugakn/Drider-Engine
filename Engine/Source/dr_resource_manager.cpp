#include "dr_resource_manager.h"
#include "dr_codec.h"

#include "dr_codec_texture.h"
#include "dr_codec_model.h"
#include "dr_file_system.h"

namespace driderSDK {

void 
ResourceManager::Init() {
  /*codecs.push_back(std::unique_ptr<CodecTexture>());
  codecs.push_back(std::unique_ptr<CodecModel>());*/
  
  factory = std::make_shared<ResourceFactory>();
}

std::shared_ptr<Resource>
ResourceManager::loadResource(TString resourceName) {
  for(auto &codec : codecs) {
    TString extension = FileSystem::getFileExtension(resourceName);
    if(codec->isCompatible(extension)) {
      
    }
  }
  return nullptr;
}

void
ResourceManager::createResource(TString resourceName,
                                Codec* codec) {
  resourceContent.insert({ resourceName, 
                           factory->CreateResource(codec->getType()) });
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