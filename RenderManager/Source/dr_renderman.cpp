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

  //dc.createTextureFromMemory(, cubeMapDesc);

  screenWidth = 1280;
  screenHeight = 720;

  for (SizeT i = 0; i < 4; ++i) {
    vecShadowCamera[i] = std::make_shared<Camera>();
  }
  m_szActiveShadowCameras = 4;
  //m_vec3DirectionalLight = Vector3D(-1.0f, -1.0f, 0.0f).normalize();
  //m_vec3DirectionalLight = Vector3D(0.0f, -10000.0f, 0.1f).normalize();
  m_vec3DirectionalLight = Vector3D(-1.0f, -1.0f, -1.0f).normalize();
  m_fDepth = 10000.0f;
  m_bFitToScene = false;
  partitions = calculatePartitions(m_szActiveShadowCameras);
  std::shared_ptr<Camera> mainCam = CameraManager::getActiveCamera();
  float fViewportWidth = static_cast<float>(mainCam->getViewportWidth());
  float fViewportHeight = static_cast<float>(mainCam->getViewportHeight());
  float fNearPlane = mainCam->getNearPlane();
  float fFarPlane = mainCam->getFarPlane();
  float fFov = mainCam->getFOV();

  for (size_t i = 0; i < m_szActiveShadowCameras; ++i) {
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

  m_TexDescDefault.width = screenWidth;
  m_TexDescDefault.height = screenHeight;
  m_TexDescDefault.pitch = screenWidth * 4;
  m_TexDescDefault.dimension = DR_DIMENSION::k2D;
  m_TexDescDefault.Format = DR_FORMAT::kR32G32B32A32_FLOAT;
  m_TexDescDefault.mipLevels = 0;
  m_TexDescDefault.CPUAccessFlags = 0;
  m_TexDescDefault.genMipMaps = true;
  m_TexDescDefault.bindFlags = DR_BIND_FLAGS::SHADER_RESOURCE |
                               DR_BIND_FLAGS::RENDER_TARGET;

  m_RTGBuffer        = dr_gfx_shared(dc.createRenderTarget(m_TexDescDefault, 4));
  m_RTSSAO           = dr_gfx_shared(dc.createRenderTarget(m_TexDescDefault, 1));
  m_RTSSAOInitBlur   = dr_gfx_shared(dc.createRenderTarget(m_TexDescDefault, 1));
  m_RTSSAOFinalBlur  = dr_gfx_shared(dc.createRenderTarget(m_TexDescDefault, 1));
  m_RTLightning      = dr_gfx_shared(dc.createRenderTarget(m_TexDescDefault, 1));
  m_RTPostProcessing = dr_gfx_shared(dc.createRenderTarget(m_TexDescDefault, 1));
  //m_TexDescDefault.width  = static_cast<Int32>(2048);
  //m_TexDescDefault.height = static_cast<Int32>(2048);
  //m_TexDescDefault.pitch =  m_TexDescDefault.width * 4;
  m_RTShadow         = dr_gfx_shared(dc.createRenderTarget(m_TexDescDefault, 1));
  m_TexDescDefault.Format = DR_FORMAT::kR32_FLOAT;
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
  m_ShadowDSoptions         = dr_gfx_shared(dc.createDepthStencil(depthTextureDesc));
  m_LightningDSoptions      = dr_gfx_shared(dc.createDepthStencil(depthTextureDesc));
  m_PostProcessingDSoptions = dr_gfx_shared(dc.createDepthStencil(depthTextureDesc));

  ResourceManager::loadResource(_T("ScreenAlignedQuad.3ds"));

  Viewport vp;
  vp.minDepth = 0.001f;
  vp.maxDepth = 10000.0f;
  vp.width = static_cast<UInt32>(1024);
  vp.height = static_cast<UInt32>(1024);

  for (size_t camIndex = 0; camIndex < 4; ++camIndex) {
    vecShadowCamera[camIndex]->setViewport(vp);
  }

  m_GBufferPass.init(&m_GBufferInitData);
  m_SSAOPass.init(&m_SSAOInitData);
  m_HorBlurPass.init(&m_HorBlurInitData);
  m_VerBlurPass.init(&m_VerBlurInitData);
  m_ShadowPass.init(&m_ShadowInitData);
  m_LightningPass.init(&m_LightningInitData);
  m_PostProcessingPass.init(&m_PostProcessingInitData);

  m_particlePass.init(&m_particleInitData);
  driderSDK::ParticleEmitter emitter;
  driderSDK::ParticleEmitterAttributes attr;
  //System
  attr.m_maxParticles = ParticleEmitter::MAX_PARTICLES;
  attr.m_initialTime = 0;
  attr.m_rate = 0.1;
  attr.m_systemMaxLife = 5000;
  attr.m_particleMaxLife = 2;
  attr.m_numParticlesToEmit = 4000;
  attr.m_isActive = true;

  ////Speed / Velocity
  //attr.m_initialSpeedLimit = 1200;
  //attr.m_finalSpeedLimit = 5;
  //attr.m_initialVelocityRandomMin = driderSDK::Vector3D(-0, -0, -0);
  //attr.m_initialVelocityRandomMax = driderSDK::Vector3D(0, 1800, 0);

  ////Position
  //attr.m_initialPositionRandomMin = driderSDK::Vector3D(-200, -0, -200);
  //attr.m_initialPositionRandomMax = driderSDK::Vector3D(50, 0, 50);

  ////Color
  //attr.m_initialColor = driderSDK::Vector3D(1, 0.1, 0.1);
  //attr.m_finalColor = driderSDK::Vector3D(0.0, 0.0, 0.0);
  //attr.m_colorRandomMin = driderSDK::Vector3D(0, 0, 0);
  //attr.m_colorRandomMax = driderSDK::Vector3D(0.0, 0, 0);

  ////Scale
  //attr.m_initialScale = 2;
  //attr.m_finaleScale = 5;
  //attr.m_scaleFactorRandomMin = 0.8;
  //attr.m_scaleFactorRandomMax = 5;

  m_emitter.init(attr);
}

void
RenderMan::draw(const RenderTarget & _out, const DepthStencil & _outds) {
  //GraphicsDriver::API().clear();
  //updateShadowCameras();

  //auto mainCam = CameraManager::getActiveCamera();
  //auto queryRequest = SceneGraph::query(*mainCam,
  //                                      QUERY_ORDER::kFrontToBack,
  //                                      QUERY_PROPERTY::kOpaque | 
  //                                      QUERY_PROPERTY::kDynamic | 
  //                                      QUERY_PROPERTY::kStatic);

  //m_GBufferDrawData.activeCam = mainCam;
  //m_GBufferDrawData.models = &queryRequest;
  //m_GBufferDrawData.OutRt = m_RTGBuffer;
  //m_GBufferDrawData.dsOptions = m_GBufferDSoptions;
  //m_GBufferPass.draw(&m_GBufferDrawData);

  //m_SSAODrawData.activeCam = mainCam;
  //m_SSAODrawData.InRt = m_RTGBuffer;
  //m_SSAODrawData.OutRt = m_RTSSAO;
  //m_SSAODrawData.dsOptions = m_SSAODSoptions;
  //m_SSAOPass.draw(&m_SSAODrawData);

  //m_HorBlurDrawData.viewportDimensionX = static_cast<float>(screenWidth);
  //m_HorBlurDrawData.viewportDimensionY = static_cast<float>(screenHeight);
  //m_HorBlurDrawData.dsOptions = m_HorBlurDSoptions;
  //m_HorBlurDrawData.InRt = m_RTSSAO;
  //m_HorBlurDrawData.OutRt = m_RTSSAOInitBlur;
  //m_HorBlurPass.draw(&m_HorBlurDrawData);

  //m_VerBlurDrawData.viewportDimensionX = static_cast<float>(screenWidth);
  //m_VerBlurDrawData.viewportDimensionY = static_cast<float>(screenHeight);
  //m_VerBlurDrawData.dsOptions = m_VerBlurDSoptions;
  //m_VerBlurDrawData.InRt = m_RTSSAOInitBlur;
  //m_VerBlurDrawData.OutRt = m_RTSSAOFinalBlur;
  //m_VerBlurPass.draw(&m_VerBlurDrawData);

  //for (size_t camIndex = 0; camIndex < m_szActiveShadowCameras; ++camIndex) {
  //  queryRequest = SceneGraph::query(*vecShadowCamera[camIndex],
  //                                   QUERY_ORDER::kFrontToBack,          
  //                                   QUERY_PROPERTY::kOpaque |
  //                                   QUERY_PROPERTY::kDynamic |
  //                                   QUERY_PROPERTY::kStatic);
  //  m_ShadowDrawData.shadowCam = vecShadowCamera[camIndex];
  //  m_ShadowDrawData.models = &queryRequest;
  //  m_ShadowDrawData.OutRt = m_RTShadowDummy[camIndex];
  //  m_ShadowDrawData.dsOptions = m_ShadowDSoptions;
  //  m_ShadowPass.draw(&m_ShadowDrawData);
  //}
  //m_ShadowPass.merge(m_RTShadowDummy, m_ShadowDSoptions, m_RTShadow);

  //m_LightningDrawData.activeCam = mainCam;
  //m_LightningDrawData.DirLight = Vector4D(m_vec3DirectionalLight, 1.0f);
  //m_LightningDrawData.GbufferRT = m_RTGBuffer;
  //m_LightningDrawData.SSAORT = m_RTSSAOFinalBlur;
  //m_LightningDrawData.ShadowRT = m_RTShadow;
  //m_LightningDrawData.Lights = &lights[0];
  //m_LightningDrawData.ActiveLights = 128;
  //m_LightningDrawData.ShadowCam = &vecShadowCamera;
  //m_LightningDrawData.shadowDepths = partitions;
  //m_LightningDrawData.dsOptions = m_LightningDSoptions;
  //m_LightningDrawData.OutRt = m_RTLightning;
  //m_LightningPass.draw(&m_LightningDrawData);
  
  _out.set(GraphicsAPI::getDeviceContext(), _outds);
  //m_PostProcessingDrawData.ColorRT = m_RTLightning; 
  //m_PostProcessingPass.draw(&m_PostProcessingDrawData); 

  m_emitter.update();
  //m_particleDrawData.activeCam = mainCam;
  //m_particleDrawData.numParticles = m_emitter.m_.size();
  m_particleDrawData.particles = &m_emitter.m_particles;
  m_particleDrawData.emitter = &m_emitter;
  m_particlePass.draw(&m_particleDrawData);

  GraphicsAPI::getBackBufferRT().set(GraphicsAPI::getDeviceContext(), GraphicsAPI::getDepthStencil());
  /*
  ./ GBuffer:
  ./ SSAO:
  ./ ShadowCascades: 
   X Diffuse Acumulation: Fog
   X Specular Acumulation: SS Reflection
   X Lights y ZSkips: 
   X Opacity: Blends
  */

  //GraphicsDriver::API().swapBuffers();
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
  m_szActiveShadowCameras = Math::clamp(m_szActiveShadowCameras,
                                        static_cast<size_t>(1),
                                        static_cast<size_t>(4));

  std::shared_ptr<Camera> mainCam = CameraManager::getActiveCamera();
  Vector3D CamPos = mainCam->getPosition();
  Vector3D CamDir = mainCam->getDirection();

  float SphereRad;
  Vector3D TrueCenter;
  
  for (size_t i = 0; i < m_szActiveShadowCameras; ++i) {
    SphereRad = m_ShadowSubFrustras[i].second;
    
    TrueCenter = CamPos + (CamDir * m_ShadowSubFrustras[i].first.z);

    vecShadowCamera[i]->setPosition(TrueCenter -
                                    (m_vec3DirectionalLight * m_fDepth));
    vecShadowCamera[i]->setTarget(TrueCenter);
    vecShadowCamera[i]->createProyection(Math::floor(SphereRad * 2.0f),
                                         Math::floor(SphereRad * 2.0f),
                                         0.1f,
                                         m_fDepth + SphereRad);
                                         //m_fDepth);
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