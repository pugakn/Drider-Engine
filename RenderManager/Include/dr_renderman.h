#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_light.h"
#include "dr_GBuffer1.h"
#include "dr_Lines.h"
#include "dr_Shadow.h"
#include "dr_SSAO.h"
#include "dr_HorBlur.h"
#include "dr_VerBlur.h"
#include "dr_SSReflection.h"
#include "dr_Lightning.h"
#include "dr_Bloom.h"
#include "dr_Luminescence.h"
#include "dr_PostProcessing.h"

#include <dr_texture.h>

//#include "dr_pass_particle_system.h"
#include <dr_module.h>
//#include <dr_particle_emitter.h>
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
  std::array<Light, RM_MAX_LIGHTS> lights;
  std::array<std::unique_ptr<Camera>, 4> vecShadowCamera;
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

  void
  setCubeMap(std::shared_ptr<TextureCore> cubemap);

  void
  setEnviromentMap(std::shared_ptr<TextureCore> enviromentmap);

  void
  setFilmLut(std::shared_ptr<TextureCore> filmLut);
  /**
  * Draws a line in debug mode.
  *
  * @start
  *  line start position.
  *
  * @end
  *  line end position.
  * 
  * @color
  *  color of the line.
  *
  * @transform
  *  Transform of the line.
  */
  void
  drawDebugLine(const Vector3D& start,
                const Vector3D& end,
                const Vector3D& color,
                const Matrix4x4& transform = Matrix4x4::identityMat4x4);

  /**
  * Draws a strip line in debug mode.
  *
  * @points
  *  vector containing all the points to be renderer.
  *
  * @color
  *  color of the line.
  *
  * @transform
  *  Transform of the line.
  */
  void
  drawDebugLine(const std::vector<Vector3D>& points,
                const Vector3D& color,
                const Matrix4x4& transform = Matrix4x4::identityMat4x4);
  
  /**
  * Draws a cube in debug mode.
  *
  * @points
  *  vector containing all the points to be renderer.
  *
  * @color
  *  color of the line.
  *
  * @transform
  *  Transform of the cube.
  */
  void
  drawDebugCube(const Vector3D& dimensions,
                const Vector3D& color,
                const Matrix4x4& transform = Matrix4x4::identityMat4x4);
 
  /**
  * Draws a cube in debug mode.
  *
  * @points
  *  vector containing all the points to be renderer.
  *
  * @color
  *  color of the line.
  *
  * @transform
  *  Transform of the cube.
  */
  void
  drawDebugSphere(const float radius,
                  const Vector3D& color,
                  const Matrix4x4& transform = Matrix4x4::identityMat4x4);

 protected:

  std::shared_ptr<Model> m_quad;
  std::shared_ptr<TextureCore> m_cubemap;
  std::shared_ptr<TextureCore> m_cubemapDiffuse;
  std::shared_ptr<TextureCore> m_FilmLut;

  //VS & FS
  GBufferPass m_GBufferPass;
  GBufferInitData m_GBufferInitData;
  GBufferDrawData m_GBufferDrawData;
  GFXUnique<DepthStencil> m_GBufferDSoptions;

  //VS & FS
  LinesPass m_LinesPass;
  LinesInitData m_LinesInitData;
  LinesDrawData m_LinesDrawData;

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

  //VS, FS & CS
  ShadowPass m_ShadowPass;
  ShadowInitData m_ShadowInitData;
  ShadowDrawData m_ShadowDrawData;
  GFXUnique<DepthStencil> m_ShadowDSoptions;
  std::array<std::pair<float, float>, 4> m_ShadowSubFrustras;

  //CS
  SSReflectionsPass m_SSReflectionPass;
  SSReflectionsInitData m_SSReflectionInitData;
  SSReflectionsDrawData m_SSReflectionDrawData;

  //CS
  LightningPass m_LightningPass;
  LightningInitData m_LightningInitData;
  LightningLightsToSSData m_LWSLightsToSSData;
  LightningTileLightsSSData m_LTileLightsData;
  LightningDrawData m_LightningDrawData;

  //CS
  BloomPass m_BloomPass;
  BloomInitData m_BloomInitData;
  BloomDrawData m_BloomDrawData;

  //CS
  //ParticleSystemPass m_particlePass;
  //ParticleSystemInitData m_particleInitData;
  //ParticleSystemDrawData m_particleDrawData;
  //ParticleEmitter m_emitter;

  //CS
  LuminescencePass m_luminescencePass;
  LuminescenceInitData m_luminescenceInitData;
  LuminescenceDrawData m_luminescenceDrawData;
  StructureBuffer* resultBuffer;

  //CS
  PostProcessingPass m_PostProcessingPass;
  PostProcessingInitData m_PostProcessingInitData;
  PostProcessingDrawData m_PostProcessingDrawData;

  std::array<GFXUnique<RenderTarget>, 4> m_RTShadowDummy; //Used for render shadow cascades.
  GFXUnique<RenderTarget> m_RTShadow; //Compressed shadows.
  /**
  * Gbuffer info:
  * 0: { xyz: position, w: linear depth };
  * 1: { xyz: normal,   w: CoC };
  * 2: { xyz: albedo,   w: metallic };
  * 3: { xyz: emissive, w: roughness };
  */
  GFXUnique<RenderTarget> m_RTGBuffer;
  GFXUnique<RenderTarget> m_RTBlurInit;
  GFXUnique<RenderTarget> m_RTSSAO_SSShadow;
  GFXUnique<RenderTarget> m_RTSSAO_SSShadowBlur;
  GFXUnique<RenderTarget> m_RTSSReflection;
  GFXUnique<RenderTarget> m_RTLightning;
  GFXUnique<RenderTarget> m_RTBrightness;
  GFXUnique<RenderTarget> m_RTBloom;
  GFXUnique<RenderTarget> m_RTLightningBlur;

  /////////////////////////////////////////////////////////////////////////////
  /*****************************Shadow pass stuff*****************************/

  /*
  * Test::updateShadowCameras
  * 
  * Updates the shadow cameras.
  */
  void
  updateShadowCameras(const Vector3D lightDir);

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

  std::vector<Vector3D> m_vecDirectionalLights;

  SizeT m_szActiveShadowCameras;
  float m_fMinDepth;
  float m_fMaxDepth;

  bool m_bFitToScene;
  /***************************************************************************/
  /////////////////////////////////////////////////////////////////////////////

};

}