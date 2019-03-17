#include "dr_resource_manager.h"

#include <algorithm>
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
#include "dr_codec_ctexture.h"
#include "dr_codec_scene.h"
#include "dr_codec_material.h"
#include "dr_script_core.h"
#include "dr_scene_core.h"
#include "dr_script_component.h"

#include <dr_graph.h>
#include <dr_gameObject.h>
#include <dr_file_system.h>
#include <dr_file.h>

#include "dr_serializable_sound.h"
//#include "dr_serializable_aabb.h"
#include "dr_serializable_render.h"
#include "dr_serializable_camera.h"
#include "dr_serializable_light.h"
#include "dr_quaternion.h"
#include "dr_transform.h"
#include "dr_serializable_script.h"
#include "dr_serializable_boxcollider.h"
#include "dr_serializable_spherecollider.h"
#include "dr_serializable_rigidbody.h"

namespace driderSDK {

ResourceManager::ResourceManager() {}

ResourceManager::~ResourceManager() {}

void
ResourceManager::onStartUp() {
  auto codecTexture  = dr_make_unique<CodecTexture>();
  auto codecModel = dr_make_unique<CodecModel>();
  auto codecSound = dr_make_unique<CodecSound>();
  auto codecScript = dr_make_unique<CodecScript>();
  auto codecCTexture = dr_make_unique<CodecCompressedTexture>();
	auto codecScene = dr_make_unique<CodecScene>();
  auto codecMaterial = dr_make_unique<CodecMaterial>();
  
  m_resourceFactories[codecTexture.get()] = std::make_shared<TextureCore>;
  m_resourceFactories[codecModel.get()] = std::make_shared<Model>;
  m_resourceFactories[codecSound.get()] = std::make_shared<SoundCore>;
  m_resourceFactories[codecScript.get()] = std::make_shared<ScriptCore>;
  m_resourceFactories[codecCTexture.get()] = std::make_shared<TextureCore>;
	m_resourceFactories[codecScene.get()] = std::make_shared<SceneCore>;
  m_resourceFactories[codecMaterial.get()] = std::make_shared<Material>;

  m_codecs.push_back(std::move(codecModel));
  m_codecs.push_back(std::move(codecTexture));
  m_codecs.push_back(std::move(codecSound));
  m_codecs.push_back(std::move(codecScript));
  m_codecs.push_back(std::move(codecCTexture));
	m_codecs.push_back(std::move(codecScene));
  m_codecs.push_back(std::move(codecMaterial));

  createDefaultResources();
  
  auto seSound = std::make_shared<sSound>();
  //auto cAABB = std::make_shared<sAABBCollider>();
  auto seRender = std::make_shared<sRender>();
  auto seCamera = std::make_shared<sCamera>();
  auto seLight = std::make_shared<sLight>();
  auto seScript = std::make_shared<sScript>();
  auto seBoxColl = std::make_shared<sBoxCollider>();
  auto seSphereColl = std::make_shared<sSphereCollider>();
  auto seRigidBody = std::make_shared<sRigidBody>();

  componentLoaders[SerializableTypeID::Sound] = seSound;
  //componentLoaders[SerializableTypeID::AABB] = cAABB;
  componentLoaders[SerializableTypeID::Render] = seRender;
  componentLoaders[SerializableTypeID::Camera] = seCamera;
  componentLoaders[SerializableTypeID::Light] = seLight;
  componentLoaders[SerializableTypeID::Script] = seScript;
  componentLoaders[SerializableTypeID::BoxCollider] = seBoxColl;
  componentLoaders[SerializableTypeID::SphereCollider] = seSphereColl;
  componentLoaders[SerializableTypeID::RigidBody] = seRigidBody;

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
ResourceManager::saveScene(const String pathName) {
  auto &rm = ResourceManager::instance();
  auto &sg = SceneGraph::instance();
  
  FileSystem fileSystem;
  File sceneFile;
  if(fileSystem.CreateAndOpen(StringUtils::toTString(pathName + ".txt").c_str(),
                           sceneFile)) {
    //sceneFile.m_file << name;
    //sceneFile.m_file << sg.getRoot()->gameObjectsCount();
    sg.getRoot()->serialize(sceneFile);

    sceneFile.Close();
  }
  else {
    Logger::instancePtr()->addError(__FILE__,
                                    __LINE__,
                                    L"[ResourceManager] Scene file wasn't saved");
  }

}

void
ResourceManager::saveMaterial(const String name) {
  auto &rm = ResourceManager::instance();

  FileSystem fileSystem;
  File matFile;
  if (fileSystem.CreateAndOpen(StringUtils::toTString(name).c_str(),
                               matFile)) {
   
    /*
    name: string;
    m_proyectShadow: bool;
    num_properties: int;
    properties: vector<Propertie>;
    */
    auto resourceName = FileSystem::GetFileName(StringUtils::toTString(name));
    auto extension = L".mat";
    auto matResource = rm.getReference(StringUtils::toTString(resourceName + extension));
    if(!matResource) {
      matResource = rm.createMaterial(StringUtils::toTString(resourceName + extension), true);

      auto mat = std::dynamic_pointer_cast<Material>(matResource);

      auto realName = StringUtils::toString(resourceName + extension);
      matFile.m_file << realName.c_str() << "\n";
      matFile.m_file << false << "\n";

      mat->addProperty(_T("Albedo"), PROPERTY_TYPE::kVec3);
      mat->addProperty(_T("Normal"), PROPERTY_TYPE::kVec3);
      mat->addProperty(_T("Emisivity"), PROPERTY_TYPE::kVec3);
      mat->addProperty(_T("Metallic"), PROPERTY_TYPE::kVec3);
      mat->addProperty(_T("Roughness"), PROPERTY_TYPE::kVec3);

      SizeT properties = mat->getPropertiesCount();
      matFile.m_file << properties << "\n";
      for (int i = 0; i < properties; i++) {
        matFile.m_file << StringUtils::toString(mat->getProperty(i)->name) << "\n";
        matFile.m_file << mat->getProperty(i)->type << "\n";
        matFile.m_file << "null" << "\n";
      }
    } else {
      auto mat = std::dynamic_pointer_cast<Material>(matResource);

      matFile.m_file << StringUtils::toString(mat->getName()) << "\n";
      matFile.m_file << false << "\n";

      SizeT properties = mat->getPropertiesCount();
      matFile.m_file << properties << "\n";
      for (int i = 0; i < properties; i++) {
        matFile.m_file << StringUtils::toString(mat->getProperty(i)->name) << "\n";
        matFile.m_file << mat->getProperty(i)->type << "\n";
        if(mat->getProperty(i)->texture.lock()) {
          matFile.m_file << StringUtils::toString(mat->getProperty(i)->texture.lock()->getName()) << "\n";
        } else {
          matFile.m_file << "null" << "\n";
        }
      }
    }
     


    matFile.Close();
  }
  else {
    Logger::instancePtr()->addError(__FILE__,
                                    __LINE__,
                                    L"[ResourceManager] Material file wasn't saved");
  }
}

std::shared_ptr<Material>
ResourceManager::loadMaterial(const String name) {
 
  auto &rm = ResourceManager::instance();

  auto resource = rm.getReferenceT<Material>(StringUtils::toTString(name));
  if(resource) {
    return std::dynamic_pointer_cast<Material>(resource);
  }

  File file;
  file.Open(L"Resources//Materials//" + StringUtils::toTString(name));

  std::shared_ptr<Material> mat;
  if (file.Size()) {
    String name;
    file.m_file >> name;
    mat = rm.createMaterial(StringUtils::toTString(name),
                                 true); 
    
    mat->setName(StringUtils::toTString(name));

    bool m_shadow;
    file.m_file >> m_shadow;
    mat->setProyectShadow(m_shadow);

    SizeT numProp;
    file.m_file >> numProp;

    for (int i = 0; i < numProp; i++) {
      String name;
      file.m_file >> name;
      TString tName = StringUtils::toTString(name);

      UInt32 propType;
      file.m_file >> propType;

      String textureName;
      file.m_file >> textureName;
      TString tTextureName = StringUtils::toTString(textureName);
      if(tTextureName != L"null") {
        ResourceManager::loadResource(tTextureName);
      }

      mat->addProperty(tName, (PROPERTY_TYPE::E)propType);
      auto text = ResourceManager::getReferenceT<TextureCore>(tTextureName);
      if(text) {
        mat->setTexture(text, tName);
      }
    }
  }
  else {
    Logger::instancePtr()->addError(__FILE__,
                                    __LINE__,
                                    L"[Resouce Manager] File material not found");
  }

  return mat;
}

void
ResourceManager::loadScene(const String name) {
  File sceneFile;
  if(sceneFile.Open(StringUtils::toTString(name))) {
    Int32 numChildsRoot;
    sceneFile.m_file >> numChildsRoot;
    
    for(int i = 0; i < numChildsRoot; i++) {
      ResourceManager::instance().loadGameObject(sceneFile);
    }
  } 
  else {
    Logger::instancePtr()->addError(__FILE__,
                                    __LINE__,
                                    L"[ResourceManager] The scene file was not found");
  }

  SceneGraph::start();
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
  pResource->setName(resourceName);
  instance().m_resources[resourceName] =  pResource;
}

void
ResourceManager::loadGameObject(File &file) {
  auto sg = SceneGraph::instancePtr();

  String name;
  file.m_file >> name;
  auto obj = sg->createObject(StringUtils::toTString(name));

  Vector3D pos;
  file.m_file >> pos.x;
  file.m_file >> pos.y;
  file.m_file >> pos.z;
  obj->getTransform().setPosition(pos);

  Quaternion rot;
  file.m_file >> rot.x;
  file.m_file >> rot.y;
  file.m_file >> rot.z;
  file.m_file >> rot.w;
  float rX = rot.getEulerAngles().x;
  float rY = rot.getEulerAngles().y;
  float rZ = rot.getEulerAngles().z;

  obj->getTransform().setRotation({rX, rY, rZ});
  
  Vector3D scale;
  file.m_file >> scale.x;
  file.m_file >> scale.y;
  file.m_file >> scale.z;
  obj->getTransform().setScale(scale);
  

  Int32 numComponents;
  file.m_file >> numComponents;
  for(int i = 0; i < numComponents; i++) {
    loadComponent(file, obj);
  }

  //father->setParent(father);
  GameObject::SharedGameObj father;
  if(name != "ROOT_NODE_X") {
    String fatherName;
    file.m_file >> fatherName;
    if(fatherName == "ROOT_NODE_X") {
      father = sg->getRoot();
    } else {
      father = sg->getRoot()->findNode(StringUtils::toTString(fatherName));
    }
    obj->setParent(father);
  }

  Int32 numChilds;
  file.m_file >> numChilds;
  for(int j = 0; j < numChilds; j++) {
    loadGameObject(file);
  }
} 

void
ResourceManager::loadComponent(File &file, 
                               std::shared_ptr<GameObject> obj) {
  SerializableTypeID::E typeID;
  Int32 type;
  file.m_file >> type;
  typeID = (SerializableTypeID::E)type;

  auto loader = componentLoaders.find(typeID);

  if(loader != componentLoaders.end()) {
    loader->second->load(file, obj);

  }
  
  /*if(typeID == SerializableTypeID::Sound) {
    sSound s;
    s.load(file, obj);
  } else if(typeID == SerializableTypeID::AABB) {
    sAABBCollider s;
    s.load(file, obj);
  } else if (typeID == SerializableTypeID::Render) {
    sRender s;
    s.load(file, obj);
  }*/
}

void
ResourceManager::insertCompilableScript(ScriptComponent* component) {
  auto rm = ResourceManager::instancePtr();
  UInt32 id = rm->idScripts++;
  component->setIDScript(id);
  rm->m_scriptsComponents.insert({{id,component}});
}

void
ResourceManager::removeCompilableScript(UInt32 ID) {
  auto rm = ResourceManager::instancePtr();
  rm->m_scriptsComponents.erase(ID);
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
    ResourceManager::instance().m_materials.push_back(material);
  } else {
    material = ResourceManager::instance().getReferenceT<Material>(materialName);
  }

  return material;
}

void
ResourceManager::clear() {
  auto& rm = instance();
  
  auto dummyResources = std::vector<TString>{_T("DUMMY_TEXTURE"), _T("DUMMY_NORMAL_TEXTURE"), 
                                             _T("DUMMY_WHITE_TEXTURE"), _T("DUMMY_BLACK_TEXTURE"),
                                             _T("DUMMY_MATERIAL")};
  
  using ResourceIt = decltype(rm.m_resources)::value_type;

  for (auto beg = rm.m_resources.begin(); beg != rm.m_resources.end(); ) {
    if (std::find(dummyResources.begin(), dummyResources.end(), beg->first) == dummyResources.end()) {
      beg = rm.m_resources.erase(beg);
    }
    else {
      ++beg;
    }
  }

  //rm.m_materials.clear();

  using MaterialIt = decltype(rm.m_materials)::value_type;

  rm.m_materials.erase(std::remove_if(rm.m_materials.begin(), rm.m_materials.end(),
                                      [&](const MaterialIt& r){
                                        //Only remove if its not a dummy resource
                                        return std::find(dummyResources.begin(), 
                                                         dummyResources.end(), 
                                                         r->getName()) == dummyResources.begin();
                                      }),
                       rm.m_materials.end());

  rm.m_scriptsComponents.clear();
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

  ResourceManager::instance().m_materials.push_back(dummyMat);
}

}