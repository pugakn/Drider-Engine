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
                      _T("SphereCollider")),
                      m_radius(radius),
                      m_center(position) {
}

void 
SphereCollider::onCreate() {
  RigidBody3DComponent* rbody = m_gameObject.getComponent<RigidBody3DComponent>();
  if (rbody) {
    m_rigidBodyShapeID = rbody->m_rigidBody->AddSphereShape(m_radius,m_center, 1);
  }
  m_body = PhysicsManager::createCollisionBody(m_gameObject.getTransform());
  m_collisionShapeID = m_body->AddSphereShape(m_radius, m_center);
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

void
SphereCollider::serialize(File &data) {

}

void
SphereCollider::deserialize(TString &data) {

}

void 
SphereCollider::setSize(float radius)
{
  RigidBody3DComponent* rbody = m_gameObject.getComponent<RigidBody3DComponent>();
  m_radius = radius;
  if (rbody) {
    rbody->m_rigidBody->RemoveShape(m_rigidBodyShapeID);
    m_rigidBodyShapeID = rbody->m_rigidBody->AddSphereShape(m_radius, m_center, 1);
  }
  m_body->RemoveShape(m_collisionShapeID);
  m_collisionShapeID = m_body->AddSphereShape(m_radius, m_center);
}

void 
SphereCollider::setLocalPosition(const Vector3D & center)
{
  RigidBody3DComponent* rbody = m_gameObject.getComponent<RigidBody3DComponent>();
  m_center = center;
  if (rbody) {
    rbody->m_rigidBody->RemoveShape(m_rigidBodyShapeID);
    m_rigidBodyShapeID = rbody->m_rigidBody->AddSphereShape(m_radius, m_center, 1);
  }
  m_body->RemoveShape(m_collisionShapeID);
  m_collisionShapeID = m_body->AddSphereShape(m_radius, m_center);
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

float
SphereCollider::getRadius() {
  return m_radius;
}

Vector3D
SphereCollider::getCenter() {
  return m_center;
}

}