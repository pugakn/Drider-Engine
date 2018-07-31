#include "dr_rendermanapp.h"
#include <dr_graphics_api.h>
#include <dr_graphics_driver.h>

namespace driderSDK {

void
renderManagerApp(const bool* renderFlag) {
  while (*renderFlag) {
    GraphicsDriver::API().clear();
    RenderManager::instance().draw(GraphicsAPI::getBackBufferRT(),
                                   GraphicsAPI::getDepthStencil());
    GraphicsDriver::API().swapBuffers();
  }
}

}