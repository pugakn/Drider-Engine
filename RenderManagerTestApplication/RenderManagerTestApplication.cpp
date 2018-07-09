#include "TestApplication.h"

using namespace driderSDK;
int main(int argc, char* argv[]) {
  RenderManApp app;

  Viewport vp;
  vp.topLeftX = 0;
  vp.topLeftY = 0;
  //vp.width = 1920;
  //vp.height = 1080;
  //vp.width = 1536;
  //vp.height = 864;
  vp.width = 1280;
  vp.height = 720;

  return app.run(vp);
}