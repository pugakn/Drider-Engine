#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_renderpass.h"
#include "dr_light.h"
#include <dr_vector4d.h>
#include <dr_vector2d.h>
#include <dr_sample_state.h>
#include <dr_camera.h>

namespace driderSDK {

struct PostProcessingInitData : PassInitData {};

struct PostProcessingDrawData : PassDrawData {
  std::shared_ptr<Camera> activeCam;
  float ChromaticAberrationStrenght;
  float CoCFocusDistance;
  float CoCFocusRange;
  float VignetteScale;
  Vector2D VignetteConcentration;
  Vector2D VignetteRad;
  GFXShared<RenderTarget> PositionDepthRT;
  GFXShared<RenderTarget> ColorRT;
  GFXShared<RenderTarget> ColorBlurRT;
  GFXShared<RenderTarget> Gbuffer;
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
    //X: Aspect Ratio; Y: FOV; Z: Near Plane; W: Far Plane
    Vector4D CameraInfo;
    //X: ChromaticAberrationStrenght, Y: FocusDistance, Z: FocusRange, W: VignetteScale
    Vector4D CA_CoC_V;
    //XY: fVignetteConcentration, ZW: fVignetteRad;
    Vector4D VignetteOptions;
  };

  CBuffer CB;

  GFXUnique<SamplerState> m_samplerState;
};

}