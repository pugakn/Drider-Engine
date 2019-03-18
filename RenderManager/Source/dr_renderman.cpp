#include "dr_renderman.h"
#include <dr_graphics_api.h>
#include <dr_graphics_driver.h>
#include <dr_device_context.h>
#include <dr_device.h>
#include <dr_texture.h>
#include <dr_camera_manager.h>
#include <dr_depth_stencil.h>

#include <dr_radian.h>
#include <dr_vertex.h>
#include <dr_vertex_buffer.h>
#include <dr_index_buffer.h>

#include <dr_time.h>
#include <dr_timer.h>

namespace driderSDK {

void
benchmark(Timer& timer, float& timeScope, String text) {
  float currentTime = timer.getMicroseconds();
  float deltaTime = currentTime - timeScope;
  std::cout << text << deltaTime << std::endl;
  timeScope = currentTime;
}


RenderManager::RenderManager() {
}

RenderManager::~RenderManager() {
}

void
RenderManager::init() {
  Device& dc = GraphicsAPI::getDevice();

  m_fLuminanceDelta = 0.0f;
  m_BloomThreshold = Vector3D(0.75f, 0.75f, 0.75f);

  ///////////Resolutions///////////

  //////////16:9//////////

  //8K
  //screenWidth = 7680;
  //screenHeight = 4320;

  //4K
  //screenWidth = 4096;
  //screenHeight = 2304;

  //UHD
  //screenWidth = 3840;
  //screenHeight = 2160;

  //2K
  //screenWidth = 2048;
  //screenHeight = 1152;

  //1080p o FHD
  //screenWidth = 1920;
  //screenHeight = 1080;

  //720p o HD
  screenWidth = 1280;
  screenHeight = 720;

  //////////21:9//////////

  //1080p o FHD
  //screenWidth = 2560;
  //screenHeight = 1080;

  //720p o HD
  //screenWidth = 1680;
  //screenHeight = 720;

  static const float blurScale = 0.5f;

  float shadowQualityMultiplier = 1.0f;
  shadowWidth = static_cast<UInt32>(1024.0f * shadowQualityMultiplier);
  shadowHeight = static_cast<UInt32>(1024.0f * shadowQualityMultiplier);

  //////////Shadows stuff//////////
  std::shared_ptr<Camera> mainCam = CameraManager::getActiveCamera();
  float fViewportWidth = static_cast<float>(mainCam->getViewportWidth());
  float fViewportHeight = static_cast<float>(mainCam->getViewportHeight());
  float fNearPlane = mainCam->getNearPlane();
  float fFarPlane = mainCam->getFarPlane();
  float fFov = mainCam->getFOV();

  m_szActiveShadowCameras = Math::max(4, 4);

  m_fMinDepth = 0.1f;
  m_fMaxDepth = 10000.0f;
  m_bFitToScene = false;

  m_vecDirectionalLights.push_back(Vector3D(1.0f, -1.0f, -1.0f).normalize());
  //m_vecDirectionalLights.push_back(Vector3D(1.0f, -1.0f, 1.0f).normalize());
  //m_vecDirectionalLights.push_back(Vector3D(-1.0f, -1.0f, -1.0f).normalize());
  //m_vecDirectionalLights.push_back(Vector3D(-1.0f, -1.0f, 1.0f).normalize());

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
    vecShadowCamera[camIndex] = std::make_unique<Camera>();
    vecShadowCamera[camIndex]->setViewport(vpShadow);
  }

  /////////Creation of RT's & DS's/////////
  std::vector<Texture*> vecTextures;

  DrTextureDesc texDescDefault;
  //RenderTarget base desc
  {
    texDescDefault.dimension = DR_DIMENSION::k2D;
    texDescDefault.width = screenWidth;
    texDescDefault.height = screenHeight;
    texDescDefault.Format = DR_FORMAT::kR16G16B16A16_FLOAT;
    texDescDefault.pitch = texDescDefault.width * 4 * 2;
    texDescDefault.mipLevels = 0;
    texDescDefault.CPUAccessFlags = 0;
    texDescDefault.genMipMaps = true;
    texDescDefault.bindFlags = DR_BIND_FLAGS::SHADER_RESOURCE |
                                 DR_BIND_FLAGS::RENDER_TARGET;
  }

  //DepthStencil Base
  DrDepthStencilDesc commonTextureDesc;
  {
    commonTextureDesc.bindFlags = DR_BIND_FLAGS::DEPTH_STENCIL |
                                  DR_BIND_FLAGS::SHADER_RESOURCE;
    commonTextureDesc.width  = screenWidth;
    commonTextureDesc.height = screenHeight;
    commonTextureDesc.Format = DR_FORMAT::kD24_UNORM_S8_UINT;
  }
  
  //Gbuffer
  {
    //RenderTarget
    texDescDefault.width = screenWidth;
    texDescDefault.height = screenHeight;
    texDescDefault.Format = DR_FORMAT::kR32G32B32A32_FLOAT;
    texDescDefault.pitch = texDescDefault.width * 4 * 4;

    GFXUnique<Texture> PositionTexure = dr_gfx_unique<Texture>(dc.createEmptyTexture(texDescDefault));
    vecTextures.push_back(PositionTexure.get());
    vecTextures.push_back(PositionTexure.get());

    texDescDefault.Format = DR_FORMAT::kR16G16B16A16_FLOAT;
    texDescDefault.pitch = texDescDefault.width * 4 * 2;

    GFXUnique<Texture> BasicTexure = dr_gfx_unique<Texture>(dc.createEmptyTexture(texDescDefault));
    vecTextures.push_back(BasicTexure.get());
    vecTextures.push_back(BasicTexure.get());

    m_RTGBuffer = dr_gfx_unique(dc.createRenderTarget(texDescDefault, 4));

    vecTextures.clear();
    PositionTexure.release();
    BasicTexure.release();

    //DepthStencil
    commonTextureDesc.width = texDescDefault.width;
    commonTextureDesc.height = texDescDefault.height;
    m_GBufferDSoptions = dr_gfx_unique(dc.createDepthStencil(commonTextureDesc));
  }

  //Shadows Auxiliars
  {
    //RenderTarget

    //Dummies
    texDescDefault.width = shadowWidth;
    texDescDefault.height = shadowHeight;
    texDescDefault.Format = DR_FORMAT::kR32_FLOAT;
    texDescDefault.pitch = texDescDefault.width * 1 * 4;
    m_RTShadowDummy[0] = dr_gfx_unique(dc.createRenderTarget(texDescDefault, 1));
    m_RTShadowDummy[1] = dr_gfx_unique(dc.createRenderTarget(texDescDefault, 1));
    m_RTShadowDummy[2] = dr_gfx_unique(dc.createRenderTarget(texDescDefault, 1));
    m_RTShadowDummy[3] = dr_gfx_unique(dc.createRenderTarget(texDescDefault, 1));

    //Compressed
    texDescDefault.width = texDescDefault.width;
    texDescDefault.height = texDescDefault.height;
    texDescDefault.Format = DR_FORMAT::kR32G32B32A32_FLOAT;
    texDescDefault.pitch = texDescDefault.width * 4 * 4;
    texDescDefault.bindFlags |= DR_BIND_FLAGS::UNORDERED_ACCESS;
    
    GFXUnique<Texture> ShadowTexure = dr_gfx_unique<Texture>(dc.createEmptyTexture(texDescDefault));
    vecTextures.push_back(ShadowTexure.get());

    m_RTShadow = dr_gfx_unique(dc.createRenderTarget(vecTextures));

    vecTextures.clear();
    ShadowTexure.release();

    texDescDefault.bindFlags &= ~DR_BIND_FLAGS::UNORDERED_ACCESS;

    //DepthStencil
    commonTextureDesc.width = texDescDefault.width;
    commonTextureDesc.height = texDescDefault.height;
    m_ShadowDSoptions = dr_gfx_unique(dc.createDepthStencil(commonTextureDesc));
  }

  //SSAO & SSShadow
  {
    //RenderTarget
    //texDescDefault.width = screenWidth * blurScale;
    //texDescDefault.height = screenHeight * blurScale;
    texDescDefault.width = screenWidth;
    texDescDefault.height = screenHeight;
    texDescDefault.Format = DR_FORMAT::kR8G8_UNORM;
    texDescDefault.pitch = texDescDefault.width * 2 * 1;
    texDescDefault.bindFlags |= DR_BIND_FLAGS::UNORDERED_ACCESS;

    GFXUnique<Texture> SSAO_SSShadowTexure = dr_gfx_unique<Texture>(dc.createEmptyTexture(texDescDefault));
    vecTextures.push_back(SSAO_SSShadowTexure.get());
    m_RTSSAO_SSShadow = dr_gfx_unique(dc.createRenderTarget(vecTextures));

    texDescDefault.bindFlags &= ~DR_BIND_FLAGS::UNORDERED_ACCESS;

    vecTextures.clear();
    SSAO_SSShadowTexure.release();
  }

  //Blur Aux
  {
    //RenderTarget
    texDescDefault.width = static_cast<UInt32>(screenWidth * blurScale);
    texDescDefault.height = static_cast<UInt32>(screenHeight * blurScale);
    texDescDefault.Format = DR_FORMAT::kR16G16B16A16_FLOAT;
    texDescDefault.pitch = texDescDefault.width * 3 * 2;
    texDescDefault.bindFlags |= DR_BIND_FLAGS::UNORDERED_ACCESS;

    GFXUnique<Texture> BlurTexure = dr_gfx_unique<Texture>(dc.createEmptyTexture(texDescDefault));
    vecTextures.push_back(BlurTexure.get());
    m_RTBlurInit = dr_gfx_unique(dc.createRenderTarget(vecTextures));

    texDescDefault.bindFlags &= ~DR_BIND_FLAGS::UNORDERED_ACCESS;

    vecTextures.clear();
    BlurTexure.release();
  }

  //Blurred SSAO & Shadow
  {
    //RenderTarget
    texDescDefault.width = static_cast<UInt32>(screenWidth * blurScale);
    texDescDefault.height = static_cast<UInt32>(screenHeight * blurScale);
    texDescDefault.Format = DR_FORMAT::kR16G16_FLOAT;
    texDescDefault.pitch = texDescDefault.width * 2 * 2;
    texDescDefault.bindFlags |= DR_BIND_FLAGS::UNORDERED_ACCESS;

    GFXUnique<Texture> SSAOBlurTexure = dr_gfx_unique<Texture>(dc.createEmptyTexture(texDescDefault));
    vecTextures.push_back(SSAOBlurTexure.get());
    m_RTSSAO_SSShadowBlur = dr_gfx_unique(dc.createRenderTarget(vecTextures));

    texDescDefault.bindFlags &= ~DR_BIND_FLAGS::UNORDERED_ACCESS;

    vecTextures.clear();
    SSAOBlurTexure.release();
  }

  //SSReflection
  {
    texDescDefault.width = static_cast<UInt32>(screenWidth * blurScale);
    texDescDefault.height = static_cast<UInt32>(screenHeight * blurScale);
    //texDescDefault.width = screenWidth;
    //texDescDefault.height = screenHeight;
    texDescDefault.Format = DR_FORMAT::kR8G8B8A8_UNORM;
    texDescDefault.pitch = texDescDefault.width * 4 * 1;
    texDescDefault.bindFlags |= DR_BIND_FLAGS::UNORDERED_ACCESS;

    GFXUnique<Texture> SSRefTexure = dr_gfx_unique<Texture>(dc.createEmptyTexture(texDescDefault));
    vecTextures.push_back(SSRefTexure.get());
    m_RTSSReflection = dr_gfx_unique(dc.createRenderTarget(vecTextures));

    texDescDefault.bindFlags &= ~DR_BIND_FLAGS::UNORDERED_ACCESS;

    vecTextures.clear();
    SSRefTexure.release();
  }

  //Lightning & LightningBlur
  {
    //RenderTarget
    texDescDefault.width = static_cast<UInt32>(screenWidth);
    texDescDefault.height = static_cast<UInt32>(screenHeight);
    texDescDefault.Format = DR_FORMAT::kR16G16B16A16_FLOAT;
    texDescDefault.pitch = texDescDefault.width * 4;
    texDescDefault.bindFlags |= DR_BIND_FLAGS::UNORDERED_ACCESS;
    texDescDefault.CPUAccessFlags = DR_CPU_ACCESS_FLAG::drRead;

    GFXUnique<Texture> ColorTexure = dr_gfx_unique<Texture>(dc.createEmptyTexture(texDescDefault));
    vecTextures.push_back(ColorTexure.get());
    m_RTLightning = dr_gfx_unique(dc.createRenderTarget(vecTextures));

    texDescDefault.CPUAccessFlags = 0;
    texDescDefault.bindFlags &= ~DR_BIND_FLAGS::UNORDERED_ACCESS;

    vecTextures.clear();
    ColorTexure.release();

    texDescDefault.width = static_cast<UInt32>(screenWidth * blurScale);
    texDescDefault.height = static_cast<UInt32>(screenHeight * blurScale);
    texDescDefault.Format = DR_FORMAT::kR16G16B16A16_FLOAT;
    texDescDefault.pitch = texDescDefault.width * 4 * 2;
    texDescDefault.bindFlags |= DR_BIND_FLAGS::UNORDERED_ACCESS;

    GFXUnique<Texture> ColorBlurTexure = dr_gfx_unique<Texture>(dc.createEmptyTexture(texDescDefault));
    vecTextures.push_back(ColorBlurTexure.get());
    m_RTLightningBlur = dr_gfx_unique(dc.createRenderTarget(vecTextures));

    texDescDefault.bindFlags &= ~DR_BIND_FLAGS::UNORDERED_ACCESS;

    vecTextures.clear();
    ColorBlurTexure.release();
  }

  //Bloom
  {
    texDescDefault.width = static_cast<UInt32>(screenWidth);
    texDescDefault.height = static_cast<UInt32>(screenHeight);
    texDescDefault.Format = DR_FORMAT::kR16G16B16A16_FLOAT;
    texDescDefault.pitch = texDescDefault.width * 4 * 1;
    texDescDefault.bindFlags |= DR_BIND_FLAGS::UNORDERED_ACCESS;

    GFXUnique<Texture> BrightnessTexure = dr_gfx_unique<Texture>(dc.createEmptyTexture(texDescDefault));
    vecTextures.push_back(BrightnessTexure.get());
    m_RTBrightness = dr_gfx_unique(dc.createRenderTarget(vecTextures));

    texDescDefault.bindFlags &= ~DR_BIND_FLAGS::UNORDERED_ACCESS;

    vecTextures.clear();
    BrightnessTexure.release();

    //RenderTarget
    texDescDefault.width = static_cast<UInt32>(screenWidth * blurScale);
    texDescDefault.height = static_cast<UInt32>(screenHeight * blurScale);
    texDescDefault.Format = DR_FORMAT::kR16G16B16A16_FLOAT;
    texDescDefault.pitch = texDescDefault.width * 4 * 1;
    texDescDefault.bindFlags |= DR_BIND_FLAGS::UNORDERED_ACCESS;

    GFXUnique<Texture> BloomTexure = dr_gfx_unique<Texture>(dc.createEmptyTexture(texDescDefault));
    vecTextures.push_back(BloomTexure.get());
    m_RTBloom = dr_gfx_unique(dc.createRenderTarget(vecTextures));

    texDescDefault.bindFlags &= ~DR_BIND_FLAGS::UNORDERED_ACCESS;

    vecTextures.clear();
    BloomTexure.release();

  }

  //PostProcessing
  {
  }

  ////////initialization of passes////////
  m_GBufferPass.init(&m_GBufferInitData);

  m_LinesPass.init(&m_LinesInitData);

  m_ShadowInitData.RTWidht = shadowWidth;
  m_ShadowInitData.RTHeight = shadowHeight;
  m_ShadowPass.init(&m_ShadowInitData);
  
  m_SSAOInitData.RTWidth = screenWidth;
  m_SSAOInitData.RTHeight = screenHeight;
  m_SSAOPass.init(&m_SSAOInitData);

  m_bSSAO = true;
  m_fSSAOSampleRadio = 0.0008f;
  m_fSSAOIntensity = 2.0f;
  m_fSSAOScale = 1.0f;
  m_fSSAOBias = 0.0002f;

  m_HorBlurPass.init(&m_HorBlurInitData);
  m_VerBlurPass.init(&m_VerBlurInitData);

  m_SSReflectionPass.init(&m_SSReflectionInitData);

  m_fEnviromentScale = 1.0f;
  m_fIrradianceScale = 1.0f;

  m_LightningInitData.RTWidth = screenWidth;
  m_LightningInitData.RTHeight = screenHeight;
  m_LightningPass.init(&m_LightningInitData);

  m_BloomInitData.RTWidth = screenWidth;
  m_BloomInitData.RTHeight = screenHeight;
  m_BloomPass.init(&m_BloomInitData);

  m_luminescenceInitData.RTWidth = screenWidth;
  m_luminescenceInitData.RTHeight = screenHeight;
  m_luminescencePass.init(&m_luminescenceInitData);

  m_PostProcessingPass.init(&m_PostProcessingInitData);
  m_fChromaticAberrationStrenght = 0.125f;
  m_bFrontFocus = true;
  m_fFocusDistance = 390.0f;
  m_fFocusRange = 300.0f;
  m_fVignetteScale = 1.0f;
  m_vec2VignetteConcentration = Vector2D(4.0f, 4.0f);
  m_vec2VignetteRad = Vector2D(1.25f, 1.25f);

//  m_particlePass.init(&m_particleInitData);
//  ParticleEmitter emitter;
//  ParticleEmitterAttributes attr;
//  //System
//#if (DR_PARTICLES_METHOD == DR_PARTICLES_GPU)
//  attr.m_maxParticles = ParticleEmitter::MAX_PARTICLES;
//  attr.m_initialTime = 0.0;
//  attr.m_rate = 0.1;
//  attr.m_systemMaxLife = 500000;
//  attr.m_particleMaxLife = 15;
//  attr.m_numParticlesToEmit = 5000;
//  attr.m_isActive = true;
//  m_emitter.init(attr);
//#else
//  attr.m_maxParticles = ParticleEmitter::MAX_PARTICLES;
//  attr.m_initialTime = 0.1;
//  attr.m_rate = 0.1;
//  attr.m_systemMaxLife = 5000;
//  attr.m_particleMaxLife = 2;
//  attr.m_numParticlesToEmit = 5000;
//  attr.m_isActive = true;
//
//#endif
//  //Generators
//  m_emitter.getGenerator<BoxGenerator>(ParticleEmitter::GENERATORS::kBOX).m_bActive = true;
//  m_emitter.getGenerator<BoxGenerator>(ParticleEmitter::GENERATORS::kBOX).m_initialPositionRandomMin = Vector3D(-1500, 0,-1500);
//  m_emitter.getGenerator<BoxGenerator>(ParticleEmitter::GENERATORS::kBOX).m_initialPositionRandomMax = Vector3D(1500, 1500, 1500);
//
//  m_emitter.getGenerator<RandomVelocityGenerator>(ParticleEmitter::GENERATORS::kRANDOM_VELOCITY).m_bActive = false;
//  m_emitter.getGenerator<RandomVelocityGenerator>(ParticleEmitter::GENERATORS::kRANDOM_VELOCITY).m_initialVelocityRandomMin = Vector3D(-0, 500, -0);
//  m_emitter.getGenerator<RandomVelocityGenerator>(ParticleEmitter::GENERATORS::kRANDOM_VELOCITY).m_initialVelocityRandomMax = Vector3D(0, 200, 0);
//
//  //Updaters
//  m_emitter.getUpdater<TimeColorUpdater>(ParticleEmitter::UPDATERS::kTIME_COLOR).m_bActive = true;
//  m_emitter.getUpdater<TimeColorUpdater>(ParticleEmitter::UPDATERS::kTIME_COLOR).m_initialColor = Vector3D(1, 0.0274509804, 0.22745098);
//  m_emitter.getUpdater<TimeColorUpdater>(ParticleEmitter::UPDATERS::kTIME_COLOR).m_finalColor = Vector3D(1, 0.8, 0.8);
//
//  m_emitter.getUpdater<TimeScaleUpdater>(ParticleEmitter::UPDATERS::kTIME_SCALE).m_bActive = true;
//  m_emitter.getUpdater<TimeScaleUpdater>(ParticleEmitter::UPDATERS::kTIME_SCALE).m_initialScale = 1;
//  m_emitter.getUpdater<TimeScaleUpdater>(ParticleEmitter::UPDATERS::kTIME_SCALE).m_finaleScale = 1;
//
//  m_emitter.getUpdater<EulerUpdater>(ParticleEmitter::UPDATERS::kEULER).m_bActive = true;
//  m_emitter.getUpdater<EulerUpdater>(ParticleEmitter::UPDATERS::kEULER).m_windForce = Vector3D(0, 0, 0);
//  m_emitter.getUpdater<EulerUpdater>(ParticleEmitter::UPDATERS::kEULER).m_gravityScale = 10;
//
//  m_emitter.getUpdater<AttractorUpdater>(ParticleEmitter::UPDATERS::kATTRACTORS).m_bActive = true;
//  m_emitter.getUpdater<AttractorUpdater>(ParticleEmitter::UPDATERS::kATTRACTORS).add(Vector3D(0,-0, 0),1000, 1500);
//  m_emitter.getUpdater<AttractorUpdater>(ParticleEmitter::UPDATERS::kATTRACTORS).add(Vector3D(0, -0, 0), -1000, 100);
//
//  m_emitter.getUpdater<VortexUpdater>(ParticleEmitter::UPDATERS::kVORTEX).m_bActive = true;
//  m_emitter.getUpdater<VortexUpdater>(ParticleEmitter::UPDATERS::kVORTEX).add(Vector3D(0, -0, 0), Vector3D(0, 1, 0), 3000, 800);
//  m_emitter.getUpdater<VortexUpdater>(ParticleEmitter::UPDATERS::kVORTEX).add(Vector3D(0, -0, 0), Vector3D(0, 1, 0), 1000, 2000);
//
//  m_emitter.getUpdater<PlaneColliderUpdater>(ParticleEmitter::UPDATERS::kPLANE_COLLISION).m_bActive = true;
//  m_emitter.getUpdater<PlaneColliderUpdater>(ParticleEmitter::UPDATERS::kPLANE_COLLISION).m_normal = Vector3D(0,1,0);
//  m_emitter.getUpdater<PlaneColliderUpdater>(ParticleEmitter::UPDATERS::kPLANE_COLLISION).m_point = Vector3D(0, 0, 0);
//  m_emitter.getUpdater<PlaneColliderUpdater>(ParticleEmitter::UPDATERS::kPLANE_COLLISION).m_k = 0;
}

void
RenderManager::draw(const RenderTarget& _out, const DepthStencil& _outds) {
  DeviceContext& dc = GraphicsAPI::getDeviceContext();

  //Timer chrono;
  //chrono.init();
  //float currentTime = chrono.getMicroseconds();


  auto mainCam = CameraManager::instance().getActiveCamera();
  auto mainCamRef = *CameraManager::instance().getActiveCamera();

  RenderQuery rqRequest{ mainCamRef,
                        QUERY_ORDER::kFrontToBack,
                        QUERY_PROPERTY::kOpaque |
                        QUERY_PROPERTY::kDynamic |
                        QUERY_PROPERTY::kStatic };
  RenderCommandBuffer queryRequest = SceneGraph::query(rqRequest);
  //system("CLS");
  //benchmark(chrono, currentTime, "GBuffer Query: ");

  m_GBufferDrawData.activeCam = mainCam;
  m_GBufferDrawData.models = &queryRequest;
  m_GBufferDrawData.OutRt = m_RTGBuffer.get();
  m_GBufferDrawData.dsOptions = m_GBufferDSoptions.get();
  m_GBufferDrawData.skysphere = &(*m_SkySphere);
  m_GBufferDrawData.cubeMapTex = m_cubemap.get();
  m_GBufferPass.draw(&m_GBufferDrawData);
  //benchmark(chrono, currentTime, "GBuffer Pass: ");

  m_LinesDrawData.activeCam = mainCam;
  m_LinesDrawData.dsOptions = m_GBufferDSoptions.get();
  m_LinesDrawData.GBufferRT = m_RTGBuffer.get();
  m_LinesPass.draw(&m_LinesDrawData);
  //benchmark(chrono, currentTime, "Lines Pass: ");

  static const float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
  m_RTSSAO_SSShadow->clear(dc, white);

  //Directional lights
  std::memset(&DirectionalLights[0], -1, RM_MAX_DIRECTIONAL_LIGHTS);
  std::vector<DirectionalLightComponent*> allDLights = SceneGraph::instance().getDirectionalLightComponents();
  SizeT numberOfDLights = allDLights.size();
  numberOfDLights = Math::clamp(numberOfDLights, 0u, static_cast<SizeT>(RM_MAX_DIRECTIONAL_LIGHTS));
  for (SizeT current = 0; current < numberOfDLights; ++current) {
    DirectionalLights[current].m_vec4Direction = allDLights[current]->GetDirectionShadow();
    DirectionalLights[current].m_vec4Color = allDLights[current]->GetColorIntensity();
  }

  for (SizeT current = 0; current < numberOfDLights; ++current) {
    m_RTShadow->clear(dc, white);
    if (DirectionalLights[current].m_vec4Direction.w > 0.5f) {
      updateShadowCameras(DirectionalLights[current].m_vec4Direction);

      m_ShadowDrawData.dsOptions = m_ShadowDSoptions.get();
      for (SizeT camIndex = 0; camIndex < m_szActiveShadowCameras; ++camIndex) {
        rqRequest.camera = *vecShadowCamera[camIndex];
        queryRequest = SceneGraph::query(rqRequest);
        m_ShadowDrawData.shadowCam = vecShadowCamera[camIndex].get();
        m_ShadowDrawData.models = &queryRequest;
        m_ShadowDrawData.OutRt = m_RTShadowDummy[camIndex].get();
        m_ShadowPass.draw(&m_ShadowDrawData);
      }
      m_ShadowPass.merge(m_RTShadowDummy, m_RTShadow.get());

      m_ShadowDrawData.ShadowCameras = &vecShadowCamera;
      m_ShadowDrawData.ShadowSliptDepths = partitions;
      m_ShadowDrawData.ActivatedShadowCascades = m_szActiveShadowCameras;
      m_ShadowDrawData.ShadowMapTextureSize = shadowWidth;
      m_ShadowDrawData.LerpBetweenShadowCascade = 0.3f;
      m_ShadowDrawData.ShadowSizesProportion[0] = 1.0f;
      m_ShadowDrawData.ShadowSizesProportion[1] = m_ShadowSubFrustras[1].second /
                                                  m_ShadowSubFrustras[0].second;
      m_ShadowDrawData.ShadowSizesProportion[2] = m_ShadowSubFrustras[2].second /
                                                  m_ShadowSubFrustras[0].second;
      m_ShadowDrawData.ShadowSizesProportion[3] = m_ShadowSubFrustras[3].second /
                                                  m_ShadowSubFrustras[0].second;

      m_ShadowPass.apply(&m_ShadowDrawData, m_RTGBuffer.get(), m_RTShadow.get(), m_RTSSAO_SSShadow.get());
    }
  }
  //benchmark(chrono, currentTime, "Cascades Shadow Pass: ");

  if (m_bSSAO) {
    m_SSAODrawData.activeCam = mainCam;
    m_SSAODrawData.InRt = m_RTGBuffer.get();
    m_SSAODrawData.OutRt = m_RTSSAO_SSShadow.get();
    m_SSAODrawData.SampleRadio = m_fSSAOSampleRadio;
    m_SSAODrawData.Intensity = m_fSSAOIntensity;
    m_SSAODrawData.Scale = m_fSSAOScale;
    m_SSAODrawData.Bias = m_fSSAOBias;
    m_SSAOPass.draw(&m_SSAODrawData);
    //benchmark(chrono, currentTime, "SSAO Pass: ");
  }

  m_HorBlurDrawData.InTexture = &m_RTSSAO_SSShadow->getTexture(0);
  m_HorBlurDrawData.OutRt = m_RTBlurInit.get();
  m_HorBlurPass.draw(&m_HorBlurDrawData);
  //benchmark(chrono, currentTime, "Horizontal Blur Pass: ");

  m_VerBlurDrawData.InTexture = &m_RTBlurInit->getTexture(0);
  m_VerBlurDrawData.OutRt = m_RTSSAO_SSShadowBlur.get();
  m_VerBlurPass.draw(&m_VerBlurDrawData);
  //benchmark(chrono, currentTime, "Vertical Blur Pass: ");

  m_SSReflectionDrawData.ActiveCam = mainCam;
  m_SSReflectionDrawData.GbufferRT = m_RTGBuffer.get();
  m_SSReflectionDrawData.ColorRT = m_RTLightning.get();
  m_SSReflectionDrawData.OutRt = m_RTSSReflection.get();
  m_SSReflectionPass.draw(&m_SSReflectionDrawData);
  //benchmark(chrono, currentTime, "Reflections Pass: ");

  //Point lights
  std::memset(&PointLights[0], -1, RM_MAX_POINT_LIGHTS);
  std::vector<PointLightComponent*> allPLights = SceneGraph::instance().getPointLightComponents();
  SizeT numberOfPLights = allPLights.size();
  numberOfPLights = Math::clamp(numberOfPLights, 0u, static_cast<SizeT>(RM_MAX_POINT_LIGHTS));
  for (SizeT current = 0; current < numberOfPLights; ++current) {
    PointLights[current].m_vec4Position = allPLights[current]->GetPositionRange();
    PointLights[current].m_vec4Color = allPLights[current]->GetColorIntensity();
  }

  //Transform Lights to ScreenSpace.
  m_LWSLightsToSSData.ActiveCam = mainCam;
  m_LWSLightsToSSData.Lights = &PointLights;
  m_LWSLightsToSSData.numberOfLights = numberOfPLights;
  m_LightningPass.lightsToScreenSpace(&m_LWSLightsToSSData);
  //benchmark(chrono, currentTime, "Lights to SS Pass: ");

  //Tile Lights
  m_LTileLightsData.OutRt = m_RTLightning.get();
  m_LTileLightsData.numberOfLights = numberOfPLights;
  m_LightningPass.tileLights(&m_LTileLightsData);
  //benchmark(chrono, currentTime, "Tile lights Pass: ");

  //Lightning Pass
  m_LightningDrawData.ActiveCam = mainCam;
  m_LightningDrawData.PLights = &PointLights;
  m_LightningDrawData.ActiveDirectionalLights = numberOfDLights;
  m_LightningDrawData.DLights = &DirectionalLights;
  m_LightningDrawData.GbufferRT = m_RTGBuffer.get();
  m_LightningDrawData.SSAO_SSShadowRT = m_RTSSAO_SSShadowBlur.get();
  m_LightningDrawData.SSReflection = m_RTSSReflection.get();
  m_LightningDrawData.OutRt = m_RTLightning.get();
  m_LightningDrawData.EnviromentCubemap = m_cubemap.get();
  m_LightningDrawData.EnviromentScale = &m_fEnviromentScale;
  m_LightningDrawData.IrradianceCubemap = m_cubemapDiffuse.get();
  m_LightningDrawData.IrradianceScale = &m_fIrradianceScale;
  m_LightningPass.draw(&m_LightningDrawData);
  //benchmark(chrono, currentTime, "Lightning Pass: ");

  m_luminescenceDrawData.InTexture = &m_RTLightning->getTexture(0);
  m_luminescenceDrawData.LuminiscenceDelta = m_fLuminanceDelta;
  m_luminescenceDrawData.resultBuffer = &resultBuffer;
  m_luminescencePass.draw(&m_luminescenceDrawData);
  //benchmark(chrono, currentTime, "Luminescence Pass: ");

  m_BloomDrawData.BloomThreshold = m_BloomThreshold;
  m_BloomDrawData.LuminiscenceDelta = m_fLuminanceDelta;
  m_BloomDrawData.ColorTexture = &m_RTLightning->getTexture(0);
  m_BloomDrawData.OutTex = &m_RTBrightness->getTexture(0);
  m_BloomPass.draw(&m_BloomDrawData);
  //benchmark(chrono, currentTime, "Bloom Pass: ");

  //Bloom Blur
  m_HorBlurDrawData.InTexture = &m_RTBrightness->getTexture(0);
  m_HorBlurDrawData.OutRt = m_RTBlurInit.get();
  m_HorBlurPass.draw(&m_HorBlurDrawData);
  //benchmark(chrono, currentTime, "Horizontal Blur Pass: ");

  m_VerBlurDrawData.InTexture = &m_RTBlurInit->getTexture(0);
  m_VerBlurDrawData.OutRt = m_RTBloom.get();
  m_VerBlurPass.draw(&m_VerBlurDrawData);
  //benchmark(chrono, currentTime, "Vertical Blur Pass: ");

  //DoF Blur
  m_HorBlurDrawData.InTexture = &m_RTLightning->getTexture(0);
  m_HorBlurDrawData.OutRt = m_RTBlurInit.get();
  m_HorBlurPass.draw(&m_HorBlurDrawData);
  //benchmark(chrono, currentTime, "Horizontal Blur Pass: ");

  m_VerBlurDrawData.InTexture = &m_RTBlurInit->getTexture(0);
  m_VerBlurDrawData.OutRt = m_RTLightningBlur.get();
  m_VerBlurPass.draw(&m_VerBlurDrawData);
  //benchmark(chrono, currentTime, "Vertical Blur Pass: ");

  //_out.set(dc, _outds);

  //m_emitter.update();
  //m_particleDrawData.activeCam = mainCam;
  //m_particleDrawData.emitter = &m_emitter;
  //m_particlePass.draw(&m_particleDrawData);

  //const float clearColor[4]{0.2f, 0.5f, 0.8f, 1.0f };
  //_out->clear(GraphicsAPI::getDeviceContext(), clearColor);

  m_PostProcessingDrawData.activeCam = mainCam;
  m_PostProcessingDrawData.ChromaticAberrationStrenght = m_fChromaticAberrationStrenght;
  m_PostProcessingDrawData.CoCFocusDistance = m_fFocusDistance;
  m_PostProcessingDrawData.CoCFocusRange = m_bFrontFocus ? m_fFocusRange : -m_fFocusRange;
  m_PostProcessingDrawData.VignetteScale = m_fVignetteScale;
  m_PostProcessingDrawData.VignetteConcentration = m_vec2VignetteConcentration;
  m_PostProcessingDrawData.VignetteRad = m_vec2VignetteRad;
  m_PostProcessingDrawData.ColorTex = &m_RTLightning->getTexture(0);
  //m_PostProcessingDrawData.ColorTex = &m_RTGBuffer->getTexture(2);
  m_PostProcessingDrawData.ColorBlurTex = &m_RTLightningBlur->getTexture(0);
  m_PostProcessingDrawData.PositionDepthTex = &m_RTGBuffer->getTexture(0);
  m_PostProcessingDrawData.BloomTex = &m_RTBloom->getTexture(0);
  m_PostProcessingDrawData.FilmLutTex = m_FilmLut.get();
  m_PostProcessingDrawData.luminescenceBuffer = resultBuffer;
  m_PostProcessingDrawData.OutRT = &_out;
  m_PostProcessingDrawData.OutDS = &_outds;
  m_PostProcessingPass.draw(&m_PostProcessingDrawData);
  //benchmark(chrono, currentTime, "Post Processing Pass: ");

  /*
  ./ GBuffer:
  ./ SSAO:
  ./ ShadowCascades: 
   X Diffuse Acumulation: Fog
   X Specular Acumulation: SS Reflection
   X Lights y ZSkips:
   X Opacity: Blends
  */
}

void
RenderManager::exit() {
}

void
RenderManager::recompile() {
  m_GBufferPass.recompileShader();
  m_SSAOPass.recompileShader();
  m_HorBlurPass.recompileShader();
  m_VerBlurPass.recompileShader();
  m_ShadowPass.recompileShader();
  m_SSReflectionPass.recompileShader();
  m_LightningPass.recompileShader();
  m_luminescencePass.recompileShader();
  m_PostProcessingPass.recompileShader();
}

void
RenderManager::onStartUp() {
  init();
}

void
RenderManager::onShutDown() {
  exit();
}

void
RenderManager::setSkySphere(std::shared_ptr<Model> skysphere) {
  m_SkySphere = skysphere;
}

void
RenderManager::setCubeMap(std::shared_ptr<TextureCore> cubemap) {
  m_cubemap = cubemap;
}

void
RenderManager::setEnviromentMap(std::shared_ptr<TextureCore> enviromentmap) {
  m_cubemapDiffuse = enviromentmap;
}

void
RenderManager::setFilmLut(std::shared_ptr<TextureCore> filmLut) {
  m_FilmLut = filmLut;
}

void
RenderManager::drawDebugLine(const Vector3D& start,
                             const Vector3D& end,
                             const Vector3D& color,
                             const Matrix4x4& transform) {
#ifdef DR_DEBUG_MODE
  m_LinesPass.addLineToQueue(start, end, color, transform);
#endif // DR_DEBUG_MODE

}

void
RenderManager::drawDebugLine(const std::vector<Vector3D>& points,
                             const Vector3D& color,
                             const Matrix4x4& transform) {
#ifdef DR_DEBUG_MODE
  m_LinesPass.addStripLineToQueue(points, color, transform);
#endif // DR_DEBUG_MODE

}

void
RenderManager::drawDebugCube(const AABB& cube,
                             const Vector3D& color,
                             const Matrix4x4& transform) {
#ifdef DR_DEBUG_MODE
  std::vector<Vector3D> square;

  auto min = cube.getMinPoint();

  square.push_back(min);
  square.push_back(min + Vector3D(cube.width, 0, 0));
  square.push_back(min + Vector3D(cube.width, 0, cube.depth));
  square.push_back(min + Vector3D(0, 0, cube.depth));
  square.push_back(min);

  m_LinesPass.addStripLineToQueue(square, color, transform);

  for (auto& it : square) {
    it.y += cube.height;
  }
  m_LinesPass.addStripLineToQueue(square, color, transform);
  
  m_LinesPass.addLineToQueue(min,
                             min + Vector3D(0, cube.height, 0),
                             color,
                             transform);

  auto max = cube.getMaxPoint();

  m_LinesPass.addLineToQueue(max,
                             max - Vector3D(0, cube.height, 0),
                             color,
                             transform);

  m_LinesPass.addLineToQueue(max - Vector3D(cube.width, 0, 0),
                             max - Vector3D(cube.width, cube.height, 0),
                             color,
                             transform);
  m_LinesPass.addLineToQueue(max - Vector3D(0, 0, cube.depth),
                             max - Vector3D(0, cube.height, cube.depth),
                             color,
                             transform);
#endif // DR_DEBUG_MODE
}

void
RenderManager::drawDebugSphere(const float radius,
                               const Vector3D& color,
                               const Matrix4x4& transform) {
#ifdef DR_DEBUG_MODE
  SizeT verticalDivs = 8;
  SizeT horizontalDivs = 16;

  Matrix4x4 verticalRotation = Matrix4x4::identityMat4x4,
            horizontalRotation = Matrix4x4::identityMat4x4;
  {
    Radian verRads(Math::PI / (float)verticalDivs),
           horRads(Math::TWO_PI / (float)horizontalDivs);

    verticalRotation.RotationX(verRads);
    horizontalRotation.RotationY(horRads);
  }

  //Auxiliar points
  std::vector<Vector3D> halfCircle;
  {
    Vector3D refPoint = Vector3D(0, radius, 0);
    for (SizeT i = 0; i < verticalDivs; ++i) {
      halfCircle.push_back(refPoint);
      refPoint = verticalRotation * refPoint;
    }
    halfCircle.push_back(refPoint);
  }

  //Vertical circles
  {
    std::vector<Vector3D> verticalCircle = halfCircle;

    for (SizeT i = 0; i < horizontalDivs; ++i) {
      for (auto& currentVertex : verticalCircle) {
        currentVertex = horizontalRotation * currentVertex;
      }
      m_LinesPass.addStripLineToQueue(verticalCircle, color, transform);
    }
  }

  //Horizontal divisions
  {
    std::vector<Vector3D> horizontalCircle;
    Vector3D horRef;

    for (SizeT v = 1; v < verticalDivs; ++v) {
      horRef = halfCircle[v];
      for (SizeT h = 0; h < (horizontalDivs + 1); ++h) {
        horizontalCircle.push_back(horRef);
        horRef = horizontalRotation * horRef;
      }
      m_LinesPass.addStripLineToQueue(horizontalCircle, color, transform);
      horizontalCircle.clear();
    }
  }

#endif
}

void
RenderManager::updateShadowCameras(const Vector4D lightDir) {
  Vector3D lightDir3;
  lightDir3 = lightDir;

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
    
    TrueCenter = CamPos + (CamDir * m_ShadowSubFrustras[i].first);

    vecShadowCamera[i]->setPosition(TrueCenter +
                                    (lightDir3 * m_fMaxDepth));
    vecShadowCamera[i]->setTarget(TrueCenter);
    vecShadowCamera[i]->createProyection(Math::floor(SphereRad * 2.0f),
                                         Math::floor(SphereRad * 2.0f),
                                         m_fMinDepth,
                                         m_fMaxDepth + extraDepth);
  }
}

std::vector<float>
RenderManager::calculatePartitions(SizeT cuts) {
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

    //fLinearValue = Math::pow(fProportion, 3.0f);
    //fPwValue = Math::pow(fProportion, 1.5f);
    //fRealValue = Math::lerp(fLinearValue, fPwValue, fProportion);
    
    //realValues.push_back(fLinearValue);
    //realValues.push_back(fPwValue);
    realValues.push_back(fRealValue);
  }
   return realValues;
}

std::pair<float, float>
RenderManager::frustrumSphere(float fVW,
                              float fVH,
                              float fNP,
                              float fFP,
                              float fF) {
  float fCenter;
  float fRadius;
  float fK = Math::sqrt(1.0f + ((fVH * fVH) / (fVW * fVW))) * Math::tan(Math::DEGREE_TO_RADIAN * fF * 0.5f);
  
  if ((fK * fK) < ((fFP - fNP) / (fFP + fNP))) {
    fCenter = 0.5f * (fFP + fNP) * (1.0f + (fK * fK));
    fRadius = 0.5f * Math::sqrt((Math::pow((fFP - fNP), 2.0f)) +
                                (2.0f * ((fFP * fFP) + (fNP * fNP)) * (fK * fK)) +
                                (Math::pow(fFP + fNP, 2.0f) * Math::pow(fK, 4.0f)));
  }
  else {
    fCenter = fFP;
    fRadius = fFP * fK;
  }

  return std::pair<float, float>(fCenter, fRadius);
}

}