#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_light.h"
#include "dr_GBuffer1.h"
#include "dr_Shadow.h"
#include "dr_SSAO.h"
#include "dr_HorBlur.h"
#include "dr_VerBlur.h"
#include "dr_Lightning.h"
#include "dr_Bloom.h"
#include "dr_Luminescence.h"
#include "dr_PostProcessing.h"

#include <dr_texture.h>

#include "dr_pass_particle_system.h"
#include <dr_module.h>
#include <dr_particle_emitter.h>
#include <dr_vector3d.h>
namespace driderSDK {

/**
* Render manager.
* This class handles and performs the draw logic.
*
* Sample usage:
* 
* RenderMan RenderManager;
* RenderManager.init();
* while (bDraw) {
*   RenderManager.draw();
*   if (bRecompileRenderManager) {
*     RenderManager.recompile();
*   }
* }
* RenderManager.exit();
}
*/
class DR_RENDERMAN_EXPORT RenderManager : public Module<RenderManager> {
 public:
  /**
  * TEST::defaultConstructor
  *
  * Default constructor.
  */
  RenderManager();

  /**
  * TEST::destructor
  *
  * Default destructor
  *
  */
  ~RenderManager();

  /**
  * TEST::init
  *
  * Initializes the RenderManager
  * (Including all the passes).
  */
  void
  init();

  /**
  * TEST::draw
  *
  * Sets up the passes draw information and calls the Draw
  * function of the passes.
  *
  * @param _out
  *  Renter target where the finall pass will draw on.
  *
  * @param _outds
  *  Depth stencil to use.
  */
  void
  draw(const RenderTarget& _out, const DepthStencil& _outds);

  /**
  * TEST::exit
  *
  * Calls the exit function of the passes.
  */
  void
  exit();

  /**
  * TEST::recompile
  *
  * Calls the recompile function of the passes.
  */
  void
  recompile();

  float* luminanceDelta;
  UInt32 screenWidth;
  UInt32 screenHeight;
  UInt32 shadowWidth;
  UInt32 shadowHeight;
  std::array<Light, 128>* lights;
  std::array<std::shared_ptr<Camera>, 4> vecShadowCamera;
  std::vector<float> partitions;
  std::vector<SceneGraph::SharedGameObject> vecGos;

  /*
  * TEST::onStartUp
  *
  */
  void
  onStartUp();

  /*
  * TEST::onStartUp
  *
  */
  void
  onShutDown();

 protected:

  std::shared_ptr<TextureCore> m_cubemap;
  std::shared_ptr<TextureCore> m_cubemapDiffuse;

  //VS & FS
  GBufferPass m_GBufferPass;
  GBufferInitData m_GBufferInitData;
  GBufferDrawData m_GBufferDrawData;
  GFXShared<DepthStencil> m_GBufferDSoptions;

  //CS
  SSAOPass m_SSAOPass;
  SSAOInitData m_SSAOInitData;
  SSAODrawData m_SSAODrawData;

  //CS
  HorBlurPass m_HorBlurPass;
  HorBlurInitData m_HorBlurInitData;
  HorBlurDrawData m_HorBlurDrawData;

  //CS
  VerBlurPass m_VerBlurPass;
  VerBlurInitData m_VerBlurInitData;
  VerBlurDrawData m_VerBlurDrawData;
  GFXShared<DepthStencil> m_VerBlurDSoptions;

  //VS, FS & CS
  ShadowPass m_ShadowPass;
  ShadowInitData m_ShadowInitData;
  ShadowDrawData m_ShadowDrawData;
  GFXShared<DepthStencil> m_ShadowDSoptions;
  std::array<std::pair<float, float>, 4> m_ShadowSubFrustras;

  //CS
  LightningPass m_LightningPass;
  LightningInitData m_LightningInitData;
  LightningDrawData m_LightningDrawData;
  GFXShared<DepthStencil> m_LightningDSoptions;

  //CS
  BloomPass m_BloomPass;
  BloomInitData m_BloomInitData;
  BloomDrawData m_BloomDrawData;

  //CS
  ParticleSystemPass m_particlePass;
  ParticleSystemInitData m_particleInitData;
  ParticleSystemDrawData m_particleDrawData;
  ParticleEmitter m_emitter;

  //CS
  LuminescencePass m_luminescencePass;
  LuminescenceInitData m_luminescenceInitData;
  LuminescenceDrawData m_luminescenceDrawData;
  StructureBuffer* resultBuffer;

  //VS & CS (TODO: Make it Compute)
  PostProcessingPass m_PostProcessingPass;
  PostProcessingInitData m_PostProcessingInitData;
  PostProcessingDrawData m_PostProcessingDrawData;
  GFXShared<DepthStencil> m_PostProcessingDSoptions;

  std::array<GFXShared<RenderTarget>, 4> m_RTShadowDummy; //Used for render shadow cascades.
  GFXShared<RenderTarget> m_RTShadow; //Compressed shadows.
  GFXShared<RenderTarget> m_RTSSShadow;
  GFXShared<RenderTarget> m_RTSSShadowBlur;
  //Gbuffer info:
  //0: { xyz: position, w: linear depth };
  //1: { xyz: normal,   w: CoC };
  //2: { xyz: albedo,   w: metallic };
  //3: { xyz: emissive, w: roughness };
  GFXShared<RenderTarget> m_RTGBuffer;
  GFXShared<RenderTarget> m_RTSSAO;
  GFXShared<RenderTarget> m_RTBlurInit;
  GFXShared<RenderTarget> m_RTSSAOBlur;
  GFXShared<RenderTarget> m_RTLightning;
  GFXShared<RenderTarget> m_RTBrightness;
  GFXShared<RenderTarget> m_RTBloom;
  std::vector<Texture*> m_vecTexture;
  GFXShared<RenderTarget> m_RTLightningBlur;

  DrTextureDesc m_TexDescDefault;

  /////////////////////////////////////////////////////////////////////////////
  /*****************************Shadow pass stuff*****************************/

  /*
  * Test::updateShadowCameras
  * 
  * Updates the shadow cameras.
  */
  void
  updateShadowCameras();

  /*
  * Test::calculatePartitions
  *
  * Updates the shadow cameras.
  *
  * @param cuts
  *  The number of partition of shadow subfrustras.
  *
  * @return
  *  The cuts proportions.
  */
  std::vector<float>
  calculatePartitions(SizeT cuts);

  /*
  * Test::frustrumSphere
  *
  * Calculates the shadow frustrum spheres radius and position.
  *
  * @param fViewportWidth
  *  The main camera viewport width
  * 
  * @param fViewportHeight
  *  The main camera viewport height
  * 
  * @param fNearPlane
  *  The main camera near plane
  * 
  * @param fFarPlane
  *  The main camera far plane
  * 
  * @param fFov
  *  The main camera fov
  *
  * @return
  *  The forward position and radius
  *  of the frustrum sphere
  */
  std::pair<float, float>
  frustrumSphere(float fViewportWidth,
                 float fViewportHeight,
                 float fNearPlane,
                 float fFarPlane,
                 float fFov);

  Vector3D m_vec3DirectionalLight;

  SizeT m_szActiveShadowCameras;
  float m_fMinDepth;
  float m_fMaxDepth;

  bool m_bFitToScene;
  /***************************************************************************/
  /////////////////////////////////////////////////////////////////////////////

};

}