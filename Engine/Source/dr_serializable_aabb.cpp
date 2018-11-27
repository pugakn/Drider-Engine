#include "dr_serializable_aabb.h"

#include <dr_gameObject.h>
#include <dr_aabb_collider.h>
#include <dr_aabb.h>

#include "dr_file.h"
#include "dr_vector3d.h"

namespace driderSDK {
void
sAABBCollider::load(File &file,
                    std::shared_ptr<GameObject> obj) {
  String name;
  file.m_file >> name;
  
  AABB aabb;
  file.m_file >> aabb.width;
  file.m_file >> aabb.height;
  file.m_file >> aabb.depth;
  file.m_file >> aabb.center.x;
  file.m_file >> aabb.center.y;
  file.m_file >> aabb.center.z;

  obj->createComponent<AABBCollider>(aabb);
  
}
}