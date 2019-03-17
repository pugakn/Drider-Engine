#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_renderpass.h"
#include "dr_light.h"

#include <dr_texture_core.h>
#include <dr_texture.h>
#include <dr_sample_state.h>
#include <dr_camera.h>
#include <dr_structure_buffer.h>

namespace driderSDK {

struct LightningInitData : PassInitData {
  SizeT RTWidth;
  SizeT RTHeight;
};

struct LightningLightsToSSData : PassDrawData {
  std::shared_ptr<Camera> ActiveCam;
  std::array<PointLight, RM_MAX_POINT_LIGHTS>* Lights;
  Int32 numberOfLights;
};

struct LightningTileLightsSSData : PassDrawData {
  RenderTarget* OutRt;
  Int32 numberOfLights;
};

struct LightningDrawData : PassDrawData {
  std::shared_ptr<Camera> ActiveCam;
  std::array<PointLight, RM_MAX_POINT_LIGHTS>* Lights;
  RenderTarget* GbufferRT;
  RenderTarget* SSAO_SSShadowRT;
  RenderTarget* SSReflection;
  RenderTarget* OutRt;
  TextureCore* EnviromentCubemap;
  float* EnviromentScale;
  TextureCore* IrradianceCubemap;
  float* IrradianceScale;
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
  recompileShader(String vsPreText = "",
                  String psPreText = "",
                  String csPreText = "");

  /*
  */
  void
  lightsToScreenSpace(LightningLightsToSSData* data);

  /*
  */
  void
  tileLights(LightningTileLightsSSData* data);

  /*
  */
  void
  draw(PassDrawData* drawData);

 private:
  struct CBufferWSLightsToSS {
    Vector4D CameraUp;
    Vector4D CameraFront;
    Matrix4x4 CameraVP;

    // [XYZ = LightPosition, W = Range]
    // W Sign: Positive = light is active, Negative = light is inactive
    Vector4D LightPosition[RM_MAX_POINT_LIGHTS];
  };

  struct CBufferTiledLights {
    Vector4D threadsInfo;
  };

  struct CBufferDraw {
    Vector4D ViewportSzEnvIrr;
    Vector4D EyePosition;
    Vector4D LightPosition[RM_MAX_POINT_LIGHTS];  //XYZ: Light Position, W: Range
    Vector4D LightColor[RM_MAX_POINT_LIGHTS];     //XYZ: Light Color, W: Intensity
    Vector4D ThreadsInfo;
  };

  SizeT m_RTWidth;
  SizeT m_RTHeight;
  SizeT m_ComputeWidthDivisions;
  SizeT m_ComputeHeightDivisions;
  SizeT m_ComputeWidthBlocks;
  SizeT m_ComputeHeightBlocks;
  SizeT m_ComputeTotalBlocks;

  GFXUnique<Shader> m_csWorldLightsToSS;
  GFXUnique<Shader> m_csTiledLights;

  GFXUnique<SamplerState> m_samplerState;
  GFXUnique<SamplerState> m_samplerStateCubemap;

  //////////New stuff//////////
  GFXShared<RenderTarget> m_RTLightsInSS;
  GFXShared<RenderTarget> m_RTLightsIndex;
  GFXShared<RenderTarget> m_RTLightsIndexAux;

  CBufferWSLightsToSS m_CBWSLightsToSSData;
  GFXUnique<ConstantBuffer> m_CBWSLightsToSS;

  CBufferTiledLights m_CBTileLightsData;
  GFXUnique<ConstantBuffer> m_CBTileLights;

  CBufferDraw m_CBDrawData;
  GFXUnique<ConstantBuffer> m_CBDraw;
};

}