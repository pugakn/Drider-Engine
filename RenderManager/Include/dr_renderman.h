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
  std::vector<std::pair<Matrix4x4, RenderMesh>> queryRequest;

  Camera Sauron;

  GBufferPass m_GBufferPass;
  GBufferInitData m_GBufferInitData;
  GBufferDrawData m_GBufferDrawData;

  RenderTarget* m_RTs;

  Texture*      ColorTex;
  Texture*      PositionTex;
  Texture*      NormalTex;

  DrTextureDesc ColorTexDesc;
  DrTextureDesc PositionTexDesc;
  DrTextureDesc NormalTexDesc;
};

}