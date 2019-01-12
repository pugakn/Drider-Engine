#include "dr_box_collider.h"
#include <dr_physics_manager.h>
//#include <dr_renderman.h>
#include "dr_rigidbody_component.h"
#include "dr_gameObject.h"
#include "dr_graph.h"

namespace driderSDK {

BoxCollider::BoxCollider(GameObject& _gameObject, const AABB& aabb)
  : ColliderComponent(_gameObject, _T("BoxCollider")), m_aabb(aabb) {
}

void 
BoxCollider::onCreate() {
  RigidBody3DComponent* rbody = m_gameObject.getComponent<RigidBody3DComponent>();
  if (rbody) {
    m_rigidBodyShapeID = rbody->m_rigidBody->AddBoxShape(Vector3D(m_aabb.width, m_aabb.height, m_aabb.depth), m_aabb.center, 1);
  }
   m_body = PhysicsManager::createCollisionBody(m_gameObject.getTransform());
   m_collisionShapeID = m_body->AddBoxShape(Vector3D(m_aabb.width, m_aabb.height, m_aabb.depth), m_aabb.center);
}

void 
BoxCollider::onUpdate() {
  ColliderComponent::onUpdate();
}

void 
BoxCollider::onRender() {
  /*
  RenderManager::instance().drawDebugCube(Vector3D(m_originalAABB.width,
                                                   m_originalAABB.height,
                                                   m_originalAABB.depth),
                                          Vector3D(1, 1, 1),
                                          m_gameObject.getTransform().getMatrix());
  */
}

void 
BoxCollider::onDestroy() {
  PhysicsManager::destroyCollisionBody(m_body);
}

UInt32 
BoxCollider::getClassID() {
  return CLASS_NAME_ID(BoxCollider);
}

void
BoxCollider::serialize(File &file) {
  file.m_file << SerializableTypeID::BoxCollider << "\n";
  file.m_file << StringUtils::toString(getName()) << "\n";

  file.m_file << m_aabb.width << "\n";
  file.m_file << m_aabb.height << "\n";
  file.m_file << m_aabb.depth << "\n";
  file.m_file << m_aabb.center.x << "\n";
  file.m_file << m_aabb.center.y << "\n";
  file.m_file << m_aabb.center.z << "\n";
}

void
BoxCollider::deserialize(TString &data) {

}

void BoxCollider::setAABB(const AABB & aabb)
{
  RigidBody3DComponent* rbody = m_gameObject.getComponent<RigidBody3DComponent>();
  m_aabb = aabb;
  if (rbody) {
    rbody->m_rigidBody->RemoveShape(m_rigidBodyShapeID);
    m_rigidBodyShapeID = rbody->m_rigidBody->AddBoxShape(Vector3D(m_aabb.width, m_aabb.height, m_aabb.depth), m_aabb.center, 1);
  }
  m_body->RemoveShape(m_collisionShapeID);
  m_collisionShapeID = m_body->AddBoxShape(Vector3D(m_aabb.width, m_aabb.height, m_aabb.depth), m_aabb.center);
}

AABB
BoxCollider::getAABB() {
  return m_aabb;
}


GameComponent* 
BoxCollider::cloneIn(GameObject& _go) {
  auto dup = _go.createComponent<BoxCollider>(m_aabb);
  dup->m_aabb = m_aabb;
  return dup;
}

COLLIDER_TYPE::E 
BoxCollider::getType() {
  return COLLIDER_TYPE::kBOX;
}

}