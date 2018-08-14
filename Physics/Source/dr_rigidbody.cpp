#include "dr_rigidbody.h"
#include <dr_time.h>

#include <dr_gameObject.h>
#include <dr_graph.h>

#include <dr_rigidbody_component.h>
namespace driderSDK {
  void 
  RigidBody3D::Update()
  {
    auto dt = Time::getDelta();
    //Aply forces
    Vector3D linearAcceleration = m_force / m_mass;
    Vector3D angularAcceleration = m_torque; //
    //Update positions and velocities
    m_linearVelocity += linearAcceleration * dt;
    m_angularVelocity += angularAcceleration * dt;

    m_position += m_linearVelocity * dt;
    //m_rotation += m_angularVelocity * dt;

    //Detect collisions
    
    //Solve constrains

  }
}