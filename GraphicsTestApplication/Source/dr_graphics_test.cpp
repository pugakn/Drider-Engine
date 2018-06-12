#include "dr_graphics_application.h"

int main(int argc, char* argv[]) {

  driderSDK::Viewport viewport;
  viewport.topLeftX = 00;
  viewport.topLeftY = 0;
  viewport.width = 1024;
  viewport.height = 640;

  return driderSDK::GraphicsApplication().run(viewport);
}