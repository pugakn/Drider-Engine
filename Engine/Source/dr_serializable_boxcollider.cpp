#include "dr_serializable_boxcollider.h"

#include <dr_file.h>
#include <dr_vector3d.h>
#include <dr_gameObject.h>
#include <dr_box_collider.h>
#include <dr_aabb.h>

namespace driderSDK {

void
sBoxCollider::load(File &file,
                   std::shared_ptr<GameObject> obj) {

  //auto component = obj->createComponent<BoxCollider>();
  AABB aabb;

  String componentName;
  file.m_file >> componentName;
  //component->setName(StringUtils::toTString(componentName));

  file.m_file >> aabb.width;

  file.m_file >> aabb.height;

  file.m_file >> aabb.depth;

  file.m_file >> aabb.center.x;
  file.m_file >> aabb.center.y;
  file.m_file >> aabb.center.z;

  //component->setAABB(aabb);

  obj->createComponent<BoxCollider>(aabb);
    
}

}