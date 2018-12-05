#include "dr_collider_component.h"
#include <dr_time.h>
#include <dr_math.h>
#include <dr_physics_manager.h>
#include "dr_graph.h"
#include "dr_aabb_collider.h"
#include "dr_gameObject.h"
#include "dr_rigidbody_component.h"
namespace driderSDK {
  void ColliderComponent::onUpdate()
  {
    m_body->setTransform(m_gameObject.getTransform());
  }
}