#pragma once
#include "dr_renderman_prerequisites.h"
#include <dr_graphics_api.h>
#include <dr_device.h>
#include <dr_viewport.h>
#include <dr_camera.h>
#include <dr_graph.h>
#include <dr_render_component.h>
#include "dr_GBuffer.h"
#include "dr_SSAO.h"

namespace driderSDK {

/*
*/
class DR_RENDERMAN_EXPORT RenderMan {
 public:
  /*
  TEST::testName
  
  Description.
  */
   RenderMan();

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
  SceneGraph::QueryResult* queryRequest;

  Viewport m_viewport;
  std::shared_ptr<Camera> Sauron;

  GBufferPass m_GBufferPass;
  GBufferInitData m_GBufferInitData;
  GBufferDrawData m_GBufferDrawData;

  SSAOPass m_SSAOPass;
  SSAOInitData m_SSAOInitData;
  SSAODrawData m_SSAODrawData;

  RenderTarget* m_RTs;

  Texture*      ColorTex;
  Texture*      PositionTex;
  Texture*      NormalTex;

  DrTextureDesc ColorTexDesc;
  DrTextureDesc PositionTexDesc;
  DrTextureDesc NormalTexDesc;
};

}