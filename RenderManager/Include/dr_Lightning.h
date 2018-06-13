#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_renderpass.h"
#include "dr_light.h"

#include <dr_texture_core.h>
#include <dr_texture.h>
#include <dr_sample_state.h>
#include <dr_camera.h>

namespace driderSDK {

struct LightningInitData : PassInitData {};

struct LightningDrawData : PassDrawData {
  std::shared_ptr<Camera> ActiveCam;
  SizeT ActiveLights;
  std::array<Light, 128>* Lights;
  std::array<std::shared_ptr<Camera>, 4>* ShadowCameras;
  std::vector<float> ShadowSliptDepths;
  SizeT ActivatedShadowCascades;
  SizeT ShadowMapTextureSize;
  float LerpBetweenShadowCascade;
  Vector4D ShadowSizesProportion;
  Vector3D BloomThreshold;
  float LuminiscenceDelta;
  GFXShared<RenderTarget> GbufferRT;
  GFXShared<RenderTarget> SSAORT;
  GFXShared<RenderTarget> ShadowRT;
  GFXShared<RenderTarget> OutRt;
  GFXShared<TextureCore> Cubemap;
  GFXShared<DepthStencil> dsOptions;
};

class LightningPass : public RenderPass {
 public:
  /*
  TEST::testName
  
  Description.
  */
  LightningPass();

  /*
  TEST::testName
  
  Description.
  */
  ~LightningPass();

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
    Vector4D  LightPosition[128];  // [XYZ = LightPosition, W = Range]
    Vector4D  LightColor[128];     // [XYZ = LightColor, W = LightIntensity]
    Matrix4x4 ShadowCameraVP[4];
    Vector4D  ShadowSliptDepth;
    Vector4D  ShadowSizesProportion;
    Vector4D  ShadowInfo;
    Vector4D  BloomThresholdLuminiscenceDelta;
  };

  CBuffer CB;

  GFXUnique<SamplerState> m_samplerState;
  GFXUnique<SamplerState> m_samplerStateCubemap;
};

}