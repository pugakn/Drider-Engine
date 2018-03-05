#pragma once

#include <dr_memory.h>
#include "dr_enableObject.h"
#include "dr_core_prerequisites.h"

namespace driderSDK {

class GameObject;

class DR_CORE_EXPORT GameComponent : public EnableObject                  
{
 public:
  GameComponent(GameObject& gameObject_);

  virtual ~GameComponent(){}

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

  /**
  * Clones the component inside the given gameObject
  */
  virtual void
  cloneIn(GameObject& _go) = 0;
 protected:
  GameObject& m_gameObject;
};

}