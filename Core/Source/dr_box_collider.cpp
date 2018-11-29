#include "dr_box_collider.h"
#include <dr_physics_manager.h>
#include <dr_renderman.h>
#include "dr_rigidbody_component.h"
#include "dr_gameObject.h"
#include "dr_graph.h"
namespace driderSDK {
  BoxCollider::BoxCollider(GameObject & _gameObject, const AABB& aabb)
    : ColliderComponent(_gameObject, _T("BoxCollider")), m_originalAABB(aabb)
  {
  }

  void 
  BoxCollider::onCreate()
  {
    RigidBody3DComponent* rbody = m_gameObject.getComponent<RigidBody3DComponent>();
    AABB transformedAABB = m_originalAABB;
    transformedAABB.recalculate(m_gameObject.getTransform().getMatrix());
    if (rbody) {
      rbody->m_rigidBody->AddBoxShape(Vector3D(transformedAABB.width, transformedAABB.height, transformedAABB.depth), Vector3D(0, 0, 0), 1);
    }
    m_body = PhysicsManager::createCollisionBody(m_gameObject.getTransform());
    m_body->AddBoxShape(Vector3D(transformedAABB.width, transformedAABB.height, transformedAABB.depth), Vector3D(0, 0, 0));
  }

  void 
  BoxCollider::onUpdate()
  {
    ColliderComponent::onUpdate();
  }

  void 
  BoxCollider::onRender()
  {
    RenderManager::instance().drawDebugCube(
      Vector3D(m_originalAABB.width, m_originalAABB.height, m_originalAABB.depth),
      Vector3D(1, 1, 1), m_gameObject.getTransform().getMatrix());
  }

  void 
  BoxCollider::onDestroy()
  {
    PhysicsManager::destroyCollisionBody(m_body);
  }

  UInt32 
  BoxCollider::getClassID()
  {
    return CLASS_NAME_ID(BoxCollider);
  }

  GameComponent* 
  BoxCollider::cloneIn(GameObject & _go)
  {
    auto dup = _go.createComponent<BoxCollider>(m_originalAABB);
    dup->m_originalAABB = m_originalAABB;
    return dup;
  }

  COLLIDER_TYPE::E 
  BoxCollider::getType()
  {
    return COLLIDER_TYPE::kBOX;
  }
}