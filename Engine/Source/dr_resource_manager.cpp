#include "dr_resource_manager.h"

#include "dr_codec_texture.h"
#include "dr_codec_model.h"
#include <dr_codec_sound.h>

#include "dr_file_system.h"

#include "dr_texture_resource.h"
#include <dr_model.h>
#include "dr_sound_core.h"

namespace driderSDK {

void
ResourceManager::init(SoundSystem* soundSystem) {
  auto codecTexture  = dr_make_unique<CodecTexture>();
  auto codecModel = dr_make_unique<CodecModel>();
  auto codecSound = dr_make_unique<CodecSound>(soundSystem);

  resourceFactory[codecTexture.get()] = std::make_shared<TextureResource>;
  resourceFactory[codecModel.get()] = std::make_shared<Model>;
  resourceFactory[codecSound.get()] = std::make_shared<SoundCore>;

  codecs.push_back(std::move(codecModel));
  codecs.push_back(std::move(codecTexture));
  codecs.push_back(std::move(codecSound));
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
        break;
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
  
  if(resource != nullptr) {
    resourceContent.insert({ resourceName, 
                             resource });
  } else {
    // Error al cargar recurso
  }
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