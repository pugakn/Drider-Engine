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
  std::shared_ptr<Camera>  activeCam;
  Int32                    shadowIndex;
  SceneGraph::QueryResult* models;
  GFXShared<RenderTarget>  OutRt;
  GFXShared<DepthStencil>  dsOptions;
  Vector3D DirectionalLight;
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
  draw(PassDrawData* drawData);

 private:
  struct CBuffer {
    Matrix4x4 WVP;
    Matrix4x4 World;
    Matrix4x4 Bones[200];
    Int32     ShadowIndex[4];
    Vector4D  extraInfo;
    Matrix4x4 WV;
  };

  CBuffer CB;

  
};

}