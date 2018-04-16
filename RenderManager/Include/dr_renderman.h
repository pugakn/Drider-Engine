#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_GBuffer1.h"
#include "dr_SSAO.h"
#include "dr_Shadow.h"
#include "dr_PostProcessing.h"
#include "dr_light.h"

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
 protected:

  GBuffer1Pass m_GBuffer1Pass;
  GBuffer1InitData m_GBuffer1InitData;
  GBuffer1DrawData m_GBuffer1DrawData;
  GFXShared<DepthStencil> m_GBuffer1DSoptions;

  SSAOPass m_SSAOPass;
  SSAOInitData m_SSAOInitData;
  SSAODrawData m_SSAODrawData;
  GFXShared<DepthStencil> m_SSAODSoptions;

  ShadowPass m_ShadowPass;
  ShadowInitData m_ShadowInitData;
  ShadowDrawData m_ShadowDrawData;
  GFXShared<DepthStencil> m_ShadowDSoptions;

  PostProcessingPass m_PostProcessingPass;
  PostProcessingInitData m_PostProcessingInitData;
  PostProcessingDrawData m_PostProcessingDrawData;
  GFXShared<DepthStencil> m_PostProcessingDSoptions;

  //0: Albedo; 1: Position; 2: Normal; 3: Emissive; 4: Metallic; 5: Roughness;
  GFXShared<RenderTarget> m_RTGBuffer1;
  //0: SSAO
  GFXShared<RenderTarget> m_RTSSAO;
  //0: Shadow0; 1: Shadow1; 2: Shadow2; 3: Shadow3;
  GFXShared<RenderTarget> m_RTShadow;

  DrTextureDesc GBufferTexDesc;


  /////////////////////////////////////////////////////////////////////////////
  /*****************************Shadow pass stuff*****************************/

  /*
  */
  void
  updateShadowCameras();

  std::vector<float>
  calculatePartitions(size_t cuts);

  std::pair<Vector3D, float>
  frustrumSphere(float fViewportWidth,
                 float fViewportHeight,
                 float fNearPlane,
                 float fFarPlane,
                 float fFov);

  Vector3D m_vec3DirectionalLight;

  size_t m_szActiveShadowCameras;
  float  m_fDepth;

  bool m_bFitToFrustrum;
  /***************************************************************************/
  /////////////////////////////////////////////////////////////////////////////

};

}