#include "dr_graphics_application.h"
#include "TestApplication.h"

using namespace driderSDK;
int main(int argc, char* argv[]) {

  argc = 0;
  TestApplication scriptApp;
  Viewport nuevo;
  nuevo.topLeftX = 0;
  nuevo.topLeftY = 0;
  nuevo.width = 700;
  nuevo.height = 650;
  nuevo.minDepth = 1;
  nuevo.maxDepth = 1000;

  if(argc > 0) {

    return scriptApp.run(nuevo);
  }

  
  GraphicsApplication gfxApp;
  return gfxApp.run(nuevo);
}