#include <iostream>

#include <dr_memory.h>
#include <dr_client_lobby_app.h>


int main(int argc, char* argv[]) {

  using namespace driderSDK;

  ClientLobby app;

  return app.run({0,0, 800, 600});
}