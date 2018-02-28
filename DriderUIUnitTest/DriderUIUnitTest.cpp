
#include <dr_application.h>
#include "TestApplication.h"
#include <dr_web_renderer.h>
using namespace driderSDK;
int main()
{
  WebRenderer::start();

  TestApplication app;
  app.startApplication();
  return 0;
}

