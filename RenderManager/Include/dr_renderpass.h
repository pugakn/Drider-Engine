#pragma once
#include "dr_renderman_prerequisites.h"
#include <dr_string_utils.h>
#include <dr_constant_buffer.h>
#include <dr_input_layout.h>
#include <dr_shader.h>
#include <dr_render_target.h>
#include <dr_gfx_memory.h>

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

  GFXUnique<Shader> m_vertexShader;
  GFXUnique<Shader>  m_fragmentShader;

  GFXUnique<ConstantBuffer> m_constantBuffer;
  GFXUnique<InputLayout> m_inputLayout;
};

}