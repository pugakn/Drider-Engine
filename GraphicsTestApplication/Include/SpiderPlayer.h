#pragma once

#include <vector>

#include <dr_gameComponent.h>
#include <dr_memory.h>
#include <dr_util_prerequisites.h>
#include <dr_vector3d.h>

#include "SpiderBehavior.h"

namespace driderSDK {

class AABBCollider;

class SpiderPlayer : public GameComponent
{
public:
  SpiderPlayer(GameObject& _go);
private:
  void
  movement(Vector3D dir);
  // Inherited via GameComponent
  virtual void 
  onCreate() override;

  virtual void
  onStart() override;

  virtual void 
  onUpdate() override;

  virtual void 
  onRender() override;

  virtual void 
  onDestroy() override;
  
  enum State 
  {
    Idle = SpiderBehavior::Warte,
    Walking = SpiderBehavior::WalkBack,
    Running = SpiderBehavior::RunBack
  };

  enum Direction
  {
    Back,
    Front,
    Left,
    Right,
    None = -1
  };

  void
  setState(State st, Direction dir, bool b, bool c);

  void 
  toggleNitro();

  //using Command = decltype(&running);

  virtual GameComponent* 
  cloneIn(GameObject& _go) override;

  const std::vector<std::shared_ptr<GameObject>>* m_spiders;
  AABBCollider* m_collider;
  SpiderBehavior* m_behavior;
  State m_state;
  Direction m_currDir;
  float m_velocity;
  //Command m_state;
};

}
