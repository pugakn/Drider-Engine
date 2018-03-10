#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_renderpass.h"
#include <dr_render_component.h>
#include <dr_camera.h>
#include <dr_matrix4x4.h>
#include <dr_input_layout.h>
#include <dr_constant_buffer.h>
#include <dr_sample_state.h>

namespace driderSDK {

struct PostProcessingInitData : PassInitData {};

struct PostProcessingDrawData : PassDrawData {
  RenderTarget* Gbuffer1RT;
  RenderTarget* Gbuffer2RT;
};

class PostProcessingPass : RenderPass {
 public:
  /*
  TEST::testName
  
  Description.
  */
  PostProcessingPass();

  /*
  TEST::testName
  
  Description.
  */
  ~PostProcessingPass();

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
  InputLayout* m_inputLayout;

  SamplerState* m_samplerState;
};

}