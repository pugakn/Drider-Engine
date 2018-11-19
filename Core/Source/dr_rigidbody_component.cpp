#include "dr_rigidbody_component.h"
#include "dr_time.h"
#include "dr_gameObject.h"
#include "dr_aabb_collider.h"
namespace driderSDK {
  RigidBody3DComponent::RigidBody3DComponent(GameObject& _gameObject)
    : GameComponent(_gameObject, _T("RigidBody3D"))
  {}

  void
   RigidBody3DComponent::onCreate() {
    m_rigidBody = PhysicsManager::createRigidBody(m_gameObject.getTransform());
    //m_rigidBody->AddBoxShape(Vector3D(5,5,5),10);
   }

  void
  RigidBody3DComponent::onUpdate() {
    auto t = m_rigidBody->getTransform();
    //Update values
    getGameObject().getTransform().setPosition(t.getPosition());
    getGameObject().getTransform().setRotation(t.getEulerAngles());
  }

  void
  RigidBody3DComponent::onRender() {
  }

  void
  RigidBody3DComponent::onDestroy() {
  }

  void
  RigidBody3DComponent::cloneIn(GameObject& _go) {

  }
  void RigidBody3DComponent::addForce(Vector3D _force)
  {
    m_rigidBody->applyForceToCenter(_force);
  }
}