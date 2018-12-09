#include <dr_application.h>
#include "TestApplication.h"
#include <dr_web_renderer.h>

using namespace driderSDK;

driderSDK::Int32
main() {
  Viewport viewport;
  viewport.width = 1280;
  viewport.height = 720;
  WebRenderer::start();

  TestApplication app;
  app.run(viewport);
  return 0;
}

