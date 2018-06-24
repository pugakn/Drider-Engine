#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_vector4d.h"
#include "dr_renderpass.h"
#include <dr_sample_state.h>
#include <dr_matrix4x4.h>
#include <dr_particle_emitter.h>
#include <dr_camera.h>
#include <dr_vertex_buffer.h>

namespace driderSDK {

class IndexBuffer;
struct LuminescenceInitData : PassInitData {};

struct LuminescenceDrawData : PassDrawData {
  Texture* InTexture;
  float LuminiscenceDelta;
};

class LuminescencePass : public RenderPass {
 public:
  /*
  TEST::defaultConstructor
  
  Description.
  */
   LuminescencePass();

  /*
  TEST::defaultDestructor
  
  Description.
  */
  ~LuminescencePass();

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
    float	LuminiscenceDelta;
    float	TextureWidth;
    float	TextureHeight;
    float pad;
  };
  
  CBuffer CB;

  StructureBuffer* m_resultBuffer;

  GFXUnique<SamplerState> m_samplerState;
};

}