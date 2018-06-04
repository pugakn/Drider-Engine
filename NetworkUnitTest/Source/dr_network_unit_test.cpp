#include <iostream>

#include <dr_memory.h>
#include <dr_client_lobby_app.h>
#include <dr_web_renderer.h> //Error when included before dr_client_lobby_app.h

int main(int argc, char* argv[]) {

  using namespace driderSDK;
  Viewport viewport;
  viewport.width = 1280;
  viewport.height = 720;
  WebRenderer::start();
  ClientLobby app;

  return app.run(viewport);
}