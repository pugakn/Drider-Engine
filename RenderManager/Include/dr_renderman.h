#pragma once
#include "dr_renderman_prerequisites.h"
#include <dr_graphics_api.h>
#include <dr_device.h>
#include <dr_camera_manager.h>
#include <dr_graph.h>
#include <dr_render_component.h>
#include <dr_depth_stencil.h>
#include <dr_depth_stencil_state.h>
#include <dr_gfx_memory.h>
#include "dr_GBuffer1.h"
#include "dr_SSAO.h"
#include "dr_PostProcessing.h"
#include "dr_light.h"

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

  /*
  */
  void
  recompile();

  std::array<Light, 128>* lights;
 protected:

  GBuffer1Pass m_GBuffer1Pass;
  GBuffer1InitData m_GBuffer1InitData;
  GBuffer1DrawData m_GBuffer1DrawData;
  GFXShared<DepthStencil> m_GBuffer1DSoptions;

  SSAOPass m_SSAOPass;
  SSAOInitData m_SSAOInitData;
  SSAODrawData m_SSAODrawData;
  GFXShared<DepthStencil> m_SSAODSoptions;

  PostProcessingPass m_PostProcessingPass;
  PostProcessingInitData m_PostProcessingInitData;
  PostProcessingDrawData m_PostProcessingDrawData;
  GFXShared<DepthStencil> m_PostProcessingDSoptions;

  //0: Albedo; 1: Position; 2: Normal; 3: Emissive; 4: Metallic; 5: Roughness;
  GFXShared<RenderTarget> m_RTGBuffer1;
  //0: SSAO
  GFXShared<RenderTarget> m_RTGBuffer2;

  DrTextureDesc GBufferTexDesc;
};

}