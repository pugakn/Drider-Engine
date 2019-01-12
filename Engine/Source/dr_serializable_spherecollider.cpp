#include "dr_serializable_spherecollider.h"

#include <dr_file.h>
#include <dr_gameObject.h>
#include <dr_sphere_collider.h>

namespace driderSDK {

void
sSphereCollider::load(File &file,
                      std::shared_ptr<GameObject> obj){

  //auto component = obj->createComponent<SphereCollider>();

  String componentName;
  file.m_file >> componentName;
  //component->setName(StringUtils::toTString(componentName));

  float r;
  file.m_file >> r;
  //component->setSize(r);

  Vector3D center;
  file.m_file >> center.x;
  file.m_file >> center.y;
  file.m_file >> center.z;
  //component->setLocalPosition(center);

  obj->createComponent<SphereCollider>(r, center);

}

}