#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_vector4d.h"
#include "dr_renderpass.h"
#include <dr_sample_state.h>

namespace driderSDK {

struct HorBlurInitData : PassInitData {};

struct HorBlurDrawData : PassDrawData {
  float                   viewportDimensionX;
  float                   viewportDimensionY;
  GFXShared<RenderTarget> InRt;
  GFXShared<RenderTarget> OutRt;
  GFXShared<DepthStencil> dsOptions;
};

class HorBlurPass : public RenderPass {
 public:
  /*
  TEST::testName
  
  Description.
  */
  HorBlurPass();

  /*
  TEST::testName
  
  Description.
  */
  ~HorBlurPass();

  /*
  TEST::testName

  Description.
  */
  void
  init(PassInitData* initData);

  /*
  */
  void
  draw(PassDrawData* drawData);

 private:
  struct CBuffer {
    Vector4D fViewportDimensions;
  };

  CBuffer CB;

  GFXUnique<SamplerState> m_samplerState;
};

}