#pragma once

#include <dr_gameComponent.h>

#include <dr_memory.h>

#include "dr_util_prerequisites.h"

namespace driderSDK {

class SpiderBehavior : public GameComponent
{
 public:

  using WeakModel = std::weak_ptr<class Model>;
      
  enum Animations 
  {
    Atack = 0,
    Die = 1,
    Die2 = 2,
    Fall,
    Jump,
    Normal,
    RunBack,
    RunFront,
    RunLeft,
    RunRight,
    WalkBack,
    WalkFront,
    WalkLeft,
    WalkRight,
    Warte,
    Count 
  };

  SpiderBehavior(GameObject& _gameObject);

  virtual void 
  onCreate() override;
  virtual void 
  onUpdate() override;
  virtual void 
  onRender() override;
  virtual void 
  onDestroy() override;
  virtual UInt32
  getClassID();

  virtual GameComponent*
  cloneIn(GameObject & _go) override;

  void 
  setAnimation(Animations anim, bool blend, bool copyElapsed);
 private:
 private:
  WeakModel m_model;

  // Inherited via GameComponent
  virtual void serialize(File & file) override;
  virtual void deserialize(TString & data) override;
};

}
