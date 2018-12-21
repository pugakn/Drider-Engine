#include "dr_rigidbody.h"
#include <dr_quaternion.h>
namespace driderSDK {
  void
    DrRigidBody::setTransform(Transform transform)
  {
    auto p = transform.getPosition();
    rp3d::Vector3 position(p.x, p.y, p.z);
    rp3d::Quaternion orientation = rp3d::Quaternion::identity();
    rp3d::Transform newTransform(position, orientation);

    // Move the collision body 
    m_body->setTransform(newTransform);
  }

  int
    DrRigidBody::AddBoxShape(Vector3D dimensions, Vector3D localPos, float mass)
  {
    const rp3d::Vector3 halfExtents(dimensions.x / 2.0f, dimensions.y / 2.0f, dimensions.z / 2.0f);
    // Create the box shape 
    rp3d::BoxShape* boxShape = new rp3d::BoxShape(halfExtents);
    m_shapes[m_nextShapeID] = (boxShape);
    rp3d::Transform t;
    t.setPosition(rp3d::Vector3(localPos.x, localPos.y, localPos.z));
    t.setOrientation(rp3d::Quaternion::identity());
    m_proxyShapes[m_nextShapeID] = (m_body->addCollisionShape(boxShape, t, mass));
    return m_nextShapeID++;
  }

  int
    DrRigidBody::AddSphereShape(float radius, Vector3D localPos, float mass)
  {
    rp3d::SphereShape* sphereShape = new rp3d::SphereShape(radius);

    m_shapes[m_nextShapeID] = (sphereShape);
    rp3d::Transform t;
    t.setPosition(rp3d::Vector3(localPos.x, localPos.y, localPos.z));
    t.setOrientation(rp3d::Quaternion::identity());
    m_proxyShapes[m_nextShapeID] = (m_body->addCollisionShape(sphereShape, t, mass));
    return m_nextShapeID++;
  }

  int
    DrRigidBody::AddCapsuleShape(float radius, float height, Vector3D localPos, float mass)
  {
    rp3d::CapsuleShape* shape = new rp3d::CapsuleShape(radius, height);
    m_shapes[m_nextShapeID] = (shape);
    rp3d::Transform t;
    t.setPosition(rp3d::Vector3(localPos.x, localPos.y, localPos.z));
    t.setOrientation(rp3d::Quaternion::identity());
    m_proxyShapes[m_nextShapeID] = (m_body->addCollisionShape(shape, t, mass));
    return m_nextShapeID++;
  }

  void DrRigidBody::RemoveShape(const int shapeID)
  {
    if (m_shapes[shapeID] != nullptr) {
      m_body->removeCollisionShape(m_proxyShapes[shapeID]);
      delete m_shapes[shapeID];
    }
  }

  void
    DrRigidBody::setType(RIGID_BODY_TYPE::E type)
  {
    m_body->setType((rp3d::BodyType) type);
  }

  void
    DrRigidBody::enableGravity(bool useGravity)
  {
    m_body->enableGravity(useGravity);
  }

  void
    DrRigidBody::applyForce(Vector3D force, Vector3D point)
  {
    m_body->applyForce(rp3d::Vector3(force.x, force.y, force.z), rp3d::Vector3(point.x, point.y, point.z));
  }

  void
    DrRigidBody::applyForceToCenter(Vector3D force)
  {
    m_body->applyForceToCenterOfMass(rp3d::Vector3(force.x, force.y, force.z));
  }

  void
    DrRigidBody::applyTorque(Vector3D torque)
  {
    m_body->applyTorque(rp3d::Vector3(torque.x, torque.y, torque.z));
  }

  Transform
    DrRigidBody::getTransform()
  {
    auto t = m_body->getTransform();
    auto p = t.getPosition();
    auto pq = t.getOrientation();
    Quaternion q(pq.x, pq.y, pq.z, pq.w);
    Transform rt;
    rt.setPosition(Vector3D(p.x, p.y, p.z));
    rt.setRotation(q.getEulerAngles());
    return rt;
  }

  RIGID_BODY_TYPE::E
  DrRigidBody::getMode() {
    return (RIGID_BODY_TYPE::E)m_body->getType();
  }
}