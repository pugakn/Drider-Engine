#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_renderpass.h"
#include <dr_graph.h>
#include <dr_texture_core.h>
#include <dr_render_component.h>
#include <dr_camera.h>
#include <dr_sample_state.h>
namespace driderSDK {

struct GBufferInitData : PassInitData {
};

struct GBufferDrawData : PassDrawData {
  std::shared_ptr<Camera> activeCam;
  RenderCommandBuffer* models;
  RenderTarget* OutRt;
  DepthStencil* dsOptions;
  Model* skysphere;
  TextureCore* cubeMapTex;
};

class GBufferPass : public RenderPass {
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

  /*
  void
  exit();
  */

 private:
  struct CBuffer {
    Matrix4x4 World;
    Matrix4x4 WorldView;
    Matrix4x4 WVP;
    Matrix4x4 Bones[200];
    Vector4D CameraInfo; //x: aspect ratio; y: fov; z: near plane; w: far plane
  };

  CBuffer CB;

  //GFXUnique<Model> skysphere;
  //GFXUnique<Texture> cubeMapTex;
  GFXUnique<SamplerState> m_samplerState;
};

}