#include <iostream>

#include <dr_util_prerequisites.h>
#include <dr_memory.h>
#include "dr_server_application.h"
#include "dr_client_application.h"


int main(int argc, char* argv[]) {

  using namespace driderSDK;
  std::cout << "1.Server 2.Client" << std::endl;

  Int32 opc;

  std::cin >> opc;

  String ip;
  UInt16 port;

  std::cout << "Ip: ";

  std::cin >> ip;

  std::cout << "Port: ";

  std::cin >> port;

  std::unique_ptr<Application> app;

  if (opc == 1) {
    app = dr_make_unique<driderSDK::ServerApplication>(ip, port);
    std::cout << "Initializing as Server." << std::endl;
  }
  else {
    app = dr_make_unique<driderSDK::ClientApplication>(ip, port);
    std::cout << "Initializing as Client" << std::endl;
  }   

  return app->run();
}