#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_renderpass.h"
#include <dr_render_component.h>
#include <dr_camera.h>
#include <dr_matrix4x4.h>
#include <dr_input_layout.h>
#include <dr_constant_buffer.h>
#include <dr_sample_state.h>
#include <dr_vector3d.h>
#include "dr_light.h"

namespace driderSDK {

struct PostProcessingInitData : PassInitData {};

struct PostProcessingDrawData : PassDrawData {
  Vector4D CameraPosition;
  GFXShared<RenderTarget> Gbuffer1RT;
  GFXShared<RenderTarget> Gbuffer2RT;
  std::array<Light, 128> *Lights;
};

class PostProcessingPass : RenderPass {
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
    Vector4D EyePosition;
    Vector4D Position[128];
    float Intensity[128];
    Vector4D Color[128];
    Int32 activeLights;
    Int32 shit1;
    Int32 shit2;
    Int32 shit3;
  };
/*
float4 EyePosition;
float4 Position[256];
float Intensity[256];
float4 Color[256];
int activeLights;
*/

  CBuffer CB;

  GFXUnique<SamplerState> m_samplerState;
};

}