#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_renderpass.h"

#include <dr_texture.h>
#include <dr_sample_state.h>
#include <dr_camera.h>

namespace driderSDK {

struct SSReflectionsInitData : PassInitData {
};

struct SSReflectionsDrawData : PassDrawData {
  std::shared_ptr<Camera> ActiveCam;
  RenderTarget* GbufferRT;
  RenderTarget* ColorRT;
  RenderTarget* OutRt;
};

class SSReflectionsPass : public RenderPass {
 public:
  /*
  TEST::testName
  
  Description.
  */
  SSReflectionsPass();

  /*
  TEST::testName
  
  Description.
  */
  ~SSReflectionsPass();

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
  struct CBufferDraw {
    Vector4D ViewportDimensions;
    Vector4D EyePosition;
    Vector4D CameraInfo;
    Matrix4x4 CameraVP;
    Vector4D ThreadsInfo;
  };

  SizeT m_RTWidth;
  SizeT m_RTHeight;
  SizeT m_ComputeWidthDivisions;
  SizeT m_ComputeHeightDivisions;
  SizeT m_ComputeWidthBlocks;
  SizeT m_ComputeHeightBlocks;
  SizeT m_ComputeTotalBlocks;

  GFXUnique<SamplerState> m_samplerState;

  CBufferDraw m_CBDrawData;
  GFXUnique<ConstantBuffer> m_CBDraw;
};

}