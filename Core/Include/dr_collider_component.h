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


  void onCollisionEnter(ColliderComponent& coll); //std::function<void(ColliderComponent& coll)>
  void onCollisionStay(ColliderComponent& coll);   // std::function<void(ColliderComponent& coll)>
  void onCollisionExit(ColliderComponent& coll);   // std::function<void(ColliderComponent& coll)>


  std::function<void(ColliderComponent& coll)> onTriggerEnter;
  std::vector<ColliderComponent*> m_collisions;

  bool m_isTrigger = false;
 protected:

};

}
