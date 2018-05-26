#pragma once

#include <dr_memory.h>
#include "dr_core_prerequisites.h"
#include "dr_enableObject.h"
#include "dr_name_object.h"

namespace driderSDK {

class GameObject;

class DR_CORE_EXPORT GameComponent : public EnableObject,
                                     public NameObject
{
 public:
  GameComponent(GameObject& gameObject_, const TString& _name);

  virtual ~GameComponent();

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

  virtual void
  setName(const TString& name);

  /**
  * Clones the component inside the given gameObject
  */
  virtual void
  cloneIn(GameObject& _go) = 0;
 protected:
  GameObject& m_gameObject;
};

}