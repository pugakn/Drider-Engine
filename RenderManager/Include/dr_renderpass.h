#pragma once
#include "dr_renderman_prerequisites.h"
#include <dr_string_utils.h>
#include <dr_constant_buffer.h>
#include <dr_input_layout.h>
#include <dr_shader.h>
#include <dr_render_target.h>

namespace driderSDK {

struct PassInitData {};

struct PassDrawData {};

/*
*/
class RenderPass {
 public:
  /*
  TEST::testName
  
  Description.
  */
  RenderPass();

  /*
  */
  ~RenderPass();

  /*
  */
  virtual void
  init(PassInitData* initData) = 0;

  /*
  */
  virtual void
  recompileShader();

  /*
  */
  virtual void
  draw(PassDrawData* drawData) = 0;

  /*
  virtual void
  exit() = 0;
  */

 protected:
  TString m_vsFilename;
  TString m_fsFilename;

  Shader* m_vertexShader;
  Shader* m_fragmentShader;

  ConstantBuffer* m_constantBuffer;
  InputLayout* m_inputLayout;
};

}