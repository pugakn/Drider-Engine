#include "dr_collider_component.h"
#include "dr_graph.h"
#include "dr_aabb_collider.h"
#include "dr_gameObject.h"
#include "dr_rigidbody_component.h"
namespace driderSDK {
  void ColliderComponent::onUpdate()
  {
    //Check collisions
    auto lastCollisions = m_collisions;
    m_collisions.clear();
    GameObject* root = SceneGraph::getRoot().get();
    for (auto &it : root->getChildren()) {
      if (it.get() == this->getGameObjectPtr()) {
        continue;
      }
      auto rbodi = getGameObject().getComponent<RigidBody3DComponent>();
      auto otherRbodi = it->getComponent<RigidBody3DComponent>();
      auto& components = it->getComponents<AABBCollider>(); //TODO: Add Other colliders
      if (components.size()) {
        for (auto & component : components) {
          if ((rbodi || otherRbodi)) { //&& (!rbodi->m_isKinematic || !otherRbodi->m_isKinematic)
            if (((AABBCollider*)this)->getTransformedAABB().intersect(component->getTransformedAABB())) { //TODO: Error when it is not AABB xDXdXDDD
              if (std::find(lastCollisions.begin(), lastCollisions.end(), component) != lastCollisions.end()) {
                //Already collisioning
                CollisionStay(*component);
              }
              else {
                //New collision
                CollisionEnter(*component);
              }
              m_collisions.push_back(component);
            }
          }
          else {

          }
        }
      }
    }
    for (auto & it : lastCollisions) {
      if (std::find(m_collisions.begin(), m_collisions.end(), it) == m_collisions.end()) {
        CollisionExit(*it);
      }
    }
  }
  void ColliderComponent::CollisionEnter(ColliderComponent & coll)
  {
    if (m_isTrigger) {
      std::cout << "Trigger Enter!" << std::endl;
      if (onTriggerEnter) {
        onTriggerEnter(coll);
      }
    }
    else if (!coll.m_isTrigger) {
      std::cout << "Collision Enter!" << std::endl;
      if (onCollisionEnter) {
        onCollisionEnter(coll);
      }
    }
  }
  void ColliderComponent::CollisionStay(ColliderComponent & coll)
  {
    if (m_isTrigger) {
      std::cout << "Trigger Stay!" << std::endl;
      if (onTriggerStay) {
        onTriggerStay(coll);
      }
    }
    else if (!coll.m_isTrigger) {
      std::cout << "Collision Stay!" << std::endl;
      if (onCollisionStay) {
        onCollisionStay(coll);
      }
    }
  }
  void ColliderComponent::CollisionExit(ColliderComponent & coll)
  {
    if (m_isTrigger) {
      std::cout << "Trigger Exit!" << std::endl;
      if (onTriggerExit) {
        onTriggerExit(coll);
      }
    }
    else if (!coll.m_isTrigger) {
      std::cout << "Collision Exit!" << std::endl;
      if (onCollisionExit) {
        onCollisionExit(coll);
      }
    }
  }
}