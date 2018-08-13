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
    

    //Modulo para controlar fisicas?
    //Todos x todos?


    GameObject& root = *SceneGraph::getRoot().get();
    auto fnc = [&]() {
      for (auto &it : root.getChildren()) {
        auto& components = it->getComponents<ColliderComponent>();
        if (components.size()) {
          for (auto & component : components) {

            ColliderComponent* coll;
            coll = component->getCollision();
            while (coll)
            {
              if (coll->m_isTrigger || component->m_isTrigger) {
                //component->onTriggerEnter(*coll);
              }
              else {
                //component->onCollisionEnter(*coll);
                //coll->onCollisionEnter(*component);
                auto rbodi = component->getGameObject().getComponent<RigidBody3DComponent>();
                if (rbodi) {
                  //rbodi->
                }
              }

              coll = component->getCollision();
            }
          //  auto type = component->getType();
          //  switch (type)
          //  {
          //  case COLLIDER_TYPE::kAABB:
          //    ((AABBCollider*)component)->getTransformedAABB().intersect(component);
          //    break;
          //  default:
          //    break;
          //  }
          }
        }
      }
    };
    //Solve constrains

  }
}