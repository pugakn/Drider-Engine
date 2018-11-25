#pragma once

#include "dr_network_prerequisites.h"
#include <dr_id_object.h>
#include <dr_gameComponent.h>

#include <dr_script_engine.h>

#include "dr_messenger.h"

namespace driderSDK {

class Vector3D;
class GameObject;

class NetworkTransformComponent : public GameComponent,
                                  public IDClass<NetworkTransformComponent> {

 public:
  NetworkTransformComponent(GameObject &gameObject_);

  virtual 
  ~NetworkTransformComponent();
  
  void
  onCreate() override;

  void
  onUpdate() override;

  void
  onRender() override;
  
  void
  onDestroy() override;

  virtual UInt32
  getClassID() override;

  GameComponent*
  cloneIn(GameObject& _go) override;

  TString 
	serialize() override;
		
	void 
	deserialize(TString& data) override;

  void
  move(const Vector3D& distance);

  BEGINING_REGISTER(NetworkTransformComponent, 0, asOBJ_REF | asOBJ_NOCOUNT)

  REGISTER_FOO(NetworkTransformComponent,
               "void move(const Vector3D& in)",
               asMETHODPR(NetworkTransformComponent,
                          move,
                          (const Vector3D&),
                          void))

  result = scriptEngine->m_scriptEngine->RegisterObjectMethod("NetworkTransformComponent",
                                                              "GameComponent@ opImplCast()",
                                                              asFUNCTION((refCast<NetworkTransformComponent, GameComponent>)),
                                                              asCALL_CDECL_OBJLAST);

  result = scriptEngine->m_scriptEngine->RegisterObjectMethod("GameComponent",
                                                              "NetworkTransformComponent@ opCast()",
                                                              asFUNCTION((refCast<GameComponent, NetworkTransformComponent>)),
                                                              asCALL_CDECL_OBJLAST);

  END_REGISTER

 private:

};

}