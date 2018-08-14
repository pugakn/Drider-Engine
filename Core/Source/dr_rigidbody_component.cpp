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
    m_position = getGameObject().getTransform().getPosition();
    m_linearVelocity = Vector3D(0, 0, 0);
    m_mass = 1.0f;
    m_drag = 0.0f;
    m_useGravity = true;
    m_force = Vector3D(0,0,0);
   }

  void
  RigidBody3DComponent::onUpdate() {
    auto dt = Time::getDelta();
    //Aply forces
    Vector3D linearAcceleration = m_force / m_mass;
    if (m_useGravity) {
      linearAcceleration += Vector3D(0, GRAVITY,0);
    }
    Vector3D angularAcceleration = m_torque; //
                                             //Update positions and velocities
    m_linearVelocity += linearAcceleration * dt;
    m_angularVelocity += angularAcceleration * dt;

    m_position += m_linearVelocity * dt;
    //m_rotation += m_angularVelocity * dt;

    //Detect collisions


    //Solve constrains

    //Update values
    getGameObject().getTransform().setPosition(m_position);

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
    m_linearVelocity += _force; //ASDFG
  }
}