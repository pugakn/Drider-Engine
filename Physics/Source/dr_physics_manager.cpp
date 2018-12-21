#include "dr_physics_manager.h"
#include <dr_time.h>
#include <dr_radian.h>
#include <dr_quaternion.h>
namespace driderSDK {

  DrCollisionBody* 
  PhysicsManager::createCollisionBody(Transform transform)
  {
    auto p = transform.getPosition();
    rp3d::Vector3 initPosition(p.x,p.y,p.z);
    rp3d::Quaternion initOrientation;
    auto ea = transform.getEulerAngles();
    initOrientation = rp3d::Quaternion::fromEulerAngles(rp3d::Vector3(ea.x, ea.y, ea.z));
    rp3d::Transform transformRp(initPosition, initOrientation);

    DrCollisionBody *body = new DrCollisionBody();
    body->m_body = instance().m_world->createCollisionBody(transformRp);
    instance().m_collisionItems[body->m_body] = body;
    return body;
  }

  DrRigidBody* 
  PhysicsManager::createRigidBody(Transform transform)
  {
    auto p = transform.getPosition();
    rp3d::Vector3 initPosition(p.x, p.y, p.z);
    rp3d::Quaternion initOrientation;
    auto ea = transform.getEulerAngles();
    initOrientation = rp3d::Quaternion::fromEulerAngles(rp3d::Vector3(ea.x,ea.y,ea.z));
    rp3d::Transform transformRp(initPosition, initOrientation);

    DrRigidBody *body = new DrRigidBody();
    body->m_body = instance().m_dynamicWorld->createRigidBody(transformRp);
    body->m_body->setMass(1);
    return body;
  }

  void 
  PhysicsManager::destroyCollisionBody(DrCollisionBody * body)
  {
    for (auto &it: body->m_shapes)
    {
      delete it.second;
    }
    instance().m_collisionItems.erase(body->m_body);
    instance().m_world->destroyCollisionBody(body->m_body);
    delete body;
  }

  void 
  PhysicsManager::destroyRigidBody(DrRigidBody * body)
  {
    for (auto &it : body->m_shapes)
    {
      delete it.second;
    }
    instance().m_dynamicWorld->destroyRigidBody(body->m_body);
    delete body;
  }

  void 
  PhysicsManager::TestCollision()
  {
    instance().m_currrentCollidingItems.clear();
    instance().m_world->testCollision(&instance().m_collisionCallback);
    for (auto it: instance().m_lastCollidingItems)
    {
      if (instance().m_currrentCollidingItems.find(it.first) == instance().m_currrentCollidingItems.end())
      {
        if (it.first->onCollisionExit)
          it.first->onCollisionExit(*it.second);
        if (it.second->onCollisionExit)
          it.second->onCollisionExit(*it.first);
      }
    }

    instance().m_lastCollidingItems = instance().m_currrentCollidingItems;
  }

  void 
  PhysicsManager::simulate()
  {
    // Constant physics time step 
    const float timeStep = 1.0 / 60.0;
    float deltaTime = Time::getDelta();
    instance().accumulator += deltaTime;

    // While there is enough accumulated time to take 
    // one or several physics steps 
    while (instance().accumulator >= timeStep) {
      // Update the Dynamics world with a constant time step 
      instance().m_dynamicWorld->update(timeStep);
      // Decrease the accumulated time 
      instance().accumulator -= timeStep;
    }
  }

  void PhysicsManager::setGravity(float g)
  {
    instance().m_dynamicWorld->setGravity(reactphysics3d::Vector3(0, g, 0));
  }

  void 
  PhysicsManager::onStartUp()
  {
    instance().m_world = new rp3d::CollisionWorld(); 
    m_gravity = rp3d::Vector3(0, -9.81, 0);
    instance().m_dynamicWorld = new rp3d::DynamicsWorld(m_gravity);
    instance().m_dynamicWorld->setContactsPositionCorrectionTechnique(rp3d::ContactsPositionCorrectionTechnique::SPLIT_IMPULSES);
  }

  void 
  PhysicsManager::onShutDown()
  {
    delete instance().m_world;
    delete instance().m_dynamicWorld;
  }

  void 
  WorldCollisionCallback::notifyContact(const rp3d::CollisionCallback::CollisionCallbackInfo & collisionCallbackInfo)
  {
    auto item1 = PhysicsManager::instance().m_collisionItems[collisionCallbackInfo.body1];
    auto item2 = PhysicsManager::instance().m_collisionItems[collisionCallbackInfo.body2];
    bool found1 = false;
    for (auto& it : PhysicsManager::instance().m_lastCollidingItems)
    {
      if (it.first == item1) {
        if (item1->onCollisionStay)
          item1->onCollisionStay(*item2);
        if (item2->onCollisionStay)
          item2->onCollisionStay(*item1);
        found1 = true;
        break;
      }
    }
    if (!found1) {
      if (item1->onCollisionEnter)
        item1->onCollisionEnter(*item2);
      if(item2->onCollisionEnter)
        item2->onCollisionEnter(*item1);
    }
    PhysicsManager::instance().m_currrentCollidingItems[item1] = item2;
  }

}