#include "dr_collider_component.h"
#include "dr_graph.h"
#include "dr_aabb_collider.h"
#include "dr_gameObject.h"
#include "dr_rigidbody_component.h"
#include <dr_time.h>
#include <dr_math.h>
namespace driderSDK {
  void ColliderComponent::onUpdate()
  {
    ////Check collisions
    //auto lastCollisions = m_collisions;
    //m_collisions.clear();
    //GameObject* root = SceneGraph::getRoot().get();
    //for (auto &it : root->getChildren()) {
    //  if (it.get() == this->getGameObjectPtr()) { //Do not check with the same GMO
    //    continue;
    //  }
    //  auto rbodi = getGameObject().getComponent<RigidBody3DComponent>();
    //  auto otherRbodi = it->getComponent<RigidBody3DComponent>();
    //  if ((rbodi || otherRbodi)) { //&& (!rbodi->m_isKinematic || !otherRbodi->m_isKinematic)
    //  auto components = it->getComponents<AABBCollider>(); //TODO: Add Other colliders
    //  if (components.size()) {
    //    for (auto & component : components) {
    //        if (((AABBCollider*)this)->getTransformedAABB().intersect(component->getTransformedAABB())) { //TODO: Error when it is not AABB xDXdXDDD
    //          if (std::find(lastCollisions.begin(), lastCollisions.end(), component) != lastCollisions.end()) {
    //            //Already collisioning
    //            CollisionStay(*component);
    //          }
    //          else {
    //            //New collision
    //            CollisionEnter(*component);
    //          }
    //          m_collisions.push_back(component);
    //        }
    //      }
    //    }
    //  }
    //}
    //for (auto & it : lastCollisions) {
    //  if (std::find(m_collisions.begin(), m_collisions.end(), it) == m_collisions.end()) {
    //    CollisionExit(*it);
    //  }
    //}
  }
  //void ColliderComponent::CollisionEnter(ColliderComponent & coll)
  //{
  //  if (m_isTrigger) {
  //    std::cout << "Trigger Enter!" << std::endl;
  //    if (onTriggerEnter) {
  //      onTriggerEnter(coll);
  //    }
  //  }
  //  else if (!coll.m_isTrigger) {
  //    std::cout << "Collision Enter!" << std::endl;
  //    CollisionRespone(coll);
  //    if (onCollisionEnter) {
  //      onCollisionEnter(coll);
  //    }
  //  }
  //}
  //void ColliderComponent::CollisionStay(ColliderComponent & coll)
  //{
  //  if (m_isTrigger) {
  //    std::cout << "Trigger Stay!" << std::endl;
  //    if (onTriggerStay) {
  //      onTriggerStay(coll);
  //    }
  //  }
  //  else if (!coll.m_isTrigger) {
  //    std::cout << "Collision Stay!" << std::endl;
  //    CollisionRespone(coll);
  //    //auto dt = Time::getDelta();
  //    //auto rgbody = getGameObject().getComponent<RigidBody3DComponent>();
  //    //if (rgbody) {
  //    //  const float restCoef = 0.4;
  //    //  Vector3D rayOrigin = rgbody->m_position - rgbody->m_linearVelocity;
  //    //  Vector3D rayDir;
  //    //  if (rgbody->m_position == rayOrigin) {
  //    //    rayDir = Vector3D(0, 1, 0);
  //    //  }
  //    //  else {
  //    //    rayDir = (rgbody->m_position - rayOrigin).normalize();
  //    //  }
  //    //  float w = abs((((AABBCollider*)&coll)->getTransformedAABB().width + ((AABBCollider*)&coll)->getGameObject().getWorldTransform().getPosition().x) - (((AABBCollider*)this)->getTransformedAABB().width + ((AABBCollider*)this)->getGameObject().getWorldTransform().getPosition().x));
  //    //  float h = abs((((AABBCollider*)&coll)->getTransformedAABB().height + ((AABBCollider*)&coll)->getGameObject().getWorldTransform().getPosition().y) - (((AABBCollider*)this)->getTransformedAABB().height + ((AABBCollider*)this)->getGameObject().getWorldTransform().getPosition().y));
  //    //  float d = abs((((AABBCollider*)&coll)->getTransformedAABB().depth + ((AABBCollider*)&coll)->getGameObject().getWorldTransform().getPosition().z) - (((AABBCollider*)this)->getTransformedAABB().depth + ((AABBCollider*)this)->getGameObject().getWorldTransform().getPosition().z));
  //    //  Vector3D m_planeNormal;
  //    //  Vector3D m_planePoint;
  //    //  if (h < w && h < d) {
  //    //    if (((AABBCollider*)&coll)->getGameObject().getWorldTransform().getPosition().y < ((AABBCollider*)this)->getGameObject().getWorldTransform().getPosition().y) {
  //    //      m_planeNormal = Vector3D(0, 1, 0);
  //    //    }
  //    //    else {
  //    //      m_planeNormal = Vector3D(0, -1, 0);
  //    //    }
  //    //    m_planePoint = coll.getGameObject().getWorldTransform().getPosition() + m_planeNormal * ((AABBCollider*)&coll)->getTransformedAABB().height;
  //    //  }
  //    //  else if (w < d) {
  //    //    if (((AABBCollider*)&coll)->getGameObject().getWorldTransform().getPosition().x < ((AABBCollider*)this)->getGameObject().getWorldTransform().getPosition().x) {
  //    //      m_planeNormal = Vector3D(1, 0, 0);
  //    //    }
  //    //    else {
  //    //      m_planeNormal = Vector3D(-1, 0, 0);
  //    //    }
  //    //    m_planePoint = coll.getGameObject().getWorldTransform().getPosition() + m_planeNormal * ((AABBCollider*)&coll)->getTransformedAABB().width;
  //    //  }
  //    //  else {
  //    //    if (((AABBCollider*)&coll)->getGameObject().getWorldTransform().getPosition().z < ((AABBCollider*)this)->getGameObject().getWorldTransform().getPosition().z) {
  //    //      m_planeNormal = Vector3D(0, 0, 1);
  //    //    }
  //    //    else {
  //    //      m_planeNormal = Vector3D(0, 0, -1);
  //    //    }
  //    //    m_planePoint = coll.getGameObject().getWorldTransform().getPosition() + m_planeNormal * ((AABBCollider*)&coll)->getTransformedAABB().depth;
  //    //  }
  //    //  float denom = rayDir.dot(m_planeNormal);
  //    //  float t = (m_planePoint - rayOrigin).dot(m_planeNormal) / denom;
  //    //  rgbody->m_position = rayOrigin + rayDir * t;
  //    //  float de = rgbody->m_linearVelocity.dot(m_planeNormal);
  //    //  float j = Math::max(-(1 + restCoef) * de, 0.0f);
  //    //  rgbody->m_linearVelocity += m_planeNormal * j;
  //    //  rgbody->m_position += (m_planeNormal * j) * dt;
  //    
  //    if (onCollisionStay) {
  //      onCollisionStay(coll);
  //    }
  //  }
  //}
  //void ColliderComponent::CollisionExit(ColliderComponent & coll)
  //{
  //  if (m_isTrigger) {
  //    std::cout << "Trigger Exit!" << std::endl;
  //    if (onTriggerExit) {
  //      onTriggerExit(coll);
  //    }
  //  }
  //  else if (!coll.m_isTrigger) {
  //    std::cout << "Collision Exit!" << std::endl;
  //    if (onCollisionExit) {
  //      onCollisionExit(coll);
  //    }
  //  }
  //}
  //void ColliderComponent::CollisionRespone(ColliderComponent & coll)
  //{
  //  auto rgbody = getGameObject().getComponent<RigidBody3DComponent>();
  //  if (rgbody) {
  //    auto dt = Time::getDelta();
  //    Vector3D normal(0, 1, 0);
  //    float penetration = 0;
  //    auto& A = this->getGameObject();
  //    auto B = coll.getGameObjectPtr();
  //    Vector3D n = B->getWorldTransform().getPosition() - A.getWorldTransform().getPosition();

  //    const AABB& abox = ((AABBCollider*)this)->getTransformedAABB();
  //    const AABB& bbox = ((AABBCollider*)&coll)->getTransformedAABB();
  //    float a_extent = (abox.getMaxPoint().x - abox.getMinPoint().x) / 2;
  //    float b_extent = (bbox.getMaxPoint().x - bbox.getMinPoint().x) / 2;
  //    float x_overlap = a_extent + b_extent - abs(n.x);

  //    if (x_overlap > 0)
  //    {
  //      float a_extent = (abox.getMaxPoint().y - abox.getMinPoint().y) / 2;
  //      float b_extent = (bbox.getMaxPoint().y - bbox.getMinPoint().y) / 2;
  //      float y_overlap = a_extent + b_extent - abs(n.y);

  //      if (y_overlap > 0)
  //      {
  //        float a_extent = (abox.getMaxPoint().z - abox.getMinPoint().z) / 2;
  //        float b_extent = (bbox.getMaxPoint().z - bbox.getMinPoint().z) / 2;
  //        float z_overlap = a_extent + b_extent - abs(n.z);

  //        if (z_overlap > 0)
  //        {
  //          // Find out which axis is axis of least penetration
  //          if (x_overlap < y_overlap && x_overlap < z_overlap)
  //          {
  //            if (n.x < 0)
  //              normal = Vector3D(-1, 0, 0);
  //            else
  //              normal = Vector3D(1, 0, 0);
  //            penetration = x_overlap;
  //          }
  //          else if (y_overlap < x_overlap && y_overlap < z_overlap)
  //          {
  //            if (n.y < 0)
  //              normal = Vector3D(0, -1, 0);
  //            else
  //              normal = Vector3D(0, 1, 0);
  //            penetration = y_overlap;
  //          }
  //          else {
  //            if (n.z < 0)
  //              normal = Vector3D(0, 0, -1);
  //            else
  //              normal = Vector3D(0, 0, 1);
  //            penetration = z_overlap;
  //          }

  //          Vector3D AVel(0, 0, 0);
  //          Vector3D BVel(0, 0, 0);
  //          float Amass = 1;
  //          float Bmass = 1;
  //          AVel = rgbody->m_linearVelocity;
  //          Amass = rgbody->m_mass;

  //          auto collRgbody = coll.getGameObject().getComponent<RigidBody3DComponent>();
  //          if (collRgbody) {
  //            BVel = collRgbody->m_linearVelocity;
  //            Bmass = collRgbody->m_mass;
  //          }
  //          // Calculate relative velocity
  //          Vector3D rv = AVel - BVel;
  //          float velAlongNormal = rv.dot(normal);
  //          float e = Math::min(0.5, 0.5);

  //          // Calculate impulse scalar
  //          float j = -(1 + e) * velAlongNormal;
  //          j /= 1 / Amass + 1 / Bmass;
  //          // Apply impulse
  //          Vector3D impulse = normal * j;

  //          float mass_sum = Amass + Bmass;
  //          float ratio = Amass / mass_sum;

  //          rgbody->m_linearVelocity += impulse * ratio;

  //          const float percent = 0.2; // usually 20% to 80%
  //          const float slop = 0.01; // usually 0.01 to 0.1
  //          Vector3D correction = n * (Math::max(penetration - slop, 0.0f) / (1.0 / Amass + 1.0 / Bmass)) *percent;
  //          // rgbody->m_position +=  correction *(1.0 / Amass) * dt ;
  //          rgbody->m_position += n * j * dt;
  //        }
  //      }
  //    }
  //  }
  //}
}