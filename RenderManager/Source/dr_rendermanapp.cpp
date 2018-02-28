#include "dr_rendermanapp.h"
#include "dr_renderman.h"

namespace driderSDK {

void
renderManagerApp() {
  RenderManager renderAdmin;

  do {
  /*
  Render order:

  GBuffer;
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