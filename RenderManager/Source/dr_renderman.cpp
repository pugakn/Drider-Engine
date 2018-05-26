#include "dr_renderman.h"
#include <dr_graphics_driver.h>
#include <dr_device.h>
#include <dr_resource_manager.h>
#include <dr_image_info.h>
#include <dr_camera_manager.h>
#include <dr_depth_stencil.h>

#include <dr_texture_core.h>

namespace driderSDK {

RenderMan::RenderMan() {
}

RenderMan::~RenderMan() {
}

void
RenderMan::init() {
  Device& dc = GraphicsAPI::getDevice();

  ResourceManager::loadResource(_T("ScreenAlignedQuad.3ds"));

  /*
  ResourceManager::loadResource(_T("GraceCubemap.tga"));
  auto CmR = ResourceManager::getReferenceT<TextureCore>(_T("GraceCubemap.tga"));

  DrTextureDesc cubeMapDesc;
  cubeMapDesc.width = 256;
  cubeMapDesc.height = 256;
  cubeMapDesc.pitch = 256 * 4;
  cubeMapDesc.dimension = DR_DIMENSION::kCUBE_MAP;
  cubeMapDesc.Format = DR_FORMAT::kB8G8R8A8_UNORM_SRGB;
  cubeMapDesc.mipLevels = 0;
  cubeMapDesc.CPUAccessFlags = 0;
  cubeMapDesc.genMipMaps = true;
  cubeMapDesc.bindFlags = DR_BIND_FLAGS::SHADER_RESOURCE;
  cubeMapDesc.dimension = DR_DIMENSION::kCUBE_MAP;

  dc.createTextureFromMemory(, cubeMapDesc);
  */

  screenWidth = 1280;
  screenHeight = 720;
  shadowWidth = 1024;
  shadowHeight = 1024;

  //////////Shadows stuff//////////
  std::shared_ptr<Camera> mainCam = CameraManager::getActiveCamera();
  float fViewportWidth = static_cast<float>(mainCam->getViewportWidth());
  float fViewportHeight = static_cast<float>(mainCam->getViewportHeight());
  float fNearPlane = mainCam->getNearPlane();
  float fFarPlane = mainCam->getFarPlane();
  float fFov = mainCam->getFOV();

  m_szActiveShadowCameras = 4;

  m_fMinDepth = 0.1f;
  m_fMaxDepth = 10000.0f;
  m_bFitToScene = false;

  //m_vec3DirectionalLight = Vector3D(-1.0f, -1.0f, 0.0f).normalize();
  //m_vec3DirectionalLight = Vector3D(0.0f, -10000.0f, 0.1f).normalize();
  m_vec3DirectionalLight = Vector3D(-1.0f, -1.0f, -1.0f).normalize();

  partitions = calculatePartitions(m_szActiveShadowCameras);

  for (SizeT i = 0; i < m_szActiveShadowCameras; ++i) {
    m_ShadowSubFrustras[i] = frustrumSphere(fViewportHeight,
                                            fViewportWidth,
                                            Math::lerp(fNearPlane,
                                                       fFarPlane,
                                                       partitions[m_bFitToScene ? 0 : i]),
                                            Math::lerp(fNearPlane,
                                                       fFarPlane,
                                                       partitions[i + 1]),
                                            fFov);
  }
  float extraDepth = m_ShadowSubFrustras[0].second;
  extraDepth = Math::max(extraDepth, m_ShadowSubFrustras[1].second);
  extraDepth = Math::max(extraDepth, m_ShadowSubFrustras[2].second);
  extraDepth = Math::max(extraDepth, m_ShadowSubFrustras[3].second);

  Viewport vpShadow;
  vpShadow.minDepth = m_fMinDepth;
  vpShadow.maxDepth = m_fMaxDepth + extraDepth;
  vpShadow.width = shadowWidth;
  vpShadow.height = shadowHeight;

  for (SizeT camIndex = 0; camIndex < 4; ++camIndex) {
    vecShadowCamera[camIndex] = std::make_shared<Camera>();
    vecShadowCamera[camIndex]->setViewport(vpShadow);
  }
  //////////End of Shadows stuff//////////

  m_TexDescDefault.width = screenWidth;
  m_TexDescDefault.height = screenHeight;
  m_TexDescDefault.pitch = m_TexDescDefault.width * 4;
  m_TexDescDefault.dimension = DR_DIMENSION::k2D;
  m_TexDescDefault.Format = DR_FORMAT::kR16G16B16A16_FLOAT;
  m_TexDescDefault.mipLevels = 0;
  m_TexDescDefault.CPUAccessFlags = 0;
  m_TexDescDefault.genMipMaps = true;
  m_TexDescDefault.bindFlags = DR_BIND_FLAGS::SHADER_RESOURCE |
                               DR_BIND_FLAGS::RENDER_TARGET;

  m_TexDescDefault.Format = DR_FORMAT::kR16G16B16A16_FLOAT;
  m_RTGBuffer        = dr_gfx_shared(dc.createRenderTarget(m_TexDescDefault, 4));

  m_TexDescDefault.Format = DR_FORMAT::kR8_SNORM;
  m_TexDescDefault.pitch = m_TexDescDefault.width;
  m_RTSSAO           = dr_gfx_shared(dc.createRenderTarget(m_TexDescDefault, 1));

  m_TexDescDefault.Format = DR_FORMAT::kR16G16B16A16_FLOAT;
  m_TexDescDefault.pitch = m_TexDescDefault.width * 4;
  m_RTBlurInit       = dr_gfx_shared(dc.createRenderTarget(m_TexDescDefault, 1));

  m_TexDescDefault.Format = DR_FORMAT::kR8_SNORM;
  m_TexDescDefault.pitch = m_TexDescDefault.width * 4;
  m_RTSSAOBlur       = dr_gfx_shared(dc.createRenderTarget(m_TexDescDefault, 1));

  m_TexDescDefault.Format = DR_FORMAT::kR16G16B16A16_FLOAT;
  m_TexDescDefault.pitch = m_TexDescDefault.width * 4;
  m_RTLightning      = dr_gfx_shared(dc.createRenderTarget(m_TexDescDefault, 1));
  m_RTPostProcessing = dr_gfx_shared(dc.createRenderTarget(m_TexDescDefault, 1));
  m_RTPreFinalBlur   = dr_gfx_shared(dc.createRenderTarget(m_TexDescDefault, 1));

  m_TexDescDefault.Format = DR_FORMAT::kR32G32B32A32_FLOAT;
  m_TexDescDefault.width  = shadowWidth;
  m_TexDescDefault.height = shadowHeight;
  m_TexDescDefault.pitch =  m_TexDescDefault.width * 4;
  m_RTShadow         = dr_gfx_shared(dc.createRenderTarget(m_TexDescDefault, 1));
  m_TexDescDefault.pitch = m_TexDescDefault.width;

  m_TexDescDefault.Format = DR_FORMAT::kR32_FLOAT;
  m_TexDescDefault.pitch = m_TexDescDefault.width;
  m_RTShadowDummy[0] = dr_gfx_shared(dc.createRenderTarget(m_TexDescDefault, 1));
  m_RTShadowDummy[1] = dr_gfx_shared(dc.createRenderTarget(m_TexDescDefault, 1));
  m_RTShadowDummy[2] = dr_gfx_shared(dc.createRenderTarget(m_TexDescDefault, 1));
  m_RTShadowDummy[3] = dr_gfx_shared(dc.createRenderTarget(m_TexDescDefault, 1));

  DrDepthStencilDesc depthTextureDesc;
  depthTextureDesc.bindFlags = DR_BIND_FLAGS::DEPTH_STENCIL | DR_BIND_FLAGS::SHADER_RESOURCE;
  depthTextureDesc.width = screenWidth;
  depthTextureDesc.height = screenHeight;
  depthTextureDesc.Format = DR_FORMAT::kD24_UNORM_S8_UINT;

  m_GBufferDSoptions        = dr_gfx_shared(dc.createDepthStencil(depthTextureDesc));
  m_SSAODSoptions           = dr_gfx_shared(dc.createDepthStencil(depthTextureDesc));
  m_HorBlurDSoptions        = dr_gfx_shared(dc.createDepthStencil(depthTextureDesc));
  m_VerBlurDSoptions        = dr_gfx_shared(dc.createDepthStencil(depthTextureDesc));
  m_LightningDSoptions      = dr_gfx_shared(dc.createDepthStencil(depthTextureDesc));
  m_PostProcessingDSoptions = dr_gfx_shared(dc.createDepthStencil(depthTextureDesc));
  depthTextureDesc.width  = shadowWidth;
  depthTextureDesc.height = shadowHeight;
  m_ShadowDSoptions         = dr_gfx_shared(dc.createDepthStencil(depthTextureDesc));

  m_GBufferPass.init(&m_GBufferInitData);
  m_SSAOPass.init(&m_SSAOInitData);
  m_HorBlurPass.init(&m_HorBlurInitData);
  m_VerBlurPass.init(&m_VerBlurInitData);
  m_ShadowPass.init(&m_ShadowInitData);
  m_LightningPass.init(&m_LightningInitData);
  m_PostProcessingPass.init(&m_PostProcessingInitData);
}

void
RenderMan::draw() {
  //GraphicsDriver::API().clear();

  updateShadowCameras();

  auto mainCam = CameraManager::getActiveCamera();
  auto queryRequest = SceneGraph::query(*mainCam,
                                        QUERY_ORDER::kFrontToBack,
                                        QUERY_PROPERTY::kOpaque | 
                                        QUERY_PROPERTY::kDynamic | 
                                        QUERY_PROPERTY::kStatic);

  m_GBufferDrawData.activeCam = mainCam;
  m_GBufferDrawData.models = &queryRequest;
  m_GBufferDrawData.OutRt = m_RTGBuffer;
  m_GBufferDrawData.dsOptions = m_GBufferDSoptions;
  m_GBufferPass.draw(&m_GBufferDrawData);

  m_SSAODrawData.activeCam = mainCam;
  m_SSAODrawData.InRt = m_RTGBuffer;
  m_SSAODrawData.OutRt = m_RTSSAO;
  m_SSAODrawData.dsOptions = m_SSAODSoptions;
  m_SSAOPass.draw(&m_SSAODrawData);

  m_HorBlurDrawData.viewportDimensionX = static_cast<float>(screenWidth);
  m_HorBlurDrawData.viewportDimensionY = static_cast<float>(screenHeight);
  m_HorBlurDrawData.dsOptions = m_HorBlurDSoptions;
  m_HorBlurDrawData.InRt = m_RTSSAO;
  m_HorBlurDrawData.OutRt = m_RTBlurInit;
  m_HorBlurPass.draw(&m_HorBlurDrawData);

  m_VerBlurDrawData.viewportDimensionX = static_cast<float>(screenWidth);
  m_VerBlurDrawData.viewportDimensionY = static_cast<float>(screenHeight);
  m_VerBlurDrawData.dsOptions = m_VerBlurDSoptions;
  m_VerBlurDrawData.InRt = m_RTBlurInit;
  m_VerBlurDrawData.OutRt = m_RTSSAOBlur;
  m_VerBlurPass.draw(&m_VerBlurDrawData);

  for (size_t camIndex = 0; camIndex < m_szActiveShadowCameras; ++camIndex) {
    queryRequest = SceneGraph::query(*vecShadowCamera[camIndex],
                                     QUERY_ORDER::kFrontToBack,          
                                     QUERY_PROPERTY::kOpaque |
                                     QUERY_PROPERTY::kDynamic |
                                     QUERY_PROPERTY::kStatic);
    m_ShadowDrawData.shadowCam = vecShadowCamera[camIndex];
    m_ShadowDrawData.models = &queryRequest;
    m_ShadowDrawData.OutRt = m_RTShadowDummy[camIndex];
    m_ShadowDrawData.dsOptions = m_ShadowDSoptions;
    m_ShadowPass.draw(&m_ShadowDrawData);
  }
  m_ShadowPass.merge(m_RTShadowDummy, m_ShadowDSoptions, m_RTShadow);
  /*
  */
  m_LightningDrawData.activeCam = mainCam;
  m_LightningDrawData.DirLight = Vector4D(m_vec3DirectionalLight, 1.0f);
  m_LightningDrawData.GbufferRT = m_RTGBuffer;
  m_LightningDrawData.SSAORT = m_RTSSAOBlur;
  m_LightningDrawData.ShadowRT = m_RTShadow;
  m_LightningDrawData.Lights = &lights[0];
  m_LightningDrawData.ActiveLights = 128;
  m_LightningDrawData.ShadowCam = &vecShadowCamera;
  m_LightningDrawData.shadowDepths = partitions;
  m_LightningDrawData.shadowSizes = Vector4D(shadowWidth,
                                             shadowWidth,
                                             shadowWidth,
                                             shadowWidth);
  m_LightningDrawData.shadowSizesProportion[0] = 1.0f;
  m_LightningDrawData.shadowSizesProportion[1] = m_ShadowSubFrustras[1].second /
                                                 m_ShadowSubFrustras[0].second;
  m_LightningDrawData.shadowSizesProportion[2] = m_ShadowSubFrustras[2].second /
                                                 m_ShadowSubFrustras[0].second;
  m_LightningDrawData.shadowSizesProportion[3] = m_ShadowSubFrustras[3].second /
                                                 m_ShadowSubFrustras[0].second;
  m_LightningDrawData.dsOptions = m_LightningDSoptions;
  m_LightningDrawData.OutRt = m_RTLightning;
  m_LightningPass.draw(&m_LightningDrawData);

  m_HorBlurDrawData.viewportDimensionX = static_cast<float>(screenWidth);
  m_HorBlurDrawData.viewportDimensionY = static_cast<float>(screenHeight);
  m_HorBlurDrawData.dsOptions = m_HorBlurDSoptions;
  m_HorBlurDrawData.InRt = m_RTLightning;
  m_HorBlurDrawData.OutRt = m_RTBlurInit;
  m_HorBlurPass.draw(&m_HorBlurDrawData);

  m_VerBlurDrawData.viewportDimensionX = static_cast<float>(screenWidth);
  m_VerBlurDrawData.viewportDimensionY = static_cast<float>(screenHeight);
  m_VerBlurDrawData.dsOptions = m_VerBlurDSoptions;
  m_VerBlurDrawData.InRt = m_RTBlurInit;
  m_VerBlurDrawData.OutRt = m_RTPreFinalBlur;
  m_VerBlurPass.draw(&m_VerBlurDrawData);
  
  m_PostProcessingDrawData.ColorRT = m_RTLightning;
  m_PostProcessingDrawData.ColorBlurRT = m_RTPreFinalBlur;
  m_PostProcessingDrawData.Gbuffer = m_RTGBuffer;
  m_PostProcessingPass.draw(&m_PostProcessingDrawData);

  /*
  ./ GBuffer:
  ./ SSAO:
  ./ ShadowCascades: 
   X Diffuse Acumulation: Fog
   X Specular Acumulation: SS Reflection
   X Lights y ZSkips: 
   X Opacity: Blends
  */

  GraphicsDriver::API().swapBuffers();
}

void
RenderMan::exit() {
}

void
RenderMan::recompile() {
  m_GBufferPass.recompileShader();
  m_SSAOPass.recompileShader();
  m_HorBlurPass.recompileShader();
  m_VerBlurPass.recompileShader();
  m_ShadowPass.recompileShader();
  m_LightningPass.recompileShader();
  m_PostProcessingPass.recompileShader();
}

void
RenderMan::updateShadowCameras() {
  std::shared_ptr<Camera> mainCam = CameraManager::getActiveCamera();
  Vector3D CamPos = mainCam->getPosition();
  Vector3D CamDir = mainCam->getDirection();

  float extraDepth = m_ShadowSubFrustras[0].second;
  extraDepth = Math::max(m_ShadowSubFrustras[1].second, extraDepth);
  extraDepth = Math::max(m_ShadowSubFrustras[2].second, extraDepth);
  extraDepth = Math::max(m_ShadowSubFrustras[3].second, extraDepth);

  Vector3D TrueCenter;
  
  for (SizeT i = 0; i < m_szActiveShadowCameras; ++i) {
    float SphereRad = m_ShadowSubFrustras[i].second;
    
    TrueCenter = CamPos + (CamDir * m_ShadowSubFrustras[i].first.z);

    vecShadowCamera[i]->setPosition(TrueCenter -
                                    (m_vec3DirectionalLight * m_fMaxDepth));
    vecShadowCamera[i]->setTarget(TrueCenter);
    vecShadowCamera[i]->createProyection(Math::floor(SphereRad * 2.0f),
                                         Math::floor(SphereRad * 2.0f),
                                         m_fMinDepth,
                                         m_fMaxDepth + extraDepth);
  }
}

std::vector<float>
RenderMan::calculatePartitions(SizeT cuts) {
  float fProportion = 0.0f,
        fLinearValue = 0.0f,
        fPwValue = 0.0f,
        fRealValue = 0.0f;

  std::vector<float> realValues;

  for (SizeT i = 0; i < cuts + 1; ++i) {
    fProportion = (float)i / cuts;

    //fLinearValue = fProportion;
    //fPwValue = Math::pow(fProportion, 2.0f);
    //fRealValue = Math::lerp(fPwValue, fLinearValue, fProportion);

    fLinearValue = Math::pow(fProportion, 2.0f);
    fPwValue = Math::pow(fProportion, 6.0f);
    fRealValue = Math::lerp(fLinearValue, fPwValue, fProportion);
    
    //realValues.push_back(fLinearValue);
    //realValues.push_back(fPwValue);
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
  Vector3D fCenter;
  float fRadius;
  float fK = Math::sqrt(1.0f + ((fVH * fVH) / (fVW * fVW))) * Math::tan(Math::DEGREE_TO_RADIAN * fF * 0.5f);
  
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