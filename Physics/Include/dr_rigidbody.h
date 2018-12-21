#pragma once
#include <ReactPhysics/reactphysics3d.h>
#include "dr_physics_prerequisites.h"
#include <functional>
#include <dr_transform.h>
#include <map>
namespace driderSDK {
  namespace RIGID_BODY_TYPE {
    enum E {
      kStatic = 0,
      kKinematic,
      kDynamic
    };
  }
  class DR_PHYSICS_EXPORT DrRigidBody {
  public:
    void
    setTransform(Transform transform);

    int
    AddBoxShape(Vector3D dimensions, Vector3D localPos, float mass);

    int
    AddSphereShape(float radius, Vector3D localPos, float mass);

    int
    AddCapsuleShape(float radius, float height, Vector3D localPos, float mass);

    void 
    RemoveShape(const int shapeID);

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

    RIGID_BODY_TYPE::E
    getMode();

  private:
    friend class PhysicsManager;
    std::map<int,rp3d::ConvexShape*> m_shapes;
    std::map<int,rp3d::ProxyShape*> m_proxyShapes;
    rp3d::RigidBody* m_body;
    int m_nextShapeID = 0;
  };
}