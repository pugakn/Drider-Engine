#pragma once

#include <dr_id_object.h>

#include "dr_core_prerequisites.h"
#include "dr_gameComponent.h"
#include <dr_vector3d.h>
#include <dr_quaternion.h>

#include "dr_gameComponent.h"
#include <dr_script_engine.h>


namespace driderSDK {

class DR_CORE_EXPORT NetworkManagerComponent : public GameComponent,
                                               public IDClass<NetworkManagerComponent> {
public:

  //ScriptComponent(GameObject& _gameObj);

  NetworkManagerComponent(GameObject &gameObject);

  virtual
  ~NetworkManagerComponent();

  void
  onCreate() override;

  void
  onUpdate() override;

  void
  onDestroy() override;

  virtual UInt32
  getClassID() override;

  /**
  * Clones the component inside the given gameObject
  */
  GameComponent*
  cloneIn(GameObject& _go) override;

  void
  setExecuteFoo(void(*foo) (const WString&));

  void 
  instantiate(GameObject& object, 
              const Vector3D position, 
              const Quaternion& rotation);

  static BEGINING_REGISTER(NetworkManagerComponent, 0, asOBJ_REF | asOBJ_NOCOUNT)
    
  result = scriptEngine->m_scriptEngine->RegisterObjectMethod("NetworkManagerComponent",
                                                              "GameComponent@ opImplCast()",
                                                              asFUNCTION((refCast<NetworkManagerComponent, GameComponent>)),
                                                              asCALL_CDECL_OBJLAST);

  result = scriptEngine->m_scriptEngine->RegisterObjectMethod("GameComponent",
                                                              "NetworkManagerComponent@ opCast()",
                                                              asFUNCTION((refCast<GameComponent, NetworkManagerComponent>)),
                                                              asCALL_CDECL_OBJLAST);

  END_REGISTER

private:
  void (*executeFunctionFoo) (const WString& msg);
  
};

}