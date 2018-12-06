#pragma once
#include <ReactPhysics/reactphysics3d.h>
#include "dr_physics_prerequisites.h"
#include <functional>
#include <dr_transform.h>
#include <list>
namespace driderSDK {
  class DR_PHYSICS_EXPORT DrCollisionBody {
  public:
    void 
    setTransform(Transform transform);

    void 
    AddBoxShape(Vector3D dimensions, Vector3D localPos);

    void 
    AddSphereShape(float radius, Vector3D localPos);

    void 
    AddCapsuleShape(float radius, float height, Vector3D localPos);

    std::function<void(DrCollisionBody& coll)> onCollisionEnter;
    std::function<void(DrCollisionBody& coll)> onCollisionExit;
    std::function<void(DrCollisionBody& coll)> onCollisionStay;
  private:
    friend class PhysicsManager;
    std::list<rp3d::ConvexShape*> m_shapes;
    std::list<rp3d::ProxyShape*> m_proxyShapes;
    rp3d::CollisionBody* m_body;
  };
}