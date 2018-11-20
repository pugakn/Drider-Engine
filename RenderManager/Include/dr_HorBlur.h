#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_vector4d.h"
#include "dr_renderpass.h"
#include <dr_sample_state.h>

namespace driderSDK {

struct HorBlurInitData : PassInitData {
};

struct HorBlurDrawData : PassDrawData {
  Texture* InTexture;
  RenderTarget* OutRt;
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

  SizeT m_RTWidth;
  SizeT m_RTHeight;
  SizeT m_ComputeWidthDivisions;
  SizeT m_ComputeHeightDivisions;
  SizeT m_ComputeWidthBlocks;
  SizeT m_ComputeHeightBlocks;
  SizeT m_ComputeTotalBlocks;

  CBuffer CB;

  GFXUnique<SamplerState> m_samplerState;
};

}