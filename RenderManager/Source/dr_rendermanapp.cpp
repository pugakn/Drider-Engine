#include "dr_rendermanapp.h"
#include <dr_graphics_api.h>

namespace driderSDK {

void
renderManagerApp(RenderMan& Manager) {

  while (1) {
    Manager.draw(GraphicsAPI::getBackBufferRT(),
                 GraphicsAPI::getDepthStencil());
  }

  Manager.exit();
}

}