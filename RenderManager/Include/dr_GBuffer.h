#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_renderpass.h"
#include <dr_render_component.h>
#include <dr_camera.h>
#include <dr_matrix4x4.h>
#include <dr_input_layout.h>
#include <dr_constant_buffer.h>

namespace driderSDK {

struct GBufferInitData : PassInitData
{
  
};

struct GBufferDrawData : PassDrawData
{
  Camera* activeCam;
  std::vector<std::pair<Matrix4x4, RenderMesh>>* models;
};

class GBufferPass : RenderPass {
 public:
  /*
  TEST::testName
  
  Description.
  */
  GBufferPass();

  /*
  TEST::testName
  
  Description.
  */
  ~GBufferPass();

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
    Matrix4x4 WVP;
    Matrix4x4 World;
  };
  
  ConstantBuffer* m_constantBuffer;
  InputLayout* m_inputLayout;
};

}