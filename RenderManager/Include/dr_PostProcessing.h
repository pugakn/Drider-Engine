#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_renderpass.h"
#include "dr_light.h"
#include <dr_vector4d.h>
#include <dr_vector2d.h>
#include <dr_sample_state.h>
#include <dr_camera.h>
#include <dr_structure_buffer.h>
#include <dr_vertex_buffer.h>
#include <dr_index_buffer.h>
#include <dr_texture_core.h>

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
  Texture* ColorTex;
  Texture* ColorBlurTex;
  Texture* PositionDepthTex;
  Texture* BloomTex;
  TextureCore* FilmLutTex;
  const RenderTarget* OutRT;
  const DepthStencil* OutDS;
  StructureBuffer* luminescenceBuffer;
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
  GFXUnique<VertexBuffer> m_VBQUAD;
  GFXUnique<IndexBuffer> m_IBQUAD;
  Vector4D m_vertex[4];
  UInt32 m_index[6];

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