#include "dr_resource_manager.h"

#include "dr_codec_texture.h"
#include "dr_codec_model.h"
#include "dr_file_system.h"

#include "dr_texture_resource.h"
#include <dr_model.h>
#include <dr_codec_sound.h>

namespace driderSDK {

void
ResourceManager::init() {
  auto codecTexture  = dr_make_unique<CodecTexture>();
  auto codecModel = dr_make_unique<CodecModel>();
  auto codecSound = dr_make_unique<CodecSound>();

  resourceFactory[codecTexture.get()] = std::make_shared<TextureResource>;
  resourceFactory[codecModel.get()] = std::make_shared<Model>;

  codecs.push_back(std::move(codecModel));
  codecs.push_back(std::move(codecTexture));


}

std::shared_ptr<Resource>
ResourceManager::loadResource(TString resourceName) {
  std::shared_ptr<Resource> r;
  
  for (auto &codec : codecs) {
    TString extension = FileSystem::GetFileExtension(resourceName);
    if (codec->isCompatible(extension)) {
      if (existInResourceContent(resourceName)) {
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
  auto resource = resourceFactory[codec]();
  auto info = codec->decode(resourceName);
  resource->init(info.get());
  

  resourceContent.insert({ resourceName, 
                           resource });
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
  if(!existInResourceContent(resourceName))
    return nullptr;
  return resourceContent.find(resourceName)->second;
}

}