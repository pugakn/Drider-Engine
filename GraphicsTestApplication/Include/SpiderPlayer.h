#pragma once

#include <vector>

#include <dr_gameComponent.h>
#include <dr_memory.h>
#include <dr_util_prerequisites.h>

namespace driderSDK {

class AABBCollider;

class SpiderPlayer : public GameComponent
{
public:
  SpiderPlayer(GameObject& _go);
private:

  // Inherited via GameComponent
  virtual void 
  onCreate() override;

  virtual void 
  onUpdate() override;

  virtual void 
  onRender() override;

  virtual void 
  onDestroy() override;

  virtual GameComponent* 
  cloneIn(GameObject& _go) override;

  const std::vector<std::shared_ptr<GameObject>>* m_spiders;
  AABBCollider* m_collider;
};

}
