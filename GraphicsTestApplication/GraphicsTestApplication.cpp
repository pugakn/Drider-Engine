#include "dr_graphics_application.h"
#include "TestApplication.h"

using namespace driderSDK;
int main(int argc, char* argv[]) {

  if(argc > 0) {
    TestApplication scriptApp;
    return scriptApp.run();
  }

  
  GraphicsApplication gfxApp;
  return gfxApp.run();
}