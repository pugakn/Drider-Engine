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
  std::shared_ptr<Camera> activeCam;
  GFXShared<Texture> InTexture;
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
  struct CBuff {
    float	LuminiscenceDelta;
    UInt32 TextureWidth;
    UInt32 TextureHeight;
    float pad;
  };
  
  CBuff m_CB;

  GFXUnique<Shader> m_computeShader;
};

}