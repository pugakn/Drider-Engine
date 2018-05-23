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

struct ParticleSystemInitData : PassInitData {};

struct ParticleSystemDrawData : PassDrawData {
  std::shared_ptr<Camera> activeCam;
  Particle* particles;
  ParticleEmitter* emitter;
  size_t numParticles;
};

class ParticleSystemPass : public RenderPass {
 public:
  /*
  TEST::testName
  
  Description.
  */
   ParticleSystemPass();

  /*
  TEST::testName
  
  Description.
  */
  ~ParticleSystemPass();

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

  GFXUnique<SamplerState> m_samplerState;
  GFXUnique<InputLayout> m_inputLayoutInstance;
  GFXUnique<VertexBuffer> m_instanceBuffer;
};

}