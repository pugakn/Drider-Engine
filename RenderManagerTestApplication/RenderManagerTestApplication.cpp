#include "TestApplication.h"

using namespace driderSDK;
int main(int argc, char* argv[]) {
  RenderManApp app;

  Viewport vp;
  vp.topLeftX = 0;
  vp.topLeftY = 0;

  //4K
  //vp.width = 4096;
  //vp.height = 2304;

  //UHD
  //vp.width = 3840;
  //vp.height = 2160;

  //2K
  //vp.width = 2048;
  //vp.height = 1152;

  //1080p o FHD
  //vp.width = 1920;
  //vp.height = 1080;

  //720p o HD
  //vp.width = 1280;
  //vp.height = 720;

  //////////21:9//////////

  //720p o HD
  vp.width = 1680;
  vp.height = 720;

  return app.run(vp);
}