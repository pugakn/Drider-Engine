#include "dr_renderman.h"
#include <dr_swap_chain.h>
#include <dr_graphics_driver.h>
#include <dr_d3d_depth_stencil.h>
#include <dr_resource_manager.h>
#include <dr_model.h>

namespace driderSDK {

RenderMan::RenderMan() {
}

RenderMan::~RenderMan() {
}

void
RenderMan::init() {
  Device& dc = GraphicsAPI::getDevice();

  UInt32 screenWidth = 1280;
  UInt32 screenHeight = 720;

  DrTextureDesc PositionDesc;
  PositionDesc.width = screenWidth;
  PositionDesc.height = screenHeight;
  PositionDesc.pitch = screenWidth * 4;
  PositionDesc.dimension = DR_DIMENSION::k2D;
  PositionDesc.Format = DR_FORMAT::kR8G8B8A8_UNORM;
  PositionDesc.mipLevels = 0;
  PositionDesc.CPUAccessFlags = DR_CPU_ACCESS_FLAG::drRead | DR_CPU_ACCESS_FLAG::drWrite;
  PositionDesc.genMipMaps = true;
  PositionDesc.bindFlags = DR_BIND_FLAGS::SHADER_RESOURCE |
                           DR_BIND_FLAGS::RENDER_TARGET;

  m_RTGBuffer1 = dc.createRenderTarget(PositionDesc, 4);
  m_RTGBuffer2 = dc.createRenderTarget(PositionDesc, 3);

  DrDepthStencilDesc depthTextureDesc;
  depthTextureDesc.bindFlags = DR_BIND_FLAGS::DEPTH_STENCIL | DR_BIND_FLAGS::SHADER_RESOURCE;
  depthTextureDesc.width = screenWidth;
  depthTextureDesc.height = screenHeight;
  depthTextureDesc.Format = DR_FORMAT::kD24_UNORM_S8_UINT;

  m_GBuffer1DSoptions = dc.createDepthStencil(depthTextureDesc);
  m_GBuffer2DSoptions = dc.createDepthStencil(depthTextureDesc);

  ResourceManager::loadResource(_T("ScreenAlignedQuad.3ds"));

  m_GBuffer1Pass.init(&m_GBuffer1InitData);
  m_GBuffer2Pass.init(&m_GBuffer2InitData);
  m_PostProcessingPass.init(&m_PostProcessingInitData);
}

void
RenderMan::draw() {
  GraphicsDriver::API().clear();

  auto mainCam = CameraManager::getActiveCamera();
  auto queryRequest = SceneGraph::query(*mainCam,
                                 QUERY_ORDER::kFrontToBack,
                                 QUERY_PROPERTY::kOpaque | 
                                 QUERY_PROPERTY::kDynamic | 
                                 QUERY_PROPERTY::kStatic);

  m_GBuffer1DrawData.activeCam = mainCam;
  m_GBuffer1DrawData.models = &queryRequest;
  m_GBuffer1DrawData.OutRt = m_RTGBuffer1;
  m_GBuffer1DrawData.dsOptions = m_GBuffer1DSoptions;
  m_GBuffer1Pass.draw(&m_GBuffer1DrawData);

  m_GBuffer2DrawData.activeCam = mainCam;
  m_GBuffer2DrawData.models = &queryRequest;
  m_GBuffer2DrawData.InRt = m_RTGBuffer1;
  m_GBuffer2DrawData.OutRt = m_RTGBuffer2;
  m_GBuffer2DrawData.dsOptions = m_GBuffer2DSoptions;
  m_GBuffer2Pass.draw(&m_GBuffer2DrawData);

  m_PostProcessingDrawData.Gbuffer1RT = m_RTGBuffer1;
  m_PostProcessingDrawData.Gbuffer2RT = m_RTGBuffer2;
  m_PostProcessingPass.draw(&m_PostProcessingDrawData);

  /*
  GBuffer1: 
  GBuffer2: 
  ShadowCascades: 
  Diffuse Acumulation: Fog
  Specular Acumulation: SS Reflection
  Lights y ZSkips: 
  Opacity: Blends
  
  */

  GraphicsDriver::API().swapBuffers();
}

void
RenderMan::exit() {
}

}