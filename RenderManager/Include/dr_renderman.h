#pragma once
#include "dr_renderman_prerequisites.h"
#include <dr_graphics_api.h>
#include <dr_device.h>
#include <dr_viewport.h>
#include <dr_camera.h>
#include <dr_graph.h>
#include <dr_render_component.h>
#include "dr_GBuffer.h"

namespace driderSDK {

/*
*/
class RenderMan {
 public:
  /*
  TEST::testName
  
  Description.
  */
   RenderMan();

   /*
   TEST::testName

   Description.
   */
   RenderMan(SceneGraph* sceneGraph);

  /*
  */
  ~RenderMan();

  /*
  */
  void
  init();

  /*
  */
  void
  draw();

  /*
  */
  void
  exit();

 protected:
  SceneGraph* m_sceneGraph;
  std::vector<std::pair<Matrix4x4, RenderMesh>> queryRequest;
  Camera Sauron;

  GBufferPass m_GBufferPass;
  GBufferInitData m_GBufferInitData;
  GBufferDrawData m_GBufferDrawData;

  RenderTarget* ColorRT;
  Texture*      ColorTex;
  RenderTarget* PositionRT;
  Texture*      PositionTex;
  RenderTarget* NormalRT;
  Texture*      NormalTex;
  RenderTarget* RoughnessRT;
  Texture*      RoughnessTex;
  RenderTarget* EmissivityRT;
  Texture*      EmissivityTex;
  RenderTarget* TransparencyRT;
  Texture*      TransparencyTex;
  RenderTarget* ShadowRT;
  Texture*      ShadowTex;
  RenderTarget* SSSRT;
  Texture*      SSSTex;
  RenderTarget* SSAORT;
  Texture*      SSAOTex;
  RenderTarget* SSAOBlurRT;
  Texture*      SSAOBlurTex;
};

}