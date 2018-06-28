#pragma once

#include <dr_gameComponent.h>
#include <dr_vector3d.h>

#include "SpiderBehavior.h"

namespace driderSDK {

class PathHolder;

class SpiderAI : public GameComponent {
 public:

  SpiderAI(GameObject& _go);

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

  virtual GameComponent*
  cloneIn(GameObject& _go) override;

  void
  setPath(PathHolder* _path);
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

  Vector3D m_velocity;
  State m_state;
  Direction m_dir;
  SpiderBehavior* m_behavior;
  PathHolder* m_path{nullptr};
  UInt32 m_pathIndex{0};
};

}
