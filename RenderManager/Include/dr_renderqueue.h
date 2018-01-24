#pragma once
#include "dr_renderman_prerequisites.h"
#include <dr_render_target.h>

namespace driderSDK {

template<typename T, typename... Args>
DR_RENDERMAN_EXPORT void
renderQueue(RenderTarget* RenderTargetInput,
            RenderTarget* RenderTargetOutput,
            Args args);

}