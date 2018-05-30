#pragma once
#include "dr_particles_prerequisites.h"
#include <dr_vector3d.h>
#include <dr_pool.h>
#include <vector>
#include <dr_matrix4x4.h>
#include <dr_vector4d.h>
#include <iostream>
#include <dr_shader.h>
#include <dr_structure_buffer.h>
#include <dr_constant_buffer.h>
#include <dr_indirect_args_buffer.h>
#define DR_PARTICLES_CPU 0
#define DR_PARTICLES_GPU 1
#define DR_PARTICLES_METHOD  DR_PARTICLES_GPU
namespace driderSDK {
  struct DR_PARTICLES_EXPORT Particle { 
    float* m_lifeTime;
    bool* m_isActive;

    Vector3D* m_position;
    Vector3D* m_rotation;

    float* m_scale;
    float* m_scaleFactor;
    float* m_speedLimit;
    //float* m_rotationSpeed;

    Vector3D* m_color;

    Vector3D* m_velocity;
    Vector3D* m_acceleration;
  };
  struct DR_PARTICLES_EXPORT GPUParticle {
    Vector4D position;
    Vector4D velocity;
    Vector4D acceleration;
    Vector4D color;

    float lifeTime;
    float scale;
    Int32 isActive;
    Int32 pad;
  };
  struct DR_PARTICLES_EXPORT RenderStructureBuffer {
    Vector4D position;
    Vector4D color;
    float scale;
    float aa;
    float aaa;
    float aaaa;
  };
  struct DR_PARTICLES_EXPORT GPUParticleSystemCBuff {
    Vector4D m_globalAcceleration;
    Vector4D m_initialColor;
    Vector4D m_finalColor;
    float m_particleMaxLife;
    float dt;
    int m_particlesToEmit;
    int  aliveParticles;
    float m_initialScale;
    float m_finaleScale;
    Int32 m_maxParticles;
    float aaaa;
  };
  struct DR_PARTICLES_EXPORT ParticleEmitterAttributes {
    bool m_isActive = false;
    bool m_loop = true;
    bool m_localSpace = false;
    UInt32 m_maxParticles = 0;
    UInt32 m_numParticlesToEmit = 0;

    float m_initialTime = 0.0f;
    float m_rate = 0.0f;
    float m_trailDist = 0.0f;
    float m_particleMaxLife = 0.0f;
    float m_systemMaxLife = 0.0f;

    float m_rithm;
    Vector3D m_initialVelocity;
    Vector3D m_position;
  };
  class DR_PARTICLES_EXPORT ParticleUpdater {
  public:
    FORCEINLINE virtual void update(float dt, size_t start, size_t end, Particle* p, const ParticleEmitterAttributes& attr) = 0;
  };
  class DR_PARTICLES_EXPORT ParticleGenerator {
  public:
    FORCEINLINE virtual void generate(size_t start, size_t end, Particle* p) = 0;
  };

  class DR_PARTICLES_EXPORT BoxGenerator : public ParticleGenerator {
  public:
    FORCEINLINE void generate(size_t start, size_t end, Particle* p) override;
    Vector3D m_initialPositionRandomMin{ 0,0,0 };
    Vector3D m_initialPositionRandomMax{ 0,0,0 };
  };
  class DR_PARTICLES_EXPORT SphereGenerator : public ParticleGenerator {
  public:
    FORCEINLINE void generate(size_t start, size_t end, Particle* p) override;
    float m_radiusRandomMax{0};
    float m_radiusRandomMin{0};
    Vector3D m_centerPosition{0,0,0};
  };

  class DR_PARTICLES_EXPORT RandomVelocityGenerator : public ParticleGenerator {
  public:
    FORCEINLINE void generate(size_t start, size_t end, Particle* p) override;
    Vector3D m_initialVelocityRandomMin{ 0,0,0 };
    Vector3D m_initialVelocityRandomMax{ 0,0,0 };
  };
  class DR_PARTICLES_EXPORT RandomScaleFactorGenerator : public ParticleGenerator {
  public:
    FORCEINLINE void generate(size_t start, size_t end, Particle* p) override;
    float m_scaleFactorRandomMin{ 1 };
    float m_scaleFactorRandomMax{ 1 };
  };
  class DR_PARTICLES_EXPORT TimeColorUpdater : public ParticleUpdater {
  public:
    FORCEINLINE void update(float dt, size_t start, size_t end, Particle* p, const ParticleEmitterAttributes& attr) override;
    Vector3D m_initialColor{ 0,0,0 };
    Vector3D m_finalColor{ 0,0,0 };
  };
  class DR_PARTICLES_EXPORT TimeScaleUpdater : public ParticleUpdater {
  public:
    FORCEINLINE void update(float dt, size_t start, size_t end, Particle* p, const ParticleEmitterAttributes& attr) override;
    float m_initialScale{ 0 };
    float m_finaleScale{ 0 };
  };
  class DR_PARTICLES_EXPORT EulerUpdater : public ParticleUpdater {
  public:
    FORCEINLINE void update(float dt, size_t start, size_t end, Particle* p, const ParticleEmitterAttributes& attr) override;
    FORCEINLINE void addForce(Vector3D _force);
    Vector3D m_globalAcceleration{ 0, -0, 0 };
    float m_gravity{0};
  };
  class DR_PARTICLES_EXPORT VelocityLimiter : public ParticleUpdater {
  public:
    FORCEINLINE void update(float dt, size_t start, size_t end, Particle* p, const ParticleEmitterAttributes& attr) override;
    float m_initialSpeedLimit{ 0 };
    float m_finalSpeedLimit{ 0 };
  };

  class DR_PARTICLES_EXPORT AttractorUpdater : public ParticleUpdater {
  public:
    FORCEINLINE void update(float dt, size_t start, size_t end, Particle* p, const ParticleEmitterAttributes& attr) override;
    float m_atractionForce{ 0 };
    Vector3D m_position{ 0, 0, 0 };
    float m_radius;
  };
  class DR_PARTICLES_EXPORT RepellerUpdater : public ParticleUpdater {
  public:
    FORCEINLINE void update(float dt, size_t start, size_t end, Particle* p, const ParticleEmitterAttributes& attr) override;
    float m_repellerForce{ 0 };
    Vector3D m_position{ 0, 0, 0 };
    float m_radius;
  };

  class DR_PARTICLES_EXPORT ParticleEmitter {
  public:
    static const Int32 MAX_PARTICLES = 10000000;
    void
      init(const ParticleEmitterAttributes& _attributes);
    void
      update();
    void
      emit();
    Particle m_particles;
    size_t m_aliveParticles{0};

    std::vector<ParticleUpdater*> m_updaters;
    std::vector<ParticleGenerator*> m_generator;
    Vector3D m_position;
    Vector3D m_rotation{0,0,0};
    Matrix4x4 m_localTransform;

    StructureBuffer* m_poolBuffer;
    StructureBuffer* m_aliveBuffer;
    StructureBuffer* m_renderBuffer;
    ConstantBuffer* m_cbufferAliveCount;

    IndirectArgsBuffer* m_drawIndirectBuffer;
    IndirectArgsBuffer* m_updateIndirectBuffer;
  private:
    ParticleEmitterAttributes m_attributes;
    float m_lifeTime = 0.0f;
    float m_timeAccum = 0.0f;

    struct inBuff{
      //Vector4D acceleration;
      Vector4D color;
    };
    struct outBuff {
      //Vector4D position;
      Vector4D color;
    };

    Shader* m_initCS;
    Shader* m_updateCS;
    Shader* m_emitCS;
    Shader* m_setupDrawArgsCS;
    StructureBuffer* m_deadBuffer;
    ConstantBuffer* m_cbuffer;
    ConstantBuffer* m_cbufferDT;
    ConstantBuffer* m_cbufferDeadCount;
    GPUParticleSystemCBuff m_cpuCbuff;
    RenderStructureBuffer* m_cpuRenderBuffer;
  };
}