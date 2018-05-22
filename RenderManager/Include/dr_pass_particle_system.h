#pragma once
#include "dr_renderman_prerequisites.h"
#include "dr_vector4d.h"
#include "dr_renderpass.h"
#include <dr_sample_state.h>
#include <dr_matrix4x4.h>
#include <dr_particle_emitter.h>
#include <dr_camera.h>
namespace driderSDK {

struct ParticleSystemInitData : PassInitData {};

struct ParticleSystemDrawData : PassDrawData {
  std::shared_ptr<Camera> activeCam;
  Particle* particles;
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
  struct CBuffer {
    Matrix4x4 WVP;
    Vector4D color;
  };

  CBuffer CB;
  GFXUnique<SamplerState> m_samplerState;
};

}