#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_renderpass.h"
#include <dr_graph.h>
#include <dr_matrix4x4.h>
#include <dr_sample_state.h>

namespace driderSDK {

struct SSAOInitData : PassInitData {
  SizeT RTWidth;
  SizeT RTHeight;
};

struct SSAODrawData : PassDrawData {
  std::shared_ptr<Camera> activeCam;
  GFXShared<RenderTarget> InRt;
  GFXShared<RenderTarget> OutRt;
  GFXShared<DepthStencil> dsOptions;
  float SampleRadio;
  float Intensity;
  float Scale;
  float Bias;
};

class SSAOPass : public RenderPass {
 public:
  /*
  TEST::testName
  
  Description.
  */
  SSAOPass();

  /*
  TEST::testName
  
  Description.
  */
  ~SSAOPass();

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
     Vector4D SSAO_Options; //X: SampleRadio Y: Intensity Z: Scale X: Bias
   };

   CBuffer CB;

   GFXUnique<SamplerState> m_samplerState;
};

}