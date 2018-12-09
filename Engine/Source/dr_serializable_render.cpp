#include "dr_serializable_render.h"

#include <dr_gameObject.h>
#include <dr_file.h>
#include <dr_aabb.h>

#include <dr_resource_manager.h>
#include <dr_render_component.h>
#include <dr_model.h>

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
    
    /*AABB aabb;
    file.m_file >> aabb.width;
    file.m_file >> aabb.height;
    file.m_file >> aabb.depth;
    file.m_file >> aabb.center.x;
    file.m_file >> aabb.center.y;
    file.m_file >> aabb.center.z;*/

  }

  /*
  	width: float;
	height: float;
	depth: float;
	center: vector3D;
  */ 
}
}