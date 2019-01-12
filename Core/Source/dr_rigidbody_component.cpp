#include "dr_rigidbody_component.h"
#include "dr_time.h"
#include "dr_gameObject.h"
#include "dr_aabb_collider.h"
#include <dr_physics_manager.h>
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
    getGameObject().getTransform().setRotation(t.getEulerAngles());
    getGameObject().getTransform().setPosition(t.getPosition());
  }

  void
  RigidBody3DComponent::onRender() {
  }

  void
  RigidBody3DComponent::onDestroy() {
    PhysicsManager::destroyRigidBody(m_rigidBody);
  }

  GameComponent*
  RigidBody3DComponent::cloneIn(GameObject& _go) {
    auto dup = _go.createComponent<RigidBody3DComponent>();
    return dup;
  }

  UInt32 
  RigidBody3DComponent::getClassID() {
    return CLASS_NAME_ID(RigidBody3DComponent);
  }

  void 
  RigidBody3DComponent::addForce(Vector3D _force)
  {
    m_rigidBody->applyForceToCenter(_force);
  }

  void 
  RigidBody3DComponent::addTorque(Vector3D torque) {
    m_rigidBody->applyTorque(torque);
  }

  void
  RigidBody3DComponent::setTransform(Transform transform) {
    m_rigidBody->setTransform(transform);
  }

  void
    RigidBody3DComponent::enableGravity(bool bUseGravity) {
    isGravityEnabled = bUseGravity;
    m_rigidBody->enableGravity(bUseGravity);
  }

  void 
  RigidBody3DComponent::setType(RIGID_BODY_TYPE::E type) {
    m_rigidBody->setType(type);
  }

  void
  RigidBody3DComponent::serialize(File &file) {
    file.m_file << SerializableTypeID::RigidBody << "\n";
    file.m_file << StringUtils::toString(getName()) << "\n";

    file.m_file << isGravity() << "\n";
    file.m_file << getMode() << "\n";
    
    Transform trans;
    trans = m_rigidBody->getTransform();   
    file.m_file << trans.getPosition().x << "\n";
    file.m_file << trans.getPosition().y << "\n";
    file.m_file << trans.getPosition().z << "\n";

  }
  
  void
  RigidBody3DComponent::deserialize(TString &data) {

  }

  bool
  RigidBody3DComponent::isGravity() {
    return isGravityEnabled;
  }

  RIGID_BODY_TYPE::E
  RigidBody3DComponent::getMode() {
    return (RIGID_BODY_TYPE::E)m_rigidBody->getMode();
  }

  Transform
  RigidBody3DComponent::getTransform() {
    return m_rigidBody->getTransform();
  }
}