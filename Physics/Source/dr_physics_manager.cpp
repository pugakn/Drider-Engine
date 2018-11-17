#include "dr_physics_manager.h"
#include <dr_time.h>
namespace driderSDK {

  DrCollisionBody * PhysicsManager::createCollisionBody(Transform transform)
  {
    auto p = transform.getPosition();
    rp3d::Vector3 initPosition(p.x,p.y,p.z);
    rp3d::Quaternion initOrientation = rp3d::Quaternion::identity();
    rp3d::Transform transformRp(initPosition, initOrientation);

    DrCollisionBody *body = new DrCollisionBody();
    body->m_body = instance().m_dynamicWorld->createCollisionBody(transformRp);
    return body;
  }
  DrRigidBody * PhysicsManager::createRigidBody(Transform transform)
  {
    auto p = transform.getPosition();
    rp3d::Vector3 initPosition(p.x, p.y, p.z);
    rp3d::Quaternion initOrientation = rp3d::Quaternion::identity();
    rp3d::Transform transformRp(initPosition, initOrientation);

    DrRigidBody *body = new DrRigidBody();
    body->m_body = instance().m_dynamicWorld->createRigidBody(transformRp);
    return body;
  }
  void PhysicsManager::destroyCollisionBody(DrCollisionBody * body)
  {
    for (auto &it: body->m_shapes)
    {
      delete it;
    }
    instance().m_dynamicWorld->destroyCollisionBody(body->m_body);
    delete body;
  }

  void PhysicsManager::destroyRigidBody(DrRigidBody * body)
  {
    instance().m_dynamicWorld->destroyRigidBody(body->m_body);
    delete body;
  }

  void PhysicsManager::TestCollision()
  {
    instance().m_dynamicWorld->testCollision(&instance().m_collisionCallback);
  }

  void PhysicsManager::simulate()
  {
    static float accumulator = 0.0f;
    // Constant physics time step 
    const float timeStep = 1.0 / 60.0;
    float deltaTime = Time::getDelta();
    accumulator += deltaTime;

    // While there is enough accumulated time to take 
    // one or several physics steps 
    while (accumulator >= timeStep) {
      // Update the Dynamics world with a constant time step 
      instance().m_dynamicWorld->update(timeStep);
      // Decrease the accumulated time 
      accumulator -= timeStep;
    }
  }

  void PhysicsManager::onStartUp()
  {
    //m_world = new rp3d::CollisionWorld(); 
    m_gravity = rp3d::Vector3(0, -9.81, 0);
    m_dynamicWorld = new rp3d::DynamicsWorld(m_gravity);
  }

  void PhysicsManager::onShutDown()
  {
    //delete m_world;
    delete m_dynamicWorld;
  }

  void DrCollisionBody::setTransform(Transform transform)
  {
    auto p = transform.getPosition();
    rp3d::Vector3 position(p.x, p.y, p.z);
    rp3d::Quaternion orientation = rp3d::Quaternion::identity();
    rp3d::Transform newTransform(position, orientation);

    // Move the collision body 
    m_body->setTransform(newTransform);
  }

  void DrCollisionBody::AddBoxShape(Vector3D dimensions)
  {
    const rp3d::Vector3 halfExtents(dimensions.x, dimensions.y, dimensions.z);
    // Create the box shape 
    rp3d::BoxShape* boxShape = new rp3d::BoxShape(halfExtents);
    m_shapes.push_back(boxShape);
    m_body->addCollisionShape(boxShape, rp3d::Transform::identity());
  }

  void DrCollisionBody::AddSphereShape(float radius)
  {
    // Create the sphere shape with a radius of 2m 
    rp3d::SphereShape* sphereShape = new rp3d::SphereShape(radius);
    m_shapes.push_back(sphereShape);
    m_body->addCollisionShape(sphereShape, rp3d::Transform::identity());
  }

  void DrCollisionBody::AddCapsuleShape(float radius, float height)
  {
    // Create the sphere shape with a radius of 2m 
    rp3d::CapsuleShape* shape = new rp3d::CapsuleShape(radius,height);
    m_shapes.push_back(shape);
    m_body->addCollisionShape(shape, rp3d::Transform::identity());
  }


  void WorldCollisionCallback::notifyContact(const rp3d::CollisionCallback::CollisionCallbackInfo & collisionCallbackInfo)
  {
    std::cout << "Hola" << std::endl;
  }

}