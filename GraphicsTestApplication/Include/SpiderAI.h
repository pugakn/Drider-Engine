#pragma once

#include <dr_gameComponent.h>
#include <dr_vector3d.h>

#include "SpiderBehavior.h"
#include <dr_timer.h>

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

  virtual UInt32
  getClassID() const override;

  void
  setPath(PathHolder* _path);
 private:

  enum State 
  {
    Idle = SpiderBehavior::Warte,
    Walk = SpiderBehavior::WalkBack,
    Run = SpiderBehavior::RunBack,
    Atack = SpiderBehavior::Atack,
    StopedRun
  };

  enum Direction
  {
    Back,
    Front,
    Left,
    Right,
    None
  };

  Timer m_inactive;
  Vector3D m_velocity;
  Vector3D m_direction;
  float m_velDecrese;
  float m_stopTime;
  State m_state;
  Direction m_dir;
  SpiderBehavior* m_behavior;
  PathHolder* m_path{nullptr};
  UInt32 m_pathIndex{0};
};

}
