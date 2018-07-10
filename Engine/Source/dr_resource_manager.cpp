#include "dr_resource_manager.h"

#include <exception>

#include <dr_file_system.h>
#include <dr_image_info.h>
#include <dr_logger.h>
#include <dr_material.h>
#include <dr_model.h>
#include <dr_sound_core.h>
#include <dr_string_utils.h>
#include <dr_texture_core.h>

#include "dr_codec_model.h"
#include "dr_codec_script.h"
#include "dr_codec_sound.h"
#include "dr_codec_texture.h"
#include "dr_script_core.h"

namespace driderSDK {

ResourceManager::ResourceManager() {}

ResourceManager::~ResourceManager() {}

void
ResourceManager::onStartUp() {
  auto codecTexture  = dr_make_unique<CodecTexture>();
  auto codecModel = dr_make_unique<CodecModel>();
  auto codecSound = dr_make_unique<CodecSound>();
  auto codecScript = dr_make_unique<CodecScript>();
  
  m_resourceFactories[codecTexture.get()] = std::make_shared<TextureCore>;
  m_resourceFactories[codecModel.get()] = std::make_shared<Model>;
  m_resourceFactories[codecSound.get()] = std::make_shared<SoundCore>;
  m_resourceFactories[codecScript.get()] = std::make_shared<ScriptCore>;

  m_codecs.push_back(std::move(codecModel));
  m_codecs.push_back(std::move(codecTexture));
  m_codecs.push_back(std::move(codecSound));
  m_codecs.push_back(std::move(codecScript));

  createDefaultResources();
}

void 
ResourceManager::createDefaultResources() {
  createDummyTexture();
  createDummyNormalTexture();
  createDummyWhiteTexture();
  createDummyBlackTexture();
  createDummyMaterial();
}


ResourceManager::SharedResource
ResourceManager::loadResource(const TString& resourceName) {
  
  return loadResource(resourceName, nullptr);

}

ResourceManager::SharedResource
ResourceManager::loadResource(const TString& resourceName,
                              void* extraData) {
  std::shared_ptr<Resource> r;
  
  auto& rm = instance();

  if (rm.isResourceLoaded(resourceName)) {
    r = getReference(resourceName);
  } 
  else {
    bool codecRead = false;

    TString extension = FileSystem::GetFileExtension(resourceName);

    for (auto &codec : rm.m_codecs) {
      if (codec->isCompatible(extension)) {
        rm.createResource(resourceName, codec.get(), extraData);
        r = getReference(resourceName);

        if (r) {
          r->setName(resourceName);
        }

        codecRead = true;

        break;
      }
    }

    if (!codecRead) {
      Logger::addLog(_T("No codec avalible for resource: ") + resourceName); 
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
    Logger::addLog(_T("Error loading resource: ") + resourceName);
  }
}

void 
ResourceManager::addResource(SharedResource pResource,
                             const TString& resourceName) {

  Logger::addLog(_T("Added resource: ") + resourceName);

  instance().m_resources[resourceName] =  pResource;
}

bool
ResourceManager::isResourceLoaded(const TString& resourceName) {
  return instance().m_resources.count(resourceName);
}

void 
ResourceManager::renameResource(const TString& lastName, 
                                const TString& newName) {
  auto& resources = instance().m_resources;
  auto it = resources.find(lastName);
  if (it != resources.end()) {
    auto res = it->second;
    resources.erase(it);

    DR_DEBUG_ONLY(
    if (resources.count(newName)) {
      Logger::addLog(_T("About to replace existing resource named: ") + newName);
    })

    resources[newName] = res;

    res->setName(newName);
  }
}

ResourceManager::SharedResource
ResourceManager::getReference(const TString& resourceName) {
  std::shared_ptr<Resource> res;

  if (instance().isResourceLoaded(resourceName)) {
    res = instance().m_resources[resourceName];
  } else {
    //throw std::exception(("Resource not found: " + StringUtils::toString(resourceName)).c_str() );

  }

  return res;
}

void
ResourceManager::createDummyTexture() {
  
  auto texture = std::make_shared<TextureCore>();

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

  constexpr Int32 areaSize = 8;
  constexpr Int32 nBoxes = 256 / areaSize;

  UInt8 value = 0;

  for (Int32 i = 0; i < nBoxes; ++i) {
    for (Int32 j = 0; j < nBoxes; ++j) {
      for (Int32 y = 0; y < areaSize; ++y) {
        for (Int32 x = 0; x < areaSize; ++x) {
          Pixel& pixel = matrix[i * areaSize + y][j * areaSize + x];
          pixel.d[0] = value;
          pixel.d[1] = 0;
          pixel.d[2] = value;
          pixel.d[3] = 255;
        }
      }
      value = value ^ 255;
    }
    value = value ^ 255;
  }
 
  std::memcpy(imageInfo.data.data(), matrix[0][0].d, 256 * 256 * 4);

  texture->init(&imageInfo);

  addResource(texture, _T("DUMMY_TEXTURE"));
}

void
ResourceManager::createDummyNormalTexture() {
  auto texture = std::make_shared<TextureCore>();

  constexpr Int32 size = 1;
  constexpr Int32 channels = 3;

  ImageInfo imageInfo;

  imageInfo.width = size;
  imageInfo.height = size;
  imageInfo.channels = channels;
  imageInfo.data.resize(size * size * channels);

  struct Pixel
  {
    UInt8 d[channels];
  };

  Pixel BluePixel = { 127, 127, 255 };
 
  std::memcpy(imageInfo.data.data(), &BluePixel.d[0], sizeof(BluePixel));

  texture->init(&imageInfo);

  addResource(texture, _T("DUMMY_NORMAL_TEXTURE"));
}

void
ResourceManager::createDummyWhiteTexture() {
  auto texture = std::make_shared<TextureCore>();

  constexpr Int32 size = 1;
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

  Pixel WhitePixel = { 255, 255, 255, 255 };

  std::memcpy(imageInfo.data.data(), &WhitePixel.d[0], sizeof(WhitePixel));

  texture->init(&imageInfo);

  addResource(texture, _T("DUMMY_WHITE_TEXTURE"));
}

void
ResourceManager::createDummyBlackTexture() {
  auto texture = std::make_shared<TextureCore>();

  constexpr Int32 size = 1;
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

  Pixel BlackPixel = { 0, 0, 0, 255 };

  std::memcpy(imageInfo.data.data(), &BlackPixel.d[0], sizeof(BlackPixel));

  texture->init(&imageInfo);

  addResource(texture, _T("DUMMY_BLACK_TEXTURE"));
}

std::shared_ptr<Material>
ResourceManager::createMaterial(const TString& materialName, bool empty) {
  
  std::shared_ptr<Material> material;

  if (!isResourceLoaded(materialName)) { //This or override another resource
    material = std::make_shared<Material>(materialName);

    if (!empty) {
      auto dummyMaterial = getReferenceT<Material>(_T("DUMMY_MATERIAL"));  
      *material = *dummyMaterial;
    }
  
    addResource(material, materialName);
  }

  return material;
}

void 
ResourceManager::createDummyMaterial() {

  auto res = getReference(_T("DUMMY_TEXTURE"));
  auto resNorm = getReference(_T("DUMMY_NORMAL_TEXTURE"));
  auto resWhite = getReference(_T("DUMMY_WHITE_TEXTURE"));
  auto resBlack = getReference(_T("DUMMY_BLACK_TEXTURE"));

  auto dummyTexture = std::dynamic_pointer_cast<TextureCore>(res);
  auto dummyNormalTexture = std::dynamic_pointer_cast<TextureCore>(resNorm);
  auto dummyWhiteTexture = std::dynamic_pointer_cast<TextureCore>(resWhite);
  auto dummyBlackTexture = std::dynamic_pointer_cast<TextureCore>(resBlack);

  DR_ASSERT(dummyTexture);
  DR_ASSERT(dummyNormalTexture);
  DR_ASSERT(dummyWhiteTexture);
  DR_ASSERT(dummyBlackTexture);

  auto dummyMat = std::make_shared<Material>(_T("DUMMY_MATERIAL"));

  dummyMat->addProperty<Vec3Property>(_T("Albedo"), 
                                      {0,0,0}, 
                                      {CHANNEL::kR, 
                                       CHANNEL::kG, 
                                       CHANNEL::kB})->texture = dummyTexture;

  dummyMat->addProperty<FloatProperty>(_T("Metallic"), 
                                       1.f, 
                                       CHANNEL::kA)->texture = dummyBlackTexture;

  dummyMat->addProperty<Vec3Property>(_T("Normal"), 
                                      {1.f, 1.f, 1.f}, 
                                      {CHANNEL::kR, 
                                       CHANNEL::kG, 
                                       CHANNEL::kB })->texture = dummyNormalTexture;

  dummyMat->addProperty<FloatProperty>(_T("Roughness"), 
                                        1.f, 
                                        CHANNEL::kA)->texture = dummyWhiteTexture;

  dummyMat->addProperty<Vec3Property>(_T("Emisivity"), 
                                      {1.f, 1.f, 1.f}, 
                                      {CHANNEL::kR, 
                                       CHANNEL::kG, 
                                       CHANNEL::kB})->texture = dummyBlackTexture;

  dummyMat->addProperty<FloatProperty>(_T("Transparency"), 
                                       1.f, 
                                       CHANNEL::kA)->texture = dummyBlackTexture;

  addResource(dummyMat, _T("DUMMY_MATERIAL"));
}

std::vector<std::shared_ptr<Resource>>
ResourceManager::getAllResources() {
  std::vector<std::shared_ptr<Resource>> resources;

  for(auto it : ResourceManager::instance().m_resources) {
    resources.emplace_back(it.second);
  }

  return resources;
}

}