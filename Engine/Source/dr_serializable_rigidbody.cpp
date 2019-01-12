#include "dr_serializable_rigidbody.h"

#include <dr_rigidbody_component.h>
#include <dr_gameObject.h>
#include <dr_transform.h>

namespace driderSDK {

void
sRigidBody::load(File &file,
                 std::shared_ptr<GameObject> obj) {
  auto component = obj->createComponent<RigidBody3DComponent>();

  String componentName;
  file.m_file >> componentName;
  TString name = StringUtils::toTString(componentName + "\n");
  //component->setName(name);

  bool gravity;
  file.m_file >> gravity;
  component->enableGravity(gravity);

  UInt32 type;
  file.m_file >> type;
  component->setType(RIGID_BODY_TYPE::E(type));
  
  Vector3D pos;
  file.m_file >> pos.x;
  file.m_file >> pos.y;
  file.m_file >> pos.z;
  Transform t = component->getTransform();
  t.setPosition(pos);
  component->setTransform(t);
}

}