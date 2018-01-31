#pragma once
#include "dr_renderman_prerequisites.h"
#include <dr_render_target.h>

namespace driderSDK {

class DR_RENDERMAN_EXPORT RenderPass {
  void
  renderQueue(RenderTarget* RenderTargetInput,
              RenderTarget* RenderTargetOutput
              );
};

}