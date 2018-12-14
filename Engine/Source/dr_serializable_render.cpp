#include "dr_serializable_render.h"

#include <dr_gameObject.h>
#include <dr_file.h>
#include <dr_aabb.h>

#include <dr_resource_manager.h>
#include <dr_render_component.h>
#include <dr_model.h>
#include <dr_texture_core.h>

namespace driderSDK {
void
sRender::load(File &file,
     std::shared_ptr<GameObject> obj) {
  String name;
  file.m_file >> name;
  
  bool isModel;
  file.m_file >> isModel;

  if(isModel) {
    String modelName;
    file.m_file >> modelName;
    //Load model
    ResourceManager::loadResource(StringUtils::toTString(modelName));
    auto model = ResourceManager::getReferenceT<Model>(StringUtils::toTString(modelName));
    obj->createComponent<RenderComponent>(model);
   
    //Material
    Int32 numMat;
    file.m_file >> numMat;
    
    for(int i = 0; i < numMat; i++) {
      String name;
      file.m_file >> name;
      
      ResourceManager::createMaterial(StringUtils::toTString(name), true);
      auto mat = ResourceManager::getReferenceT<Material>(StringUtils::toTString(name));

      bool shadow;
      file.m_file >> shadow;
      mat->setProyectShadow(shadow);

      UInt32 numProperties;
      file.m_file >> numProperties;
      for(int i = 0; i < numProperties; i++) {
        String name;
        file.m_file >> name;
        TString tName = StringUtils::toTString(name);
        
        UInt32 propType;
        file.m_file >> propType;
        
        String textureName;
        file.m_file >> textureName;
        TString tTextureName = StringUtils::toTString(textureName);
        ResourceManager::loadResource(tTextureName);
        
        mat->addProperty(tName,
                        (PROPERTY_TYPE::E)propType);
        auto text = ResourceManager::getReferenceT<TextureCore>(tTextureName);
        mat->setTexture(text, tName);
      }
    
      model->meshes[i].material = mat;
    }

  }

  /*
  	width: float;
	height: float;
	depth: float;
	center: vector3D;
  */ 
}


}