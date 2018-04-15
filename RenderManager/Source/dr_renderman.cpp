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

  UInt32 screenWidth = 1280;
  UInt32 screenHeight = 720;

  for (SizeT i = 0; i < 4; ++i) {
    m_vecShadowCamera[i] = std::make_shared<Camera>();
  }
  m_szActiveShadowCameras = 4;
  //m_vec3DirectionalLight = Vector3D(0.0f, -1.0f, 0.0f).normalize();
  //m_vec3DirectionalLight = Vector3D(0.0f, -1000.0f, 1.0f).normalize();
  m_vec3DirectionalLight = Vector3D(-1.0f, -1.0f, 1.0f).normalize();
  m_fDepth = 10000.0f; 
  m_bFitToFrustrum = true;

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

  m_RTGBuffer1  = dr_gfx_shared(dc.createRenderTarget(GBufferTexDesc, 6));
  m_RTSSAO      = dr_gfx_shared(dc.createRenderTarget(GBufferTexDesc, 1));
  //GBufferTexDesc.width  = static_cast<Int32>(1024);
  //GBufferTexDesc.height = static_cast<Int32>(1024);
  //GBufferTexDesc.pitch = static_cast<Int32>(1024) * 4;
  //GBufferTexDesc.Format = DR_FORMAT::kR32G32B32A32_FLOAT;
  m_RTShadow    = dr_gfx_shared(dc.createRenderTarget(GBufferTexDesc, 1));

  DrDepthStencilDesc depthTextureDesc;
  depthTextureDesc.bindFlags = DR_BIND_FLAGS::DEPTH_STENCIL | DR_BIND_FLAGS::SHADER_RESOURCE;
  depthTextureDesc.width = screenWidth;
  depthTextureDesc.height = screenHeight;
  depthTextureDesc.Format = DR_FORMAT::kD24_UNORM_S8_UINT;

  m_GBuffer1DSoptions = dr_gfx_shared(dc.createDepthStencil(depthTextureDesc));
  m_SSAODSoptions     = dr_gfx_shared(dc.createDepthStencil(depthTextureDesc));
  //depthTextureDesc.width = static_cast<Int32>(1024);
  //depthTextureDesc.height = static_cast<Int32>(1024);
  m_ShadowDSoptions   = dr_gfx_shared(dc.createDepthStencil(depthTextureDesc));

  ResourceManager::loadResource(_T("ScreenAlignedQuad.3ds"));

  Viewport vp;
  vp.minDepth = 0.001f;
  vp.maxDepth = 10000.0f;
  vp.width = static_cast<UInt32>(1024);
  vp.height = static_cast<UInt32>(1024);

  for (size_t camIndex = 0; camIndex < 4; ++camIndex) {
    m_vecShadowCamera[camIndex]->setViewport(vp);
  }

  m_GBuffer1Pass.init(&m_GBuffer1InitData);
  m_ShadowPass.init(&m_ShadowInitData);
  m_SSAOPass.init(&m_SSAOInitData);
  m_PostProcessingPass.init(&m_PostProcessingInitData);
}

void
RenderMan::draw() {
  GraphicsDriver::API().clear();
  updateShadowCameras();

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

  for (size_t camIndex = 0; camIndex < 1; ++camIndex) {
    queryRequest = SceneGraph::query(*m_vecShadowCamera[camIndex],
                                     QUERY_ORDER::kFrontToBack,          
                                     QUERY_PROPERTY::kOpaque |
                                     QUERY_PROPERTY::kDynamic |
                                     QUERY_PROPERTY::kStatic);
    m_ShadowDrawData.activeCam = m_vecShadowCamera[camIndex];
    m_ShadowDrawData.shadowIndex = camIndex;
    m_ShadowDrawData.models = &queryRequest;
    m_ShadowDrawData.OutRt = m_RTShadow;
    m_ShadowDrawData.dsOptions = m_ShadowDSoptions;
    m_ShadowPass.draw(&m_ShadowDrawData);
  }

  m_SSAODrawData.InRt = m_RTGBuffer1;
  m_SSAODrawData.OutRt = m_RTSSAO;
  m_SSAODrawData.dsOptions = m_SSAODSoptions;
  m_SSAOPass.draw(&m_SSAODrawData);

  m_PostProcessingDrawData.activeCam = mainCam;
  m_PostProcessingDrawData.DirLight = Vector4D(m_vec3DirectionalLight, 1.0f);
  m_PostProcessingDrawData.Gbuffer1RT = m_RTGBuffer1;
  m_PostProcessingDrawData.SSAORT = m_RTSSAO;
  m_PostProcessingDrawData.ShadowRT = m_RTShadow;
  m_PostProcessingDrawData.Lights = &lights[0];
  m_PostProcessingDrawData.ActiveLights = 128;
  m_PostProcessingDrawData.ShadowCam = &m_vecShadowCamera;
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
  m_ShadowPass.recompileShader();
  m_SSAOPass.recompileShader();
  m_PostProcessingPass.recompileShader();
}

void
RenderMan::updateShadowCameras() {
  m_szActiveShadowCameras = Math::clamp(m_szActiveShadowCameras,
                                        static_cast<size_t>(1),
                                        static_cast<size_t>(4));
  std::vector<float> partitions = calculatePartitions(m_szActiveShadowCameras);

  std::shared_ptr<Camera> mainCam = CameraManager::getActiveCamera();
  float fViewportWidth = mainCam->getViewportWidth();
  float fViewportHeight = mainCam->getViewportHeight();
  float fNearPlane = mainCam->getNearPlane();
  float fFarPlane = mainCam->getFarPlane();
  float fFov = mainCam->getFOV();

  std::pair<Vector3D, float> subFrustraSphere;

  Vector3D TrueCenter;
  
  for (size_t i = 0; i < m_szActiveShadowCameras; ++i) {
    subFrustraSphere = frustrumSphere(fViewportWidth,
                                      fViewportHeight,
                                      Math::lerp(fNearPlane,
                                                 fFarPlane,
                                                 //(m_bFitToFrustrum || true) ? partitions[i + 0] : partitions[0]),
                                                 partitions[i + 0]),
                                      Math::lerp(fNearPlane,
                                                 fFarPlane,
                                                 partitions[i + 1]),
                                      fFov);

    TrueCenter = mainCam->getPosition() +
                 (mainCam->getDirection() * subFrustraSphere.first.z) +
                 (m_vec3DirectionalLight * subFrustraSphere.second);

    m_vecShadowCamera[i]->setPosition(TrueCenter -
                                      (m_vec3DirectionalLight * m_fDepth));
    m_vecShadowCamera[i]->setTarget(TrueCenter);
    m_vecShadowCamera[i]->createProyection(subFrustraSphere.second * 2.0f,
                                           subFrustraSphere.second * 2.0f,
                                           0.001f,
                                           m_fDepth);
  }
}

std::vector<float>
RenderMan::calculatePartitions(size_t cuts) {
  float fLinearValue = 0.0f,
        fLnValue = 0.0f,
        fRealValue = 0.0f;

  std::vector<float> realValues;

  for (size_t i = 0; i < cuts + 1; ++i) {
    fLinearValue = (float)i / cuts;
    //fLnValue = log(1.0f + ((float)i / cuts));
    fLnValue = Math::pow((float)i / cuts, 2.0f);
    fRealValue = Math::lerp(fLnValue, fLinearValue, fLinearValue);

    realValues.push_back(fRealValue);
  }
  return realValues;
}

std::pair<Vector3D, float>
RenderMan::frustrumSphere(float fVW,
                          float fVH,
                          float fNP,
                          float fFP,
                          float fF) {
  float fK = Math::sqrt(1.0f + ((fVH * fVH) / (fVW * fVW))) *
    Math::tan(Math::DEGREE_TO_RADIAN * fF * 0.5f);

  Vector3D fCenter = Vector3D(0.0f, 0.0f, 0.0f);
  float fRadius = 0.0f;

  if ((fK * fK) < ((fFP - fNP) / (fFP + fNP))) {
    fCenter = Vector3D(0.0f, 0.0f, 0.5f * (fFP + fNP) * (1.0f + (fK * fK)));
    fRadius = 0.5f * Math::sqrt((Math::pow((fFP - fNP), 2.0f)) +
      (2.0f * ((fFP * fFP) + (fNP * fNP)) * (fK * fK)) +
      (Math::pow(fFP + fNP, 2.0f) * Math::pow(fK, 4.0f)));
  }
  else {
    fCenter = Vector3D(0.0f, 0.0f, fFP);
    fRadius = fFP * fK;
  }

  return std::pair<Vector3D, float>(fCenter, fRadius);
}

}