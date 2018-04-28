#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_renderpass.h"
#include <dr_graph.h>
#include <dr_matrix4x4.h>
#include <dr_sample_state.h>

namespace driderSDK {

struct SSAOInitData : PassInitData {};

struct SSAODrawData : PassDrawData {
  std::shared_ptr<Camera> activeCam;
  GFXShared<RenderTarget> InRt;
  GFXShared<RenderTarget> OutRt;
  GFXShared<DepthStencil> dsOptions;
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
     Matrix4x4 View;
     Matrix4x4 ViewInverse;
     Matrix4x4 Projection;
     Matrix4x4 ProjectionInverse;
     Matrix4x4 VP;
     Matrix4x4 VPInverse;
   };

   CBuffer CB;

   GFXUnique<SamplerState> m_samplerState;
};

}