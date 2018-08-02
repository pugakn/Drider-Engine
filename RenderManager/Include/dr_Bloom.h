#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_renderpass.h"

#include <dr_texture_core.h>
#include <dr_texture.h>
#include <dr_sample_state.h>
#include <dr_camera.h>
#include <dr_structure_buffer.h>

namespace driderSDK {

struct BloomInitData : PassInitData {
  SizeT RTWidth;
  SizeT RTHeight;
};

struct BloomDrawData : PassDrawData {
  Vector3D BloomThreshold;
  float LuminiscenceDelta;
  Texture* ColorTexture;
  Texture* OutTex;
};

class BloomPass : public RenderPass {
 public:
  /*
  TEST::testName
  
  Description.
  */
  BloomPass();

  /*
  TEST::testName
  
  Description.
  */
  ~BloomPass();

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
    Vector4D BloomThresholdLuminiscenceDelta;
  };

  CBuffer CB;

  SizeT m_RTWidth;
  SizeT m_RTHeight;
  SizeT m_ComputeWidthDivisions;
  SizeT m_ComputeHeightDivisions;
  SizeT m_ComputeWidthBlocks;
  SizeT m_ComputeHeightBlocks;
  SizeT m_ComputeTotalBlocks;

  GFXUnique<SamplerState> m_samplerState;
};

}