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
  RenderTarget* PositionRT;
  RenderTarget* NormalRT;
  RenderTarget* RoughnessRT;
  RenderTarget* EmissivityRT;
  RenderTarget* TransparencyRT;
  RenderTarget* ShadowRT;
  RenderTarget* SSSRT;
  RenderTarget* SSAORT;
  RenderTarget* SSAOBlurRT;

  Texture*      ColorTex;
  Texture*      PositionTex;
  Texture*      NormalTex;
  Texture*      RoughnessTex;
  Texture*      EmissivityTex;
  Texture*      TransparencyTex;
  Texture*      ShadowTex;
  Texture*      SSSTex;
  Texture*      SSAOTex;
  Texture*      SSAOBlurTex;

  DrTextureDesc ColorTexDesc;
  DrTextureDesc PositionTexDesc;
  DrTextureDesc NormalTexDesc;
  DrTextureDesc RoughnessTexDesc;
  DrTextureDesc EmissivityTexDesc;
  DrTextureDesc TransparencyTexDesc;
  DrTextureDesc ShadowTexDesc;
  DrTextureDesc SSSTexDesc;
  DrTextureDesc SSAOTexDesc;
  DrTextureDesc SSAOBlurTexDesc;
};

}