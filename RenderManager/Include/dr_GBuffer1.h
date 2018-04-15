#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_renderpass.h"
#include <dr_graph.h>
#include <dr_render_component.h>
#include <dr_camera.h>

namespace driderSDK {

struct GBuffer1InitData : PassInitData {
};

struct GBuffer1DrawData : PassDrawData {
  std::shared_ptr<Camera> activeCam;
  SceneGraph::QueryResult* models;
  GFXShared<RenderTarget> OutRt;
  GFXShared<DepthStencil> dsOptions;
};

class GBuffer1Pass : public RenderPass {
 public:
  /*
  TEST::testName
  
  Description.
  */
   GBuffer1Pass();

  /*
  TEST::testName
  
  Description.
  */
  ~GBuffer1Pass();

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
    Matrix4x4 WVP;
    Matrix4x4 World;
    Matrix4x4 Bones[200];
  };

  CBuffer CB;

  GFXUnique<Texture> cubeMapTex;
};

}