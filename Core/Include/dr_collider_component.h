#pragma once

#include "dr_core_prerequisites.h"
#include "dr_gameComponent.h"

#include <functional>
#include <vector>
namespace driderSDK {

namespace COLLIDER_TYPE {
enum E
{
  kAABB
};
}

class DR_CORE_EXPORT ColliderComponent : public GameComponent
{
 public:
  using GameComponent::GameComponent;

  virtual COLLIDER_TYPE::E
  getType() = 0;

  virtual void
  onUpdate() override;

  std::function<void(ColliderComponent& coll)> onCollisionEnter;
  std::function<void(ColliderComponent& coll)> onCollisionExit;
  std::function<void(ColliderComponent& coll)> onCollisionStay;


  std::function<void(ColliderComponent& coll)> onTriggerEnter;
  std::function<void(ColliderComponent& coll)> onTriggerExit;
  std::function<void(ColliderComponent& coll)> onTriggerStay;


  std::vector<ColliderComponent*> m_collisions;

  bool m_isTrigger = false;
 protected:
   void CollisionEnter(ColliderComponent& coll);
   void CollisionStay(ColliderComponent& coll);
   void CollisionExit(ColliderComponent& coll);
};

}
