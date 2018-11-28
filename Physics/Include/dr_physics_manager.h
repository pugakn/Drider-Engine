#pragma once
#include <ReactPhysics/reactphysics3d.h>
#include "dr_physics_prerequisites.h"
#include <dr_module.h>
#include <functional>
#include <dr_transform.h>
#include <list>
#include <unordered_map>
namespace driderSDK {
  namespace RIGID_BODY_TYPE {
    enum E {
      kStatic,
      kKinematic,
      kDynamic
    };
  }

  class DR_PHYSICS_EXPORT WorldCollisionCallback : public rp3d::CollisionCallback
  {
  public:
    WorldCollisionCallback() = default;
    ~WorldCollisionCallback() = default;
  private:
    // This method will be called for each contact
    virtual void notifyContact(const rp3d::CollisionCallback::CollisionCallbackInfo& collisionCallbackInfo) override;
  };

  class DR_PHYSICS_EXPORT DrCollisionBody {
  public:
    void setTransform(Transform transform);
    void AddBoxShape(Vector3D dimensions, Vector3D localPos);
    void AddSphereShape(float radius, Vector3D localPos);
    void AddCapsuleShape(float radius, float height, Vector3D localPos);

    std::function<void(DrCollisionBody& coll)> onCollisionEnter;
    std::function<void(DrCollisionBody& coll)> onCollisionExit ;
    std::function<void(DrCollisionBody& coll)> onCollisionStay;
  private:
    friend class PhysicsManager;
    std::list<rp3d::ConvexShape*> m_shapes;
    std::list<rp3d::ProxyShape*> m_proxyShapes;
    rp3d::CollisionBody* m_body;
  };

  class DR_PHYSICS_EXPORT DrRigidBody {
  public:
    void setTransform(Transform transform);
    void AddBoxShape(Vector3D dimensions, Vector3D localPos, float mass);
    void AddSphereShape(float radius, Vector3D localPos, float mass);
    void AddCapsuleShape(float radius, float height,Vector3D localPos, float mass);

    void setType(RIGID_BODY_TYPE::E type);
    void enableGravity(bool useGravity);
    void applyForce(Vector3D force, Vector3D point);
    void applyForceToCenter(Vector3D force);
    void applyTorque(Vector3D torque);

    Transform getTransform();

  private:
    friend class PhysicsManager;
    std::list<rp3d::ConvexShape*> m_shapes;
    std::list<rp3d::ProxyShape*> m_proxyShapes;
    rp3d::RigidBody* m_body;
  };


  class DR_PHYSICS_EXPORT PhysicsManager : public Module<PhysicsManager>{
  public:
    PhysicsManager() = default;
    ~PhysicsManager() = default;
    static DrCollisionBody* createCollisionBody(Transform transform);
    static DrRigidBody* createRigidBody(Transform transform);
    static void destroyCollisionBody(DrCollisionBody* body);
    static void destroyRigidBody(DrRigidBody* body);
    static void TestCollision();
    static void simulate();
    /*
    * TEST::onStartUp
    *
    */
    void
      onStartUp();

    /*
    * TEST::onStartUp
    *
    */
    void
      onShutDown();

    //std::vector<std::pair<DrCollisionBody*, DrCollisionBody*>> m_collisions;
  private:
    friend class DrCollisionBody;
    friend class WorldCollisionCallback;
    rp3d::CollisionWorld* m_world;
    WorldCollisionCallback m_collisionCallback;

    // Gravity vector 
    rp3d::Vector3 m_gravity;
    rp3d::DynamicsWorld* m_dynamicWorld;

    std::unordered_map<rp3d::CollisionBody*, DrCollisionBody*> m_collisionItems;
    std::unordered_map<DrCollisionBody*, DrCollisionBody*> m_currrentCollidingItems;
    std::unordered_map<DrCollisionBody*, DrCollisionBody*> m_lastCollidingItems;

    float accumulator = 0.0f;
  };
}