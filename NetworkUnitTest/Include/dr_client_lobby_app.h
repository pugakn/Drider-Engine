#pragma once

#include <dr_application.h>

#include "dr_client.h"

namespace driderSDK {

class ClientLobby : public Application, public Client
{
public:
  ClientLobby() : Application() 
  {}
private:

  // Inherited via Application
  virtual void 
  postInit() override;

  virtual void 
  postUpdate() override;

  virtual void 
  postRender() override;

  virtual void 
  postDestroy() override;


  // Inherited via Client
  virtual void 
  onJoinAccepted() override;

  virtual void 
  onJoinDenied() override;

  virtual void 
  onConnectionLoss() override;

  virtual void 
  onChatMsgReceived(const WString& clientName, 
                    const WString& msg) override;

  virtual void 
  onLobbiesListReceived(LobbiesList&& lobbies) override;
 private:
  
  LobbiesList m_lobbies;
 
};

}