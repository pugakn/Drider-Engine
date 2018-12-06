#include "dr_collision_body.h"
namespace driderSDK {
  void
    DrCollisionBody::setTransform(Transform transform)
  {
    auto p = transform.getPosition();
    rp3d::Vector3 position(p.x, p.y, p.z);
    rp3d::Quaternion orientation = rp3d::Quaternion::identity();
    rp3d::Transform newTransform(position, orientation);

    // Move the collision body 
    m_body->setTransform(newTransform);
  }

  void
    DrCollisionBody::AddBoxShape(Vector3D dimensions, Vector3D localPos)
  {
    const rp3d::Vector3 halfExtents(dimensions.x / 2.0f, dimensions.y / 2.0f, dimensions.z / 2.0f);
    // Create the box shape 
    rp3d::BoxShape* boxShape = new rp3d::BoxShape(halfExtents);
    m_shapes.push_back(boxShape);
    rp3d::Transform t;
    t.setPosition(rp3d::Vector3(localPos.x, localPos.y, localPos.z));
    t.setOrientation(rp3d::Quaternion::identity());
    m_proxyShapes.push_back(m_body->addCollisionShape(boxShape, t));
  }

  void
    DrCollisionBody::AddSphereShape(float radius, Vector3D localPos)
  {
    rp3d::SphereShape* sphereShape = new rp3d::SphereShape(radius);
    m_shapes.push_back(sphereShape);
    rp3d::Transform t;
    t.setPosition(rp3d::Vector3(localPos.x, localPos.y, localPos.z));
    t.setOrientation(rp3d::Quaternion::identity());
    m_proxyShapes.push_back(m_body->addCollisionShape(sphereShape, t));
  }

  void
    DrCollisionBody::AddCapsuleShape(float radius, float height, Vector3D localPos)
  {
    rp3d::CapsuleShape* shape = new rp3d::CapsuleShape(radius, height);
    m_shapes.push_back(shape);
    rp3d::Transform t;
    t.setPosition(rp3d::Vector3(localPos.x, localPos.y, localPos.z));
    t.setOrientation(rp3d::Quaternion::identity());
    m_proxyShapes.push_back(m_body->addCollisionShape(shape, t));
  }
}