#include "dr_integration_app.h"

using namespace driderSDK;

int main() {
  DriderEngine DriderApp;
  Viewport viewport;
  viewport.topLeftX = viewport.topLeftY = 0;
  viewport.width = 1024;
  viewport.height = 920;

  DriderApp.run(viewport); 
  
}