#include "TestApplication.h"
#include <dr_application.h>

using namespace driderSDK;

Int32 main() {
  Viewport viewport;
  viewport.width = 1280;
  viewport.height = 720;

  TestApplication app;
  app.run(viewport);

  return 0;
}