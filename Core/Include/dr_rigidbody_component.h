#pragma once

#include "dr_core_prerequisites.h"
#include "dr_gameComponent.h"
#include <dr_vector3d.h>
#include <dr_quaternion.h>
#include <dr_matrix3x3.h>
#include <vector>
namespace driderSDK {
  class DR_CORE_EXPORT RigidBody3DComponent : public GameComponent
  {
  public:
    RigidBody3DComponent(GameObject& _gameObject);
    virtual void
      onCreate() override;

    virtual void
      onUpdate() override;

    virtual void
      onRender() override;

    virtual void
      onDestroy() override;

    virtual void
      cloneIn(GameObject& _go) override;


    float m_mass;
    float m_drag;
    float m_angularDrag;

    bool m_isKinematic;
    bool m_useGravity;
    bool m_detectCollisions;
    bool m_freezeRotation;

    Vector3D m_position;
    Vector3D m_angularVelocity;
    Vector3D m_linearVelocity;

    Vector3D m_force;
    Vector3D m_torque;

    Quaternion m_rotation;
    Matrix3x3 m_inertiaTensor;
  protected:

  };

}

