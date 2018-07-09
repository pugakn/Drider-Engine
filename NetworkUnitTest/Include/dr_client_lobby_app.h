#pragma once

#include <dr_application.h>
#include <dr_timer.h>
#include "dr_client.h"
#include <dr_web_renderer.h>

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

  virtual void
  onResize() override;

  void 
  initCallbacks();

  // Inherited via Client
  virtual void 
  onJoinAccepted() override;

  virtual void 
  onJoinDenied() override;

  virtual void 
  onConnectionLoss() override;

  virtual void 
  onChatMsgReceived(WString&& clientName, 
                    WString&& msg) override;

  virtual void 
  onLobbiesListReceived(LobbiesList&& lobbies) override;

  void 
  drawMainMenu();
  
  void 
  drawInGame();

  void
  drawPopupErrConn();

  void
  drawPopupDisconn();

  void 
  drawLoading();
 private:

  using DrawFunc = decltype(&drawInGame);
  
  bool m_err;
  Timer m_timer;
  DrawFunc m_currentDraw;
  LobbiesList m_lobbies;
  String m_nameStr; 
  String m_msg;
  std::vector<std::pair<String, String>> m_messages;

  WebRenderer m_netLobby;
};

}