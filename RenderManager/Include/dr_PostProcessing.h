#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_renderpass.h"
#include "dr_light.h"
#include <dr_sample_state.h>
#include <dr_camera.h>

namespace driderSDK {

struct PostProcessingInitData : PassInitData {};

struct PostProcessingDrawData : PassDrawData {
  std::shared_ptr<Camera> activeCam;
  Vector4D DirLight;
  GFXShared<RenderTarget> Gbuffer1RT;
  GFXShared<RenderTarget> SSAORT;
  GFXShared<RenderTarget> ShadowRT;
  float ActiveLights;
  std::array<Light, 128>* Lights;
  std::array<std::shared_ptr<Camera>, 4>* ShadowCam;
};

class PostProcessingPass : public RenderPass {
 public:
  /*
  TEST::testName
  
  Description.
  */
  PostProcessingPass();

  /*
  TEST::testName
  
  Description.
  */
  ~PostProcessingPass();

  /*
  TEST::testName
  Description.
  */
  void
  init(PassInitData* initData);

  /*
  */
  void
  draw(PassDrawData* drawData);

 private:
  struct CBuffer {
    Vector4D  EyePosition;         // [XYZ = Cameraposition, W = ActiveLights]
    Vector4D  DirLight;
    Vector4D  LightPosition[128];  // [XYZ = LightPosition]
    Vector4D  LightColor[128];     // [XYZ = LightColor, W = LightIntensity]
    Matrix4x4 VP;
    Matrix4x4 VPInv;
    Matrix4x4 ShadowCam[4];
    Matrix4x4 ShadowCamInv[4];
  };

  CBuffer CB;

  GFXUnique<SamplerState> m_samplerState;
};

}