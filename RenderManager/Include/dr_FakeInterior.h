#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_renderpass.h"
#include <dr_graph.h>
#include <dr_render_component.h>
#include <dr_camera.h>
#include <dr_sample_state.h>
namespace driderSDK {

struct FakeInteriorInitData : PassInitData {
};

struct FakeInteriorDrawData : PassDrawData {
  std::shared_ptr<Camera> activeCam;
  RenderCommandBuffer* models;
  const RenderTarget* OutRt;
  float depthOffset;
  DepthStencil* dsOptions;
};

class FakeInteriorPass : public RenderPass {
 public:
  /*
  TEST::testName
  
  Description.
  */
   FakeInteriorPass();

  /*
  TEST::testName
  
  Description.
  */
  ~FakeInteriorPass();

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

  /*
  void
  exit();
  */

 private:
  struct CBuffer {
    Matrix4x4 World;
    Matrix4x4 WVP;
    Vector4D CameraPosition;
    Vector4D vertice[8];
  };

  CBuffer CB;

  GFXUnique<Texture> cubeMapTex;
  GFXUnique<SamplerState> m_samplerState;
};

}