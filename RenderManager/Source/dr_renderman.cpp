#include "dr_renderman.h"
#include <dr_graphics_driver.h>
#include <dr_device.h>
#include <dr_texture.h>
#include <dr_resource_manager.h>
#include <dr_image_info.h>
#include <dr_camera_manager.h>
#include <dr_depth_stencil.h>

#include <dr_texture_core.h>

namespace driderSDK {

RenderManager::RenderManager() {
}

RenderManager::~RenderManager() {
}

void
RenderManager::init() {
  Device& dc = GraphicsAPI::getDevice();

  ResourceManager::loadResource(_T("ScreenAlignedQuad.3ds"));

  ImageInfo cubeMapDesc;
  cubeMapDesc.width = 256;
  cubeMapDesc.height = 256;
  cubeMapDesc.textureDimension = DR_DIMENSION::kCUBE_MAP;
  cubeMapDesc.channels = DR_FORMAT::kB8G8R8A8_UNORM_SRGB;

  ResourceManager::loadResource(_T("GraceCubemap.tga"), &cubeMapDesc);
  m_cubemap = ResourceManager::getReferenceT<TextureCore>(_T("GraceCubemap.tga"));
  ResourceManager::loadResource(_T("GraceDiffuseCubemap.tga"), &cubeMapDesc);
  m_cubemapDiffuse = ResourceManager::getReferenceT<TextureCore>(_T("GraceDiffuseCubemap.tga"));


  ///////////Resolutions///////////
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

  shadowWidth = 1024;
  shadowHeight = 1024;

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

  //m_vec3DirectionalLight = Vector3D(-1.0f, -1.0f, 0.0f).normalize();
  //m_vec3DirectionalLight = Vector3D(0.0f, -10000.0f, 0.1f).normalize();
  m_vec3DirectionalLight = Vector3D(1.0f, -1.0f, -1.0f).normalize();

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

  /////////Creation of RT's & DS's/////////
  float blurScale = 0.5f;
  //RenderTarget Base
  {
    m_TexDescDefault.dimension = DR_DIMENSION::k2D;
    m_TexDescDefault.width = screenWidth;
    m_TexDescDefault.height = screenHeight;
    m_TexDescDefault.Format = DR_FORMAT::kR16G16B16A16_FLOAT;
    m_TexDescDefault.pitch = m_TexDescDefault.width * 4 * 2;
    m_TexDescDefault.mipLevels = 0;
    m_TexDescDefault.CPUAccessFlags = 0;
    m_TexDescDefault.genMipMaps = true;
    m_TexDescDefault.bindFlags = DR_BIND_FLAGS::SHADER_RESOURCE |
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

  //Shadows
  {
    //RenderTarget
    m_TexDescDefault.width = shadowWidth;
    m_TexDescDefault.height = shadowHeight;
    m_TexDescDefault.Format = DR_FORMAT::kR32G32B32A32_FLOAT;
    m_TexDescDefault.pitch = m_TexDescDefault.width * 4 * 4;
    m_TexDescDefault.bindFlags |= DR_BIND_FLAGS::UNORDERED_ACCESS;

    Texture* ShadowTexure = dc.createEmptyTexture(m_TexDescDefault);
    m_vecTexture.push_back(ShadowTexure);

    m_RTShadow = dr_gfx_shared(dc.createRenderTarget(m_vecTexture));

    m_vecTexture.clear();
    m_TexDescDefault.bindFlags &= ~DR_BIND_FLAGS::UNORDERED_ACCESS;

    m_TexDescDefault.Format = DR_FORMAT::kR32_FLOAT;
    m_TexDescDefault.pitch = m_TexDescDefault.width * 1 * 4;
    m_RTShadowDummy[0] = dr_gfx_shared(dc.createRenderTarget(m_TexDescDefault, 1));
    m_RTShadowDummy[1] = dr_gfx_shared(dc.createRenderTarget(m_TexDescDefault, 1));
    m_RTShadowDummy[2] = dr_gfx_shared(dc.createRenderTarget(m_TexDescDefault, 1));
    m_RTShadowDummy[3] = dr_gfx_shared(dc.createRenderTarget(m_TexDescDefault, 1));

    //DepthStencil
    commonTextureDesc.width = shadowWidth;
    commonTextureDesc.height = shadowHeight;
    m_ShadowDSoptions = dr_gfx_shared(dc.createDepthStencil(commonTextureDesc));
  }
  
  //Gbuffer
  {
    //RenderTarget
    m_TexDescDefault.width = screenWidth;
    m_TexDescDefault.height = screenHeight;
    m_TexDescDefault.Format = DR_FORMAT::kR32G32B32A32_FLOAT;
    m_TexDescDefault.pitch = m_TexDescDefault.width * 4 * 4;

    GFXUnique<Texture> PositionTexure = dr_gfx_unique<Texture>(dc.createEmptyTexture(m_TexDescDefault));
    m_vecTexture.push_back(PositionTexure.get());
    m_vecTexture.push_back(PositionTexure.get());

    m_TexDescDefault.Format = DR_FORMAT::kR16G16B16A16_FLOAT;
    m_TexDescDefault.pitch = m_TexDescDefault.width * 4 * 2;

    GFXUnique<Texture> BasicTexure = dr_gfx_unique<Texture>(dc.createEmptyTexture(m_TexDescDefault));
    m_vecTexture.push_back(BasicTexure.get());
    m_vecTexture.push_back(BasicTexure.get());

    m_RTGBuffer = dr_gfx_shared(dc.createRenderTarget(m_TexDescDefault, 4));

    m_vecTexture.clear();
    PositionTexure.release();
    BasicTexure.release();

    //DepthStencil
    commonTextureDesc.width = screenWidth;
    commonTextureDesc.height = screenHeight;
    m_GBufferDSoptions = dr_gfx_shared(dc.createDepthStencil(commonTextureDesc));
  }

  //SSAO
  {
    //RenderTarget
    m_TexDescDefault.width = screenWidth;
    m_TexDescDefault.height = screenHeight;
    m_TexDescDefault.Format = DR_FORMAT::kR8_UNORM;
    m_TexDescDefault.pitch = m_TexDescDefault.width * 1 * 1;
    m_TexDescDefault.bindFlags |= DR_BIND_FLAGS::UNORDERED_ACCESS;

    GFXUnique<Texture> SSAOTexure = dr_gfx_unique<Texture>(dc.createEmptyTexture(m_TexDescDefault));
    m_vecTexture.push_back(SSAOTexure.get());
    m_RTSSAO = dr_gfx_shared(dc.createRenderTarget(m_vecTexture));

    m_TexDescDefault.bindFlags &= ~DR_BIND_FLAGS::UNORDERED_ACCESS;

    m_vecTexture.clear();
    SSAOTexure.release();
  }

  //Blur Aux
  {
    //RenderTarget
    m_TexDescDefault.width = screenWidth * blurScale;
    m_TexDescDefault.height = screenHeight * blurScale;
    m_TexDescDefault.Format = DR_FORMAT::kR16G16B16A16_FLOAT;
    m_TexDescDefault.pitch = m_TexDescDefault.width * 4 * 2;
    m_TexDescDefault.bindFlags |= DR_BIND_FLAGS::UNORDERED_ACCESS;

    GFXUnique<Texture> BlurTexure = dr_gfx_unique<Texture>(dc.createEmptyTexture(m_TexDescDefault));
    m_vecTexture.push_back(BlurTexure.get());
    m_RTBlurInit = dr_gfx_shared(dc.createRenderTarget(m_vecTexture));

    m_TexDescDefault.bindFlags &= ~DR_BIND_FLAGS::UNORDERED_ACCESS;

    m_vecTexture.clear();
    BlurTexure.release();
  }

  //Blurred SSAO
  {
    //RenderTarget
    m_TexDescDefault.width = screenWidth * blurScale;
    m_TexDescDefault.height = screenHeight * blurScale;
    m_TexDescDefault.Format = DR_FORMAT::kR16_FLOAT;
    m_TexDescDefault.pitch = m_TexDescDefault.width * 1 * 1;
    m_TexDescDefault.bindFlags |= DR_BIND_FLAGS::UNORDERED_ACCESS;

    GFXUnique<Texture> SSAOBlurTexure = dr_gfx_unique<Texture>(dc.createEmptyTexture(m_TexDescDefault));
    m_vecTexture.push_back(SSAOBlurTexure.get());
    m_RTSSAOBlur = dr_gfx_shared(dc.createRenderTarget(m_vecTexture));

    m_TexDescDefault.bindFlags &= ~DR_BIND_FLAGS::UNORDERED_ACCESS;

    m_vecTexture.clear();
    SSAOBlurTexure.release();
  }

  //Lightning & LightningBlur
  {
    //RenderTarget
    m_TexDescDefault.width = screenWidth;
    m_TexDescDefault.height = screenHeight;
    m_TexDescDefault.Format = DR_FORMAT::kR16G16B16A16_FLOAT;
    m_TexDescDefault.pitch = m_TexDescDefault.width * 4 * 2;

    m_RTLightning = dr_gfx_shared(dc.createRenderTarget(m_TexDescDefault, 2));

    m_TexDescDefault.width = screenWidth * blurScale;
    m_TexDescDefault.height = screenHeight * blurScale;
    m_TexDescDefault.Format = DR_FORMAT::kR16G16B16A16_FLOAT;
    m_TexDescDefault.pitch = m_TexDescDefault.width * 4 * 2;
    m_TexDescDefault.bindFlags |= DR_BIND_FLAGS::UNORDERED_ACCESS;

    GFXUnique<Texture> ColorBlurTexure = dr_gfx_unique<Texture>(dc.createEmptyTexture(m_TexDescDefault));
    m_vecTexture.push_back(ColorBlurTexure.get());
    m_RTLightningBlur = dr_gfx_shared(dc.createRenderTarget(m_vecTexture));

    m_TexDescDefault.bindFlags &= ~DR_BIND_FLAGS::UNORDERED_ACCESS;

    m_vecTexture.clear();
    ColorBlurTexure.release();

    //DepthStencil
    commonTextureDesc.width = screenWidth;
    commonTextureDesc.height = screenHeight;
    m_LightningDSoptions = dr_gfx_shared(dc.createDepthStencil(commonTextureDesc));
  }

  //Bloom
  {
    //RenderTarget
    m_TexDescDefault.width = screenWidth * blurScale;
    m_TexDescDefault.height = screenHeight * blurScale;
    m_TexDescDefault.Format = DR_FORMAT::kR16G16B16A16_FLOAT;
    m_TexDescDefault.pitch = m_TexDescDefault.width * 4 * 1;
    m_TexDescDefault.bindFlags |= DR_BIND_FLAGS::UNORDERED_ACCESS;

    GFXUnique<Texture> BloomTexure = dr_gfx_unique<Texture>(dc.createEmptyTexture(m_TexDescDefault));
    m_vecTexture.push_back(BloomTexure.get());
    m_RTBloom = dr_gfx_shared(dc.createRenderTarget(m_vecTexture));

    m_TexDescDefault.bindFlags &= ~DR_BIND_FLAGS::UNORDERED_ACCESS;

    m_vecTexture.clear();
    BloomTexure.release();
  }

  //PostProcessing
  {
    //DepthStencil
    m_PostProcessingDSoptions = dr_gfx_shared(dc.createDepthStencil(commonTextureDesc));
  }

  ////////initialization of passes////////
  m_ShadowInitData.RTWidht = shadowWidth;
  m_ShadowInitData.RTHeight = shadowHeight;
  m_ShadowPass.init(&m_ShadowInitData);

  m_GBufferPass.init(&m_GBufferInitData);

  m_SSAOInitData.RTWidth = screenWidth;
  m_SSAOInitData.RTHeight = screenHeight;
  m_SSAOPass.init(&m_SSAOInitData);

  m_HorBlurPass.init(&m_HorBlurInitData);
  m_VerBlurPass.init(&m_VerBlurInitData);

  m_LightningInitData.RTWidth = screenWidth;
  m_LightningInitData.RTHeight = screenHeight;
  m_LightningPass.init(&m_LightningInitData);

  m_luminescencePass.init(&m_luminescenceInitData);

  m_PostProcessingPass.init(&m_PostProcessingInitData);

  m_particlePass.init(&m_particleInitData);
  driderSDK::ParticleEmitter emitter;
  driderSDK::ParticleEmitterAttributes attr;
  //System
#if (DR_PARTICLES_METHOD == DR_PARTICLES_GPU)
  attr.m_maxParticles = ParticleEmitter::MAX_PARTICLES;
  attr.m_initialTime = 0.0;
  attr.m_rate = 0.1;
  attr.m_systemMaxLife = 500000;
  attr.m_particleMaxLife = 15;
  attr.m_numParticlesToEmit = 5000;
  attr.m_isActive = true;
  m_emitter.init(attr);
#else
  attr.m_maxParticles = ParticleEmitter::MAX_PARTICLES;
  attr.m_initialTime = 0.1;
  attr.m_rate = 0.1;
  attr.m_systemMaxLife = 5000;
  attr.m_particleMaxLife = 2;
  attr.m_numParticlesToEmit = 5000;
  attr.m_isActive = true;

#endif
  //Generators
  m_emitter.getGenerator<BoxGenerator>(ParticleEmitter::GENERATORS::kBOX).m_bActive = true;
  m_emitter.getGenerator<BoxGenerator>(ParticleEmitter::GENERATORS::kBOX).m_initialPositionRandomMin = Vector3D(-1500, 0,-1500);
  m_emitter.getGenerator<BoxGenerator>(ParticleEmitter::GENERATORS::kBOX).m_initialPositionRandomMax = Vector3D(1500, 1500, 1500);

  m_emitter.getGenerator<RandomVelocityGenerator>(ParticleEmitter::GENERATORS::kRANDOM_VELOCITY).m_bActive = false;
  m_emitter.getGenerator<RandomVelocityGenerator>(ParticleEmitter::GENERATORS::kRANDOM_VELOCITY).m_initialVelocityRandomMin = Vector3D(-0, 500, -0);
  m_emitter.getGenerator<RandomVelocityGenerator>(ParticleEmitter::GENERATORS::kRANDOM_VELOCITY).m_initialVelocityRandomMax = Vector3D(0, 200, 0);

  //Updaters
  m_emitter.getUpdater<TimeColorUpdater>(ParticleEmitter::UPDATERS::kTIME_COLOR).m_bActive = true;
  m_emitter.getUpdater<TimeColorUpdater>(ParticleEmitter::UPDATERS::kTIME_COLOR).m_initialColor = Vector3D(1, 0.0274509804, 0.22745098);
  m_emitter.getUpdater<TimeColorUpdater>(ParticleEmitter::UPDATERS::kTIME_COLOR).m_finalColor = Vector3D(1, 0.8, 0.8);

  m_emitter.getUpdater<TimeScaleUpdater>(ParticleEmitter::UPDATERS::kTIME_SCALE).m_bActive = true;
  m_emitter.getUpdater<TimeScaleUpdater>(ParticleEmitter::UPDATERS::kTIME_SCALE).m_initialScale = 1;
  m_emitter.getUpdater<TimeScaleUpdater>(ParticleEmitter::UPDATERS::kTIME_SCALE).m_finaleScale = 1;

  m_emitter.getUpdater<EulerUpdater>(ParticleEmitter::UPDATERS::kEULER).m_bActive = true;
  m_emitter.getUpdater<EulerUpdater>(ParticleEmitter::UPDATERS::kEULER).m_windForce = Vector3D(0, 0, 0);
  m_emitter.getUpdater<EulerUpdater>(ParticleEmitter::UPDATERS::kEULER).m_gravityScale = 10;

  m_emitter.getUpdater<AttractorUpdater>(ParticleEmitter::UPDATERS::kATTRACTORS).m_bActive = true;
  m_emitter.getUpdater<AttractorUpdater>(ParticleEmitter::UPDATERS::kATTRACTORS).add(Vector3D(0,-0, 0),1000, 1500);
  m_emitter.getUpdater<AttractorUpdater>(ParticleEmitter::UPDATERS::kATTRACTORS).add(Vector3D(0, -0, 0), -1000, 100);

  m_emitter.getUpdater<VortexUpdater>(ParticleEmitter::UPDATERS::kVORTEX).m_bActive = true;
  m_emitter.getUpdater<VortexUpdater>(ParticleEmitter::UPDATERS::kVORTEX).add(Vector3D(0, -0, 0), Vector3D(0, 1, 0), 3000, 800);
  m_emitter.getUpdater<VortexUpdater>(ParticleEmitter::UPDATERS::kVORTEX).add(Vector3D(0, -0, 0), Vector3D(0, 1, 0), 1000, 2000);

  m_emitter.getUpdater<PlaneColliderUpdater>(ParticleEmitter::UPDATERS::kPLANE_COLLISION).m_bActive = true;
  m_emitter.getUpdater<PlaneColliderUpdater>(ParticleEmitter::UPDATERS::kPLANE_COLLISION).m_normal = Vector3D(0,1,0);
  m_emitter.getUpdater<PlaneColliderUpdater>(ParticleEmitter::UPDATERS::kPLANE_COLLISION).m_point = Vector3D(0, 0, 0);
  m_emitter.getUpdater<PlaneColliderUpdater>(ParticleEmitter::UPDATERS::kPLANE_COLLISION).m_k = 0;
}

void
RenderManager::draw(const RenderTarget& _out, const DepthStencil& _outds) {
  updateShadowCameras();

  auto mainCam = CameraManager::getActiveCamera();
  SceneGraph::QueryResult queryRequest;

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

  queryRequest = SceneGraph::query(*mainCam,
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
  m_SSAODrawData.SampleRadio = 0.0008f;
  m_SSAODrawData.Intensity = 1.0F;
  m_SSAODrawData.Scale = 1.0f;
  m_SSAODrawData.Bias = 0.0002f;
  m_SSAOPass.draw(&m_SSAODrawData);

  m_HorBlurDrawData.InTexture = &m_RTSSAO->getTexture(0);
  m_HorBlurDrawData.OutRt = m_RTBlurInit;
  m_HorBlurPass.draw(&m_HorBlurDrawData);

  m_VerBlurDrawData.InTexture = &m_RTBlurInit->getTexture(0);
  m_VerBlurDrawData.OutRt = m_RTSSAOBlur;
  m_VerBlurPass.draw(&m_VerBlurDrawData);

  m_LightningDrawData.ActiveCam = mainCam;
  m_LightningDrawData.Lights = &lights[0];
  m_LightningDrawData.ActiveLights = 128;
  m_LightningDrawData.ShadowCameras = &vecShadowCamera;
  m_LightningDrawData.ShadowSliptDepths = partitions;
  m_LightningDrawData.ActivatedShadowCascades = m_szActiveShadowCameras;
  m_LightningDrawData.ShadowMapTextureSize = shadowWidth;
  m_LightningDrawData.LerpBetweenShadowCascade = 0.3f;
  m_LightningDrawData.BloomThreshold = Vector3D(0.75f, 0.75f, 0.75f);
  m_LightningDrawData.LuminiscenceDelta = 0.0f;
  m_LightningDrawData.ShadowSizesProportion[0] = 1.0f;
  m_LightningDrawData.ShadowSizesProportion[1] = m_ShadowSubFrustras[1].second /
                                                 m_ShadowSubFrustras[0].second;
  m_LightningDrawData.ShadowSizesProportion[2] = m_ShadowSubFrustras[2].second /
                                                 m_ShadowSubFrustras[0].second;
  m_LightningDrawData.ShadowSizesProportion[3] = m_ShadowSubFrustras[3].second /
                                                 m_ShadowSubFrustras[0].second;
  m_LightningDrawData.GbufferRT = m_RTGBuffer;
  m_LightningDrawData.SSAORT = m_RTSSAOBlur;
  m_LightningDrawData.ShadowRT = m_RTShadow;
  m_LightningDrawData.OutRt = m_RTLightning;
  m_LightningDrawData.EnviromentCubemap = m_cubemap;
  m_LightningDrawData.IrradianceCubemap = m_cubemapDiffuse;
  m_LightningDrawData.dsOptions = m_LightningDSoptions;
  m_LightningPass.draw(&m_LightningDrawData);

  //DoF
  m_HorBlurDrawData.InTexture = &m_RTLightning->getTexture(0);
  m_HorBlurDrawData.OutRt = m_RTBlurInit;
  m_HorBlurPass.draw(&m_HorBlurDrawData);

  m_VerBlurDrawData.InTexture = &m_RTBlurInit->getTexture(0);
  m_VerBlurDrawData.OutRt = m_RTLightningBlur;
  m_VerBlurPass.draw(&m_VerBlurDrawData);

  //Bloom
  m_HorBlurDrawData.InTexture = &m_RTLightning->getTexture(1);
  m_HorBlurDrawData.OutRt = m_RTBlurInit;
  m_HorBlurPass.draw(&m_HorBlurDrawData);

  m_VerBlurDrawData.InTexture = &m_RTBlurInit->getTexture(0);
  m_VerBlurDrawData.OutRt = m_RTBloom;
  m_VerBlurPass.draw(&m_VerBlurDrawData);

  m_luminescenceDrawData.InTexture = &m_RTLightning->getTexture(0);
  m_luminescenceDrawData.LuminiscenceDelta = 0.0f;
  m_luminescenceDrawData.resultBuffer = &resultBuffer;
  m_luminescencePass.draw(&m_luminescenceDrawData);

  _out.set(GraphicsAPI::getDeviceContext(), _outds);

  //m_emitter.update();
  //m_particleDrawData.activeCam = mainCam;
  //m_particleDrawData.emitter = &m_emitter;
  //m_particlePass.draw(&m_particleDrawData);

  //const float clearColor[4]{0.2f, 0.5f, 0.8f, 1.0f };
  //_out->clear(GraphicsAPI::getDeviceContext(), clearColor);

  m_PostProcessingDrawData.activeCam = mainCam;
  m_PostProcessingDrawData.ChromaticAberrationStrenght = 0.075f;
  m_PostProcessingDrawData.CoCFocusDistance = 390.0f;
  m_PostProcessingDrawData.CoCFocusRange = 50.0f;
  m_PostProcessingDrawData.VignetteScale = 1.0f;
  m_PostProcessingDrawData.VignetteConcentration = Vector2D(4.0f, 4.0f);
  m_PostProcessingDrawData.VignetteRad = Vector2D(1.25f, 1.25f);
  m_PostProcessingDrawData.PositionDepthRT = m_RTGBuffer;
  m_PostProcessingDrawData.ColorRT = m_RTLightning;
  m_PostProcessingDrawData.ColorBlurRT = m_RTLightningBlur;
  m_PostProcessingDrawData.BloomRT = m_RTBloom;
  m_PostProcessingDrawData.Gbuffer = m_RTGBuffer;
  m_PostProcessingDrawData.luminescenceBuffer = resultBuffer;
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

  //GraphicsDriver::API().swapBuffers();
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
RenderManager::updateShadowCameras() {
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