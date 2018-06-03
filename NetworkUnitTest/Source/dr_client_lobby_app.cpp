#include "dr_client_lobby_app.h"


#include <dr_network_manager.h>
#include <dr_input_manager.h>
#include <dr_graphics_driver.h>
#include <dr_graphics_api.h>

namespace driderSDK {

void 
ClientLobby::postInit() {

  NetworkManager::startUp();
  InputManager::startUp();
  //GraphicsDriver::startUp(DR_GRAPHICS_API::D3D11);

  Client::init();

}

void 
ClientLobby::postUpdate() {
  
  Client::update();

}

void 
ClientLobby::postRender() {

}

void 
ClientLobby::postDestroy() {
  
  Client::quit();

  InputManager::shutDown();
  GraphicsDriver::shutDown();
  NetworkManager::shutDown();
}

void 
ClientLobby::onJoinAccepted() {}

void 
ClientLobby::onJoinDenied() {}

void 
ClientLobby::onConnectionLoss() {}

void 
ClientLobby::onChatMsgReceived(const WString& clientName, const WString& msg) {}

void 
ClientLobby::onLobbiesListReceived(LobbiesList && lobbies) {}

}