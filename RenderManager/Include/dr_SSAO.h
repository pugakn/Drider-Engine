#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_renderpass.h"
#include <dr_render_component.h>
#include <dr_camera.h>
#include <dr_matrix4x4.h>
#include <dr_input_layout.h>
#include <dr_constant_buffer.h>

namespace driderSDK {

struct SSAOInitData : PassInitData {};

struct SSAODrawData : PassDrawData {
  RenderTarget* GbufferRT;
};

class SSAOPass : RenderPass {
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
  };

  CBuffer CB;
  
  ConstantBuffer* m_constantBuffer;
  InputLayout* m_inputLayout;
};

}