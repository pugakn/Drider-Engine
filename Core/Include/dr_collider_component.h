#pragma once

#include "dr_core_prerequisites.h"
#include "dr_gameComponent.h"

#include <functional>
#include <vector>

namespace driderSDK {
class DrCollisionBody;
namespace COLLIDER_TYPE {
enum E
{
  kAABB,
  kBOX,
  kSPHERE
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

  DrCollisionBody * m_body;
 protected:
};

}
