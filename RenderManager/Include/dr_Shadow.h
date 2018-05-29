#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_renderpass.h"
#include <dr_graph.h>
#include <dr_render_component.h>
#include <dr_camera.h>
#include <dr_sample_state.h>

namespace driderSDK {

struct ShadowInitData : PassInitData {
};

struct ShadowDrawData : PassDrawData {
  std::shared_ptr<Camera>  shadowCam;
  SceneGraph::QueryResult* models;
  GFXShared<RenderTarget>  OutRt;
  GFXShared<DepthStencil>  dsOptions;
};

class ShadowPass : public RenderPass {
 public:
  /*
  TEST::testName
  
  Description.
  */
  ShadowPass();

  /*
  TEST::testName
  
  Description.
  */
  ~ShadowPass();

  /*
  TEST::testName

  Description.
  */
  void
  init(PassInitData* initData);

  /*
  */
  void
  recompileShader();

  /*
  */
  void
  draw(PassDrawData* drawData);

  /*
  */
  void
  merge(std::array<GFXShared<RenderTarget>, 4> m_RTShadowDummy,
        GFXShared<DepthStencil> dsOptions,
        GFXShared<RenderTarget> OutRt);

 private:
  struct CBuffer {
    Matrix4x4 WVP;
    Matrix4x4 Bones[200];
  };

  CBuffer CB;

  GFXUnique<SamplerState> m_samplerState;

  /////////////////////////////////////////////////////////////////////////////
  GFXUnique<Shader> m_ShaderVMerge;
  GFXUnique<Shader> m_ShaderFMerge;
};

}