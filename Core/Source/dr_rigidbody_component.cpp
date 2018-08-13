#include "dr_rigidbody_component.h"
namespace driderSDK {
  RigidBody3DComponent::RigidBody3DComponent(GameObject& _gameObject)
    : GameComponent(_gameObject, _T("RigidBody3D"))
  {}

  void
   RigidBody3DComponent::onCreate() {

   }

  void
  RigidBody3DComponent::onUpdate() {

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
}