#pragma once
#include <ReactPhysics/reactphysics3d.h>
#include "dr_physics_prerequisites.h"
#include <functional>
#include <dr_transform.h>
#include <list>
namespace driderSDK {
  namespace RIGID_BODY_TYPE {
    enum E {
      kStatic,
      kKinematic,
      kDynamic
    };
  }
  class DR_PHYSICS_EXPORT DrRigidBody {
  public:
    void 
    setTransform(Transform transform);

    void 
    AddBoxShape(Vector3D dimensions, Vector3D localPos, float mass);

    void 
    AddSphereShape(float radius, Vector3D localPos, float mass);

    void 
    AddCapsuleShape(float radius, float height, Vector3D localPos, float mass);

    void 
    setType(RIGID_BODY_TYPE::E type);

    void 
    enableGravity(bool useGravity);

    void 
    applyForce(Vector3D force, Vector3D point);

    void 
    applyForceToCenter(Vector3D force);

    void 
    applyTorque(Vector3D torque);

    Transform 
    getTransform();

  private:
    friend class PhysicsManager;
    std::list<rp3d::ConvexShape*> m_shapes;
    std::list<rp3d::ProxyShape*> m_proxyShapes;
    rp3d::RigidBody* m_body;
  };
}