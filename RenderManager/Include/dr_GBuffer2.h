#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_renderpass.h"
#include <dr_graph.h>
#include <dr_render_component.h>
#include <dr_camera.h>
#include <dr_matrix4x4.h>
#include <dr_input_layout.h>
#include <dr_constant_buffer.h>

namespace driderSDK {

struct GBuffer2InitData : PassInitData {};

struct GBuffer2DrawData : PassDrawData {
  std::shared_ptr<Camera> activeCam;
  SceneGraph::QueryResult* models;
  RenderTarget* InRt;
  RenderTarget* OutRt;
  DepthStencil* dsOptions;
};

class GBuffer2Pass : RenderPass {
 public:
  /*
  TEST::testName
  
  Description.
  */
  GBuffer2Pass();

  /*
  TEST::testName
  
  Description.
  */
  ~GBuffer2Pass();

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
    Matrix4x4 Bones[200];
  };

  CBuffer CB;

  InputLayout* m_inputLayout;
  ConstantBuffer* m_constantBuffer;
};

}