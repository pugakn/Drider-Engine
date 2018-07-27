#pragma once

#include <dr_memory.h>
#include "dr_core_prerequisites.h"
#include "dr_enableObject.h"
#include "dr_name_object.h"

#include <dr_export_script.h>
#include <..\..\Script\Include\dr_script_engine.h>

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
  onStart() {}

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

  void 
  kill();

  bool 
  isKilled() const;

  virtual UInt32
  getClassID() const = 0;
  /**
  * Clones the component inside the given gameObject
  */
  virtual GameComponent*
  cloneIn(GameObject& _go) = 0;

  static BEGINING_REGISTER(GameComponent, 0, asOBJ_REF | asOBJ_NOCOUNT)

  END_REGISTER
 protected:
  GameObject& m_gameObject;
  bool m_isKilled;
};

}