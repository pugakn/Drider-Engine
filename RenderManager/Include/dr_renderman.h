#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_GBuffer1.h"
#include "dr_SSAO.h"
#include "dr_Shadow.h"
#include "dr_PostProcessing.h"
#include "dr_light.h"
#include "dr_HorBlur.h"
#include "dr_VerBlur.h"

#include <dr_texture.h>

namespace driderSDK {

/*
*/
class DR_RENDERMAN_EXPORT RenderMan {
 public:
  /*
  TEST::Constructor

  Default class constructuror.
  */
   RenderMan();

  /*
  TEST::Destructor

  Default class destructuror.
  */
  ~RenderMan();

  /*
  */
  void
  init();

  /*
  */
  void
  draw();

  /*
  */
  void
  exit();

  /*
  */
  void
  recompile();

  std::array<Light, 128>* lights;
  std::array<std::shared_ptr<Camera>, 4> m_vecShadowCamera;
  std::vector<SceneGraph::SharedGameObject> m_vecGos;
  std::vector<float> partitions;
  UInt32 screenWidth;
  UInt32 screenHeight;
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

  PostProcessingPass m_PostProcessingPass;
  PostProcessingInitData m_PostProcessingInitData;
  PostProcessingDrawData m_PostProcessingDrawData;
  GFXShared<DepthStencil> m_PostProcessingDSoptions;

  //0: { xyz: normal,   w: position };
  //1: { xyz: albedo,   w: metallic };
  //2: { xyz: emissive, w: roughness }
  GFXShared<RenderTarget> m_RTGBuffer;
  GFXShared<RenderTarget> m_RTSSAO;
  GFXShared<RenderTarget> m_RTSSAOInitBlur;
  GFXShared<RenderTarget> m_RTSSAOFinalBlur;
  std::array<GFXShared<RenderTarget>, 4> m_RTShadowDummy; //Used for render separated shadowCams
  GFXShared<RenderTarget> m_RTShadow;

  DrTextureDesc GBufferTexDesc;


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
  float  m_fDepth;

  bool m_bFitToScene;
  /***************************************************************************/
  /////////////////////////////////////////////////////////////////////////////

};

}