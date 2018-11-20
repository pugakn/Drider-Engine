#include "dr_physics_manager.h"
#include <dr_time.h>
#include <dr_radian.h>
#include <dr_quaternion.h>
namespace driderSDK {

  DrCollisionBody * PhysicsManager::createCollisionBody(Transform transform)
  {
    auto p = transform.getPosition();
    rp3d::Vector3 initPosition(p.x,p.y,p.z);
    rp3d::Quaternion initOrientation;
    auto ea = transform.getEulerAngles();
    initOrientation = rp3d::Quaternion::fromEulerAngles(rp3d::Vector3(ea.x, ea.y, ea.z));
    rp3d::Transform transformRp(initPosition, initOrientation);

    DrCollisionBody *body = new DrCollisionBody();
    body->m_body = instance().m_dynamicWorld->createCollisionBody(transformRp);
    return body;
  }
  DrRigidBody * PhysicsManager::createRigidBody(Transform transform)
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
    for (auto &it : body->m_shapes)
    {
      delete it;
    }
    instance().m_dynamicWorld->destroyRigidBody(body->m_body);
    delete body;
  }

  void PhysicsManager::TestCollision()
  {
    instance().m_dynamicWorld->testCollision(&instance().m_collisionCallback);
  }

  void PhysicsManager::simulate()
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

  void PhysicsManager::onStartUp()
  {
    //m_world = new rp3d::CollisionWorld(); 
    m_gravity = rp3d::Vector3(0, -9.81, 0);
    instance().m_dynamicWorld = new rp3d::DynamicsWorld(m_gravity);
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

  void DrCollisionBody::AddBoxShape(Vector3D dimensions, Vector3D localPos)
  {
    const rp3d::Vector3 halfExtents(dimensions.x, dimensions.y, dimensions.z);
    // Create the box shape 
    rp3d::BoxShape* boxShape = new rp3d::BoxShape(halfExtents);
    m_shapes.push_back(boxShape);
    rp3d::Transform t;
    t.setPosition(rp3d::Vector3(localPos.x, localPos.y, localPos.z));
    t.setOrientation(rp3d::Quaternion::identity());
    m_proxyShapes.push_back(m_body->addCollisionShape(boxShape, t));
  }

  void DrCollisionBody::AddSphereShape(float radius, Vector3D localPos)
  {
    rp3d::SphereShape* sphereShape = new rp3d::SphereShape(radius);
    m_shapes.push_back(sphereShape);
    rp3d::Transform t;
    t.setPosition(rp3d::Vector3(localPos.x, localPos.y, localPos.z));
    t.setOrientation(rp3d::Quaternion::identity());
    m_proxyShapes.push_back(m_body->addCollisionShape(sphereShape, t));
  }

  void DrCollisionBody::AddCapsuleShape(float radius, float height, Vector3D localPos)
  {
    rp3d::CapsuleShape* shape = new rp3d::CapsuleShape(radius,height);
    m_shapes.push_back(shape);
    rp3d::Transform t;
    t.setPosition(rp3d::Vector3(localPos.x, localPos.y, localPos.z));
    t.setOrientation(rp3d::Quaternion::identity());
    m_proxyShapes.push_back(m_body->addCollisionShape(shape, t));
  }


  void WorldCollisionCallback::notifyContact(const rp3d::CollisionCallback::CollisionCallbackInfo & collisionCallbackInfo)
  {
    std::cout << "Hola" << std::endl;
  }




  void DrRigidBody::setTransform(Transform transform)
  {
    auto p = transform.getPosition();
    rp3d::Vector3 position(p.x, p.y, p.z);
    rp3d::Quaternion orientation = rp3d::Quaternion::identity();
    rp3d::Transform newTransform(position, orientation);

    // Move the collision body 
    m_body->setTransform(newTransform);
  }

  void DrRigidBody::AddBoxShape(Vector3D dimensions,Vector3D localPos, float mass)
  {
    const rp3d::Vector3 halfExtents(dimensions.x/2.0f, dimensions.y/2.0f, dimensions.z/2.0f);
    // Create the box shape 
    rp3d::BoxShape* boxShape = new rp3d::BoxShape(halfExtents);
    m_shapes.push_back(boxShape);
    rp3d::Transform t;
    t.setPosition(rp3d::Vector3(localPos.x, localPos.y, localPos.z));
    t.setOrientation(rp3d::Quaternion::identity());
    m_proxyShapes.push_back(m_body->addCollisionShape(boxShape, t,mass));
  }

  void DrRigidBody::AddSphereShape(float radius, Vector3D localPos,float mass)
  {
    rp3d::SphereShape* sphereShape = new rp3d::SphereShape(radius);
    m_shapes.push_back(sphereShape);
    rp3d::Transform t;
    t.setPosition(rp3d::Vector3(localPos.x, localPos.y, localPos.z));
    t.setOrientation(rp3d::Quaternion::identity());
    m_proxyShapes.push_back(m_body->addCollisionShape(sphereShape,t,mass));
  }

  void DrRigidBody::AddCapsuleShape(float radius, float height, Vector3D localPos, float mass)
  {
    rp3d::CapsuleShape* shape = new rp3d::CapsuleShape(radius, height);
    m_shapes.push_back(shape);
    rp3d::Transform t;
    t.setPosition(rp3d::Vector3(localPos.x, localPos.y, localPos.z));
    t.setOrientation(rp3d::Quaternion::identity());
    m_proxyShapes.push_back(m_body->addCollisionShape(shape, t,mass));
  }

  void DrRigidBody::setType(RIGID_BODY_TYPE::E type)
  {
    m_body->setType((rp3d::BodyType) type);
  }

  void DrRigidBody::enableGravity(bool useGravity)
  {
    m_body->enableGravity(useGravity);
  }

  void DrRigidBody::applyForce(Vector3D force, Vector3D point)
  {
    m_body->applyForce(rp3d::Vector3(force.x, force.y, force.z), rp3d::Vector3(point.x, point.y, point.z));
  }

  void DrRigidBody::applyForceToCenter(Vector3D force)
  {
    m_body->applyForceToCenterOfMass(rp3d::Vector3(force.x, force.y, force.z));
  }

  void DrRigidBody::applyTorque(Vector3D torque)
  {
    m_body->applyTorque(rp3d::Vector3(torque.x, torque.y, torque.z));
  }

  Transform DrRigidBody::getTransform()
  {
    auto t = m_body->getTransform();
    auto p = t.getPosition();
    auto pq = t.getOrientation();
    Quaternion q(pq.x, pq.y, pq.z, pq.w);
    Transform rt;
    rt.setPosition(Vector3D(p.x,p.y,p.z));
    rt.setRotation(q.getEulerAngles());
    return rt;
  }

}