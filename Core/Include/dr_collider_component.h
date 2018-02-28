#pragma once

#include "dr_core_prerequisites.h"
#include "dr_gameComponent.h"

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

protected:

};

}
