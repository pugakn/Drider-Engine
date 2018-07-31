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

struct LightningDrawData : PassDrawData {
  std::shared_ptr<Camera> ActiveCam;
  SizeT ActiveLights;
  std::array<Light, 128>* Lights;
  GFXShared<RenderTarget> GbufferRT;
  GFXShared<RenderTarget> SSAORT;
  GFXShared<RenderTarget> ShadowRT;
  GFXShared<RenderTarget> OutRt;
  GFXShared<TextureCore> EnviromentCubemap;
  GFXShared<TextureCore> IrradianceCubemap;
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

  void
  tileLights(PassDrawData* drawData);

 private:
  struct CBuffer1 {
    Vector4D fViewportDimensions;
    Vector4D EyePosition;         // [XYZ = Cameraposition, W = ActiveLights]
    Vector4D LightPosition[128];  // [XYZ = LightPosition, W = Range]
    Vector4D LightColor[128];     // [XYZ = LightColor, W = LightIntensity]
    Vector4D threadsInfo;
  };

  struct CBuffer2 {
    Vector4D ViewportDimensions;
    Vector4D CameraUp;
    Vector4D ThreadsGroups;
    Matrix4x4 VP;
    Vector4D LightPosition[128];	//XYZ: Light Position, W: Range
  };

  CBuffer1 CB;
  CBuffer2 CBTiled;

  GFXUnique<ConstantBuffer> m_constantBufferTiled;

  SizeT m_RTWidth;
  SizeT m_RTHeight;
  SizeT m_ComputeWidthDivisions;
  SizeT m_ComputeHeightDivisions;
  SizeT m_ComputeWidthBlocks;
  SizeT m_ComputeHeightBlocks;
  SizeT m_ComputeTotalBlocks;

  TString m_csTiledLightsFilename;

  GFXUnique<Shader> m_csTiledLights;

  GFXUnique<SamplerState> m_samplerState;
  GFXUnique<SamplerState> m_samplerStateCubemap;

  GFXUnique<StructureBuffer> m_sbNumberOfLights;
  GFXUnique<StructureBuffer> m_sbLightsIndex;
};

}