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

  int
    DrCollisionBody::AddBoxShape(Vector3D dimensions, Vector3D localPos)
  {
    const rp3d::Vector3 halfExtents(dimensions.x / 2.0f, dimensions.y / 2.0f, dimensions.z / 2.0f);
    // Create the box shape 
    rp3d::BoxShape* boxShape = new rp3d::BoxShape(halfExtents);
    m_shapes[m_nextShapeID] = (boxShape);
    rp3d::Transform t;
    t.setPosition(rp3d::Vector3(localPos.x, localPos.y, localPos.z));
    t.setOrientation(rp3d::Quaternion::identity());
    m_proxyShapes[m_nextShapeID] = (m_body->addCollisionShape(boxShape, t));
    return m_nextShapeID++;
  }

  int
    DrCollisionBody::AddSphereShape(float radius, Vector3D localPos)
  {
    rp3d::SphereShape* sphereShape = new rp3d::SphereShape(radius);
    m_shapes[m_nextShapeID] = (sphereShape);
    rp3d::Transform t;
    t.setPosition(rp3d::Vector3(localPos.x, localPos.y, localPos.z));
    t.setOrientation(rp3d::Quaternion::identity());
    m_proxyShapes[m_nextShapeID] = (m_body->addCollisionShape(sphereShape, t));
    return m_nextShapeID++;
  }

  int
    DrCollisionBody::AddCapsuleShape(float radius, float height, Vector3D localPos)
  {
    rp3d::CapsuleShape* shape = new rp3d::CapsuleShape(radius, height);
    m_shapes[m_nextShapeID] = (shape);
    rp3d::Transform t;
    t.setPosition(rp3d::Vector3(localPos.x, localPos.y, localPos.z));
    t.setOrientation(rp3d::Quaternion::identity());
    m_proxyShapes[m_nextShapeID] = (m_body->addCollisionShape(shape, t));
    return m_nextShapeID++;
  }
  void DrCollisionBody::RemoveShape(const int shapeID)
  {
    if (m_shapes[shapeID] != nullptr) {
      m_body->removeCollisionShape(m_proxyShapes[shapeID]);
      delete m_shapes[shapeID];
    }
  }
}