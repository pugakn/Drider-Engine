#include "dr_rendermanapp.h"
#include "dr_renderman.h"

namespace driderSDK {

void
renderManagerApp() {
  RenderMan Manager;

  Manager.init();

  while (1) {
    //Manager.draw();
  }

  Manager.exit();
}

}