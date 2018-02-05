#include "dr_rendermanapp.h"
#include "dr_renderpass.h"

namespace driderSDK {

void
renderManagerApp() {
  RenderPass renderAdmin;

  do {
  /*
  Render order:
  Normal + Depth Pre-Pass
  Shadow Maps
  Screen Space Ambient Occlusion
  Light Pre-Pass
  Forward-Rendering of Opaque Objects
  Transparent Objects
  Light Bloom
  Anti-Aliasing
  Color Correction
  User Interface

  Depth of Field
  */
  } while(true);
}

}