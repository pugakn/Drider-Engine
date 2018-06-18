#pragma once

#include <dr_gameComponent.h>

#include "SpiderBehavior.h"

namespace driderSDK {

class SpiderAI : public GameComponent {
 public:

  SpiderAI(GameObject& _go);

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
 private:

  enum State 
  {
    Idle = SpiderBehavior::Warte,
    Walk = SpiderBehavior::WalkBack,
    Run = SpiderBehavior::RunBack,
    Atack = SpiderBehavior::Atack
  };

  enum Direction
  {
    Back,
    Front,
    Left,
    Right,
    None
  };

  State m_state;
  Direction m_dir;
};

}
