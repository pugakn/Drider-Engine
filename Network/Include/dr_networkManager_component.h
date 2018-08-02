#pragma once

#include <dr_id_object.h>

#include "dr_network_prerequisites.h"
#include <dr_gameComponent.h>
#include <dr_vector3d.h>
#include <dr_quaternion.h>

#include <dr_script_engine.h>

#include "dr_messenger.h"


namespace driderSDK {

class DR_NETWORK_EXPORT NetworkManagerComponent : public GameComponent,
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

  void
  onRender() override;

  virtual UInt32
  getClassID() override;

  /**
  * Clones the component inside the given gameObject
  */
  GameComponent*
  cloneIn(GameObject& _go) override;

  void 
  instantiate(GameObject& object, 
              const Vector3D position, 
              const Quaternion& rotation);

  void 
  registerObject();

  /*void 
  registerVar(TString varName,
              float value);

  void 
  registerVar(TString varName,
              Int32 value);*/

  template <typename T> void
  registerVar(const TString& varName,
              T const& value) {
    Messenger::sendFunction(m_gameObject.getName(),
                            FUNCTION_TYPE::RegisterVar,
                            varName,
                            value);
  }

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
  
};

}