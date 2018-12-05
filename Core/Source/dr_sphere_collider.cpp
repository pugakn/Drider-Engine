#include "dr_sphere_collider.h"
#include <dr_physics_manager.h>
//#include <dr_renderman.h>
#include "dr_rigidbody_component.h"
#include "dr_gameObject.h"
#include "dr_graph.h"

namespace driderSDK {

SphereCollider::SphereCollider(GameObject& _gameObject,
                               const float radius,
                               const Vector3D& position)
  : ColliderComponent(_gameObject,
                      _T("BoxCollider")),
                      m_radius(radius),
                      m_center(position) {
}

void 
SphereCollider::onCreate() {
  RigidBody3DComponent* rbody = m_gameObject.getComponent<RigidBody3DComponent>();
  Vector3D r(m_radius, m_radius, m_radius);
  r = m_gameObject.getTransform().getMatrix() * r;
  if (rbody) {
    rbody->m_rigidBody->AddSphereShape(r.x,m_center, 1);
  }
  m_body = PhysicsManager::createCollisionBody(m_gameObject.getTransform());
  m_body->AddSphereShape(r.x, m_center);
}

void 
SphereCollider::onUpdate() {
  ColliderComponent::onUpdate();
}

void 
SphereCollider::onRender() {
  /*
  RenderManager::instance().drawDebugSphere(m_radius,
                                            Vector3D(1, 1, 1),
                                            m_gameObject.getTransform().getMatrix());
  */
}

void 
SphereCollider::onDestroy() {
  PhysicsManager::destroyCollisionBody(m_body);
}

UInt32 
SphereCollider::getClassID() {
  return CLASS_NAME_ID(SphereCollider);
}

GameComponent* 
SphereCollider::cloneIn(GameObject& _go) {
  auto dup = _go.createComponent<SphereCollider>(m_radius,m_center);
  return dup;
}

COLLIDER_TYPE::E 
SphereCollider::getType() {
  return COLLIDER_TYPE::kBOX;
}

}