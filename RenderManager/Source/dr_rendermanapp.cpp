#include "dr_rendermanapp.h"
#include "dr_renderman.h"

namespace driderSDK {

void
renderManagerApp(const SceneGraph* sceneGraph, const RenderTarget & _out, const DepthStencil & _outds) {
  RenderMan Manager;

  Manager.init();

  while (1) {
    Manager.draw(_out,_outds);
  }

  Manager.exit();
}

}