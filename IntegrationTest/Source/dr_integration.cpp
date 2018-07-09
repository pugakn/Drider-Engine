#include "dr_integration_app.h"
#include <dr_web_renderer.h>

using namespace driderSDK;

int main() {
  WebRenderer::start();
  DriderEngine DriderApp;
  Viewport viewport;
  viewport.topLeftX = viewport.topLeftY = 0;
  viewport.width = 1024;
  viewport.height = 920;

  DriderApp.run(viewport); 
  
}