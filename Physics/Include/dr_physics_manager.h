#pragma once
#include <ReactPhysics/reactphysics3d.h>
#include "dr_physics_prerequisites.h"
#include <dr_module.h>
#include <functional>
#include <dr_transform.h>
#include <list>
#include <unordered_map>
#include "dr_collision_body.h"
#include "dr_rigidbody.h"
namespace driderSDK {
  class DR_PHYSICS_EXPORT WorldCollisionCallback : public rp3d::CollisionCallback
  {
  public:
    WorldCollisionCallback() = default;
    ~WorldCollisionCallback() = default;
  private:
    // This method will be called for each contact
    virtual void notifyContact(const rp3d::CollisionCallback::CollisionCallbackInfo& collisionCallbackInfo) override;
  };


  class DR_PHYSICS_EXPORT PhysicsManager : public Module<PhysicsManager>{
  public:
    PhysicsManager() = default;
    ~PhysicsManager() = default;

    static DrCollisionBody* 
    createCollisionBody(Transform transform);

    static DrRigidBody* 
    createRigidBody(Transform transform);

    static void 
    destroyCollisionBody(DrCollisionBody* body);

    static void 
    destroyRigidBody(DrRigidBody* body);

    static void 
    TestCollision();

    static void 
    simulate();

    static void
    setGravity(float g);
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