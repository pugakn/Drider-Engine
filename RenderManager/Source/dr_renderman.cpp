#include "dr_renderman.h"
#include <dr_graphics_driver.h>
#include <dr_device.h>
#include <dr_resource_manager.h>
#include <dr_image_info.h>
#include <dr_camera_manager.h>
#include <dr_depth_stencil.h>

namespace driderSDK {

RenderMan::RenderMan() {
}

RenderMan::~RenderMan() {
}

void
RenderMan::init() {
  Device& dc = GraphicsAPI::getDevice();

  UInt32 screenWidth = 1080;
  UInt32 screenHeight = 720;

  GBufferTexDesc.width = screenWidth;
  GBufferTexDesc.height = screenHeight;
  GBufferTexDesc.pitch = screenWidth * 4;
  GBufferTexDesc.dimension = DR_DIMENSION::k2D;
  GBufferTexDesc.Format = DR_FORMAT::kR16G16B16A16_FLOAT;
  GBufferTexDesc.mipLevels = 0;
  GBufferTexDesc.CPUAccessFlags = DR_CPU_ACCESS_FLAG::drRead | DR_CPU_ACCESS_FLAG::drWrite;
  GBufferTexDesc.genMipMaps = true;
  GBufferTexDesc.bindFlags = DR_BIND_FLAGS::SHADER_RESOURCE |
                             DR_BIND_FLAGS::RENDER_TARGET;

  m_RTGBuffer1 = dr_gfx_shared(dc.createRenderTarget(GBufferTexDesc, 6));
  m_RTGBuffer2 = dr_gfx_shared(dc.createRenderTarget(GBufferTexDesc, 1));

  DrDepthStencilDesc depthTextureDesc;
  depthTextureDesc.bindFlags = DR_BIND_FLAGS::DEPTH_STENCIL | DR_BIND_FLAGS::SHADER_RESOURCE;
  depthTextureDesc.width = screenWidth;
  depthTextureDesc.height = screenHeight;
  depthTextureDesc.Format = DR_FORMAT::kD24_UNORM_S8_UINT;

  m_GBuffer1DSoptions = dr_gfx_shared(dc.createDepthStencil(depthTextureDesc));
  m_SSAODSoptions = dr_gfx_shared(dc.createDepthStencil(depthTextureDesc));

  ResourceManager::loadResource(_T("ScreenAlignedQuad.3ds"));
  ImageInfo cubeMap;
  cubeMap.textureDimension = DR_DIMENSION::kCUBE_MAP; 
  ResourceManager::loadResource(_T("grace-new.hdr"), &cubeMap);

  m_GBuffer1Pass.init(&m_GBuffer1InitData);
  m_SSAOPass.init(&m_SSAOInitData);
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

  m_SSAODrawData.InRt = m_RTGBuffer1;
  m_SSAODrawData.OutRt = m_RTGBuffer2;
  m_SSAODrawData.dsOptions = m_SSAODSoptions;
  m_SSAOPass.draw(&m_SSAODrawData);

  m_PostProcessingDrawData.CameraPosition = mainCam->getPosition();
  m_PostProcessingDrawData.Gbuffer1RT = m_RTGBuffer1;
  m_PostProcessingDrawData.Gbuffer2RT = m_RTGBuffer2;
  m_PostProcessingDrawData.Lights = &lights[0];
  m_PostProcessingDrawData.ActiveLights = 128;
  m_PostProcessingPass.draw(&m_PostProcessingDrawData);

  /*
  GBuffer1:
  SSAO:
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

void
RenderMan::recompile() {
  m_GBuffer1Pass.recompileShader();
  m_SSAOPass.recompileShader();
  m_PostProcessingPass.recompileShader();
}

}