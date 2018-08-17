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

  void
  registerFloat(const TString& varName,
                float value);

  void
  registerInt(const TString& varName,
              Int32 value);

  void
  registerString(const TString& varName,
                 const TString& value);

  void
  instantiate(OBJ_TYPE::E objType,
              const Vector3D& position,
              const Vector3D& direction);

  void
  move(const Vector3D& distance);

  static BEGINING_REGISTER(NetworkManagerComponent, 0, asOBJ_REF | asOBJ_NOCOUNT)

  result = REGISTER_ENUM(ObjectType)
  result = REGISTER_ENUM_VALUE(ObjectType, OBJ_TYPE, kPlayer)
  result = REGISTER_ENUM_VALUE(ObjectType, OBJ_TYPE, kProjectile)

  result = REGISTER_FOO(NetworkManagerComponent,
                        "void instantiate(ObjectType, const Vector3D& in, const Vector3D& in)",
                        asMETHODPR(NetworkManagerComponent, 
                                   instantiate, 
                                   (OBJ_TYPE::E, const Vector3D&, const Vector3D&),
                                   void));

  result = REGISTER_FOO(NetworkManagerComponent,
                        "void registerFloat(const TString& in, float)",
                        asMETHODPR(NetworkManagerComponent,
                                   registerFloat,
                                   (const TString&, float),
                                   void));

  result = REGISTER_FOO(NetworkManagerComponent,
                        "void move(const Vector3D& in)",
                        asMETHODPR(NetworkManagerComponent,
                                   move,
                                   (const Vector3D&),
                                   void));

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