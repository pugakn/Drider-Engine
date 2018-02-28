#include "dr_rendermanapp.h"

#include "dr_renderman.h"

namespace driderSDK {

void
renderManagerApp(SceneGraph* sceneGraph) {
  RenderMan Manager;

  Manager.init(0);

  while (1) {
    Manager.draw(0);
  }

  Manager.exit(0);
}

}