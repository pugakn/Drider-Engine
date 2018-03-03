#include "dr_resource_manager.h"

#include <iostream>

#include <dr_file_system.h>
#include <dr_model.h>
#include <dr_sound_core.h>
#include <dr_string_utils.h>
#include <dr_texture_core.h>
#include <dr_image_info.h>

#include "dr_codec_model.h"
#include "dr_codec_sound.h"
#include "dr_codec_texture.h"



namespace driderSDK {


ResourceManager::ResourceManager() {}

ResourceManager::~ResourceManager() {}

void
ResourceManager::onStartUp() {
  auto codecTexture  = dr_make_unique<CodecTexture>();
  auto codecModel = dr_make_unique<CodecModel>();
  auto codecSound = dr_make_unique<CodecSound>();

  m_resourceFactories[codecTexture.get()] = std::make_shared<TextureCore>;
  m_resourceFactories[codecModel.get()] = std::make_shared<Model>;
  m_resourceFactories[codecSound.get()] = std::make_shared<SoundCore>;

  m_codecs.push_back(std::move(codecModel));
  m_codecs.push_back(std::move(codecTexture));
  m_codecs.push_back(std::move(codecSound));

  createDefaultResources();
}

void 
ResourceManager::createDefaultResources() {

  auto defaultTexture = std::make_shared<TextureCore>();

  constexpr Int32 size = 256;
  constexpr Int32 channels = 4;

  ImageInfo imageInfo;

  imageInfo.width = size;
  imageInfo.height = size;
  imageInfo.channels = channels;
  imageInfo.data.resize(size * size * channels);

  struct Pixel 
  {
    UInt8 d[channels];
  };

  Pixel matrix[size][size];

  constexpr Int32 areaSize = 32;
  constexpr Int32 nBoxes = 256 / areaSize;

  UInt8 value = 0;

  for (Int32 i = 0; i < nBoxes; ++i) {
    for (Int32 j = 0; j < nBoxes; ++j) {
      for (Int32 y = 0; y < areaSize; ++y) {
        for (Int32 x = 0; x < areaSize; ++x) {
          Pixel& pixel = matrix[i * areaSize + y][j * areaSize + x];
          pixel.d[0] = value;
          pixel.d[1] = value;
          pixel.d[2] = value;
          pixel.d[3] = 255;
        }
      }
      value = value ^ 255;
    }
    value = value ^ 255;
  }
 
  std::memcpy(imageInfo.data.data(), matrix[0][0].d, 256 * 256 * 4);

  defaultTexture->init(&imageInfo);

  addResource(defaultTexture, _T("DUMMY_TEXTURE"));

  auto defaultMaterial = std::make_shared<Material>();

  defaultMaterial->addProperty<Vec3Property>(_T("Albedo"), 
                                             {0,0,0}, 
                                             {CHANNEL::kR, 
                                              CHANNEL::kG, 
                                              CHANNEL::kB})->texture = 
                                                             defaultTexture;

  defaultMaterial->addProperty<FloatProperty>(_T("Metallic"), 
                                              1.f, 
                                              CHANNEL::kA)->texture = 
                                                            defaultTexture;

  defaultMaterial->addProperty<Vec3Property>(_T("Normal"), 
                                              {1.f, 1.f, 1.f}, 
                                              { CHANNEL::kR, 
                                                CHANNEL::kG, 
                                                CHANNEL::kB })->texture = 
                                                                defaultTexture;

  defaultMaterial->addProperty<FloatProperty>(_T("Roughness"), 
                                              1.f, 
                                              CHANNEL::kA)->texture = 
                                                            defaultTexture;

  defaultMaterial->addProperty<Vec3Property>(_T("Emisivity"), 
                                              {1.f, 1.f, 1.f}, 
                                              { CHANNEL::kR, 
                                                CHANNEL::kG, 
                                                CHANNEL::kB })->texture = 
                                                                defaultTexture;

  defaultMaterial->addProperty<FloatProperty>(_T("Transparency"), 
                                              1.f, 
                                              CHANNEL::kA)->texture = 
                                                            defaultTexture;

  addResource(defaultMaterial, _T("DEFAULT_MATERIAL"));
}

std::shared_ptr<Resource>
ResourceManager::loadResource(const TString& resourceName) {
  
  return loadResource(resourceName, nullptr);

}

std::shared_ptr<Resource>
ResourceManager::loadResource(const TString& resourceName,
                              void* extraData) {
  std::shared_ptr<Resource> r;
  
  auto& rm = instance();

  if (rm.isResourceLoaded(resourceName)) {
    r = getReference(resourceName);
  } 
  else {
    for (auto &codec : rm.m_codecs) {
      TString extension = FileSystem::GetFileExtension(resourceName);
      if (codec->isCompatible(extension)) {
        rm.createResource(resourceName, codec.get(), extraData);
        r = getReference(resourceName);
      }
    }    
  }

  return r;
}

void
ResourceManager::createResource(const TString& resourceName,
                                Codec* codec,
                                void* extraInfo) {
  auto resource = instance().m_resourceFactories[codec]();

  auto info = codec->decode(resourceName);
  
  if (info) {

    if (extraInfo) {
      resource->init(info.get(), extraInfo);
    }

    else {
      resource->init(info.get());
    }

    addResource(resource, resourceName);
  }
  else {
    //ERROR
    std::cout << "Error loading resource: ";
    std::cout << StringUtils::toString(resourceName) << std::endl;
  }
}

void 
ResourceManager::addResource(std::shared_ptr<Resource> pResource,
                             const TString& resourceName) {

  std::cout << "Added resource: ";
  std::cout << StringUtils::toString(resourceName) << std::endl;

  instance().m_resources[resourceName] =  pResource;
}

bool
ResourceManager::isResourceLoaded(const TString& resourceName) {
  return instance().m_resources.count(resourceName);
}

std::shared_ptr<Resource>
ResourceManager::getReference(const TString& resourceName) {
  std::shared_ptr<Resource> res;

  if (instance().isResourceLoaded(resourceName)) {
    res = instance().m_resources[resourceName];
  }

  return res;
}

}