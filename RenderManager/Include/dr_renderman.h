#pragma once
#include "dr_renderman_prerequisites.h"
#include <dr_render_target.h>
#include <dr_camera.h>

namespace driderSDK {

/*
*/
class DR_RENDERMAN_EXPORT RenderManager {
 public:
  /*
  */
   RenderManager();

  /*
  */
  ~RenderManager();

  /*
  */
  Int32
  createRenderTarget(Int32 width,
                     Int32 height,
                     Int32 channels);

  /*
  */
  void
  setRenderEye(Camera* actualCamera);

  /*
  */
  void
  renderPass(std::vector<Texture*> TextureInput,
             std::vector<RenderTarget*> RenderTargetOutput,
             void* args);

 private:
  std::vector<RenderTarget> allRenderTargets;
};

}