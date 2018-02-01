#pragma once

#include <dr_memory.h>
#include "dr_core_prerequisites.h"

namespace driderSDK {

class GameObject;

class GameComponent : public std::enable_shared_from_this<GameComponent>
{
 public:
  GameComponent(GameObject& gameObject_);

  GameObject& 
  getGameObject();

  virtual void
  onCreate() = 0;

  virtual void
  onUpdate() = 0;

  /*********
  /* TEMP */
  virtual void 
  onRender() = 0;
  /********/

  virtual void
  onDestroy() = 0;
 private:
  GameObject& m_gameObject;
};

}