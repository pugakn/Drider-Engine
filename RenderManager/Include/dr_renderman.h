#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_light.h"
#include "dr_GBuffer1.h"
#include "dr_SSAO.h"
#include "dr_HorBlur.h"
#include "dr_VerBlur.h"
#include "dr_Shadow.h"
#include "dr_Lightning.h"
#include "dr_PostProcessing.h"

#include <dr_texture.h>

#include "dr_pass_particle_system.h"
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
class DR_RENDERMAN_EXPORT RenderMan {
 public:
  /**
  * TEST::defaultConstructor
  *
  * Default constructor.
  */
  RenderMan();

  /**
  * TEST::destructor
  *
  * Default destructor
  *
  */
  ~RenderMan();

  /**
  * TEST::init
  *
  * Initializes the RenderManager (Including all the passes).
  */
  void
  init();

  /**
  * TEST::draw
  *
  * Sets up the passes draw information and calls the Draw
  * function of the passes.
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

  UInt32 screenWidth;
  UInt32 screenHeight;
  UInt32 shadowWidth;
  UInt32 shadowHeight;
  std::array<Light, 128>* lights;
  std::array<std::shared_ptr<Camera>, 4> vecShadowCamera;
  std::vector<float> partitions;
  std::vector<SceneGraph::SharedGameObject> vecGos;
 protected:
  std::shared_ptr<Texture> m_cubemap;

  GBufferPass m_GBufferPass;
  GBufferInitData m_GBufferInitData;
  GBufferDrawData m_GBufferDrawData;
  GFXShared<DepthStencil> m_GBufferDSoptions;

  SSAOPass m_SSAOPass;
  SSAOInitData m_SSAOInitData;
  SSAODrawData m_SSAODrawData;
  GFXShared<DepthStencil> m_SSAODSoptions;

  HorBlurPass m_HorBlurPass;
  HorBlurInitData m_HorBlurInitData;
  HorBlurDrawData m_HorBlurDrawData;
  GFXShared<DepthStencil> m_HorBlurDSoptions;

  ParticleSystemPass m_particlePass;
  ParticleSystemInitData m_particleInitData;
  ParticleSystemDrawData m_particleDrawData;
  ParticleEmitter m_emitter;

  VerBlurPass m_VerBlurPass;
  VerBlurInitData m_VerBlurInitData;
  VerBlurDrawData m_VerBlurDrawData;
  GFXShared<DepthStencil> m_VerBlurDSoptions;

  //VerBlurPass m_VerBlurPass;
  //VerBlurInitData m_VerBlurInitData;
  //VerBlurDrawData m_VerBlurDrawData;
  //GFXShared<DepthStencil> m_VerBlurDSoptions;

  ShadowPass m_ShadowPass;
  ShadowInitData m_ShadowInitData;
  ShadowDrawData m_ShadowDrawData;
  GFXShared<DepthStencil> m_ShadowDSoptions;
  std::array<std::pair<Vector3D, float>, 4> m_ShadowSubFrustras;

  LightningPass m_LightningPass;
  LightningInitData m_LightningInitData;
  LightningDrawData m_LightningDrawData;
  GFXShared<DepthStencil> m_LightningDSoptions;

  PostProcessingPass m_PostProcessingPass;
  PostProcessingInitData m_PostProcessingInitData;
  PostProcessingDrawData m_PostProcessingDrawData;
  GFXShared<DepthStencil> m_PostProcessingDSoptions;

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
  std::array<GFXShared<RenderTarget>, 4> m_RTShadowDummy; //Used for render separated shadowCams
  GFXShared<RenderTarget> m_RTShadow;
  GFXShared<RenderTarget> m_RTPreFinalBlur;
  GFXShared<RenderTarget> m_RTPostProcessing;

  DrTextureDesc m_TexDescDefault;

  /////////////////////////////////////////////////////////////////////////////
  /*****************************Shadow pass stuff*****************************/

  /*
  */
  void
  updateShadowCameras();

  std::vector<float>
  calculatePartitions(SizeT cuts);

  std::pair<Vector3D, float>
  frustrumSphere(float fViewportWidth,
                 float fViewportHeight,
                 float fNearPlane,
                 float fFarPlane,
                 float fFov);

  Vector3D m_vec3DirectionalLight;

  SizeT m_szActiveShadowCameras;
  float  m_fMinDepth;
  float  m_fMaxDepth;

  bool m_bFitToScene;
  /***************************************************************************/
  /////////////////////////////////////////////////////////////////////////////

};

}