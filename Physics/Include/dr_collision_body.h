#pragma once
#include <ReactPhysics/reactphysics3d.h>
#include "dr_physics_prerequisites.h"
#include <functional>
#include <dr_transform.h>
#include <map>
namespace driderSDK {
  class DR_PHYSICS_EXPORT DrCollisionBody {
  public:
    void 
    setTransform(Transform transform);

    int 
    AddBoxShape(Vector3D dimensions, Vector3D localPos);

    int 
    AddSphereShape(float radius, Vector3D localPos);

    int 
    AddCapsuleShape(float radius, float height, Vector3D localPos);

    void
    RemoveShape(const int shapeID);

    std::function<void(DrCollisionBody& coll)> onCollisionEnter;
    std::function<void(DrCollisionBody& coll)> onCollisionExit;
    std::function<void(DrCollisionBody& coll)> onCollisionStay;
  private:
    friend class PhysicsManager;
    std::map<int,rp3d::ConvexShape*> m_shapes;
    std::map<int,rp3d::ProxyShape*> m_proxyShapes;
    rp3d::CollisionBody* m_body;
    int m_nextShapeID = 0;
  };
}