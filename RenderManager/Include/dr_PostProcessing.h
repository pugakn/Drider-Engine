#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_renderpass.h"
#include "dr_light.h"
#include <dr_sample_state.h>

namespace driderSDK {

struct PostProcessingInitData : PassInitData {};

struct PostProcessingDrawData : PassDrawData {
  Vector4D CameraPosition;
  GFXShared<RenderTarget> Gbuffer1RT;
  GFXShared<RenderTarget> SSAORT;
  GFXShared<RenderTarget> ShadowRT;
  float ActiveLights;
  std::array<Light, 128>* Lights;
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
    Vector4D EyePosition;         // [XYZ = Cameraposition, W = ActiveLights]
    Vector4D LightPosition[128];  // [XYZ = LightPosition]
    Vector4D LightColor[128];     // [XYZ = LightColor, W = LightIntensity]
  };

  CBuffer CB;

  GFXUnique<SamplerState> m_samplerState;
};

}