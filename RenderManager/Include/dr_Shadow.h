#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_renderpass.h"
#include <dr_graph.h>
#include <dr_render_component.h>
#include <dr_camera.h>
#include <dr_sample_state.h>

namespace driderSDK {

struct ShadowInitData : PassInitData {
  SizeT RTWidht;
  SizeT RTHeight;
};

struct ShadowDrawData : PassDrawData {
  //CascadeInfo
  Camera* shadowCam;
  RenderCommandBuffer*    models;
  RenderTarget* OutRt;
  DepthStencil* dsOptions;
  //ShadowInfo
  std::array<std::unique_ptr<Camera>, 4>* ShadowCameras;
  std::vector<float> ShadowSliptDepths;
  SizeT ActivatedShadowCascades;
  SizeT ShadowMapTextureSize;
  float LerpBetweenShadowCascade;
  Vector4D ShadowSizesProportion;
};

class ShadowPass : public RenderPass {
 public:
  /*
  TEST::testName
  
  Description.
  */
  ShadowPass();

  /*
  TEST::testName
  
  Description.
  */
  ~ShadowPass();

  /*
  TEST::testName

  Description.
  */
  void
  init(PassInitData* initData);

  void
  changeSize(SizeT Width, SizeT Height);

  void
  recompileShader(String vsPreText = "",
                  String psPreText = "",
                  String csPreText = "");

  /*
  */
  void
  draw(PassDrawData* drawData);

  /*
  */
  void
  merge(std::array<GFXUnique<RenderTarget>, 4>& m_RTShadowDummy,
        RenderTarget* CompressedShadowsOutRt);

  
  /*
  */
  void
  apply(PassDrawData* drawData,
        RenderTarget* PositionDepthRt,
        RenderTarget* CompressedShadowsOutRt,
        RenderTarget* ResultShadowsRt);

 private:
  struct CBuffer1 {
    Matrix4x4 WVP;
    Matrix4x4 Bones[200];
  };

  struct CBuffer2 {
    Vector4D  fViewportDimensions;
    Matrix4x4 ShadowVP[4];
    Vector4D  ShadowSplitDepth;
    Vector4D  ShadowSizesProportion;
    Vector4D  ShadowInfo; //X: Activated cascades, Y: TextureSize, Z: CascadeLerp
  };

  CBuffer1 CascadeCB;
  CBuffer2 ShadowCB;

  TString m_csFilenameShadowApply;

  GFXUnique<Shader> m_computeShaderApply;

  GFXUnique<ConstantBuffer> m_constantBufferSSShadow;

  GFXUnique<SamplerState> m_samplerState;

  SizeT RTWidht;
  SizeT RTHeight;
};

}