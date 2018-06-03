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
  static const Int32 MAX_ATTRACTORS = 4;
  static const Int32 MAX_REPELLERS = MAX_ATTRACTORS;
  static const Int32 MAX_COLLIDERS = MAX_ATTRACTORS;

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
    Int32 m_bTimeColorUpdaterActive;
    Int32 m_bTimeScaleUpdaterActive;
    Int32 m_bEulerUpdaterActive;
    Int32 m_bAttractorUpdaterActive;
    Int32 m_bRepellerUpdaterActive;
    Int32 m_bColliderUpdaterActive;
    Int32 m_bBoxGeneratorActive;
    Int32 m_bRandVelocityGeneratorActive;

    //Particle system
    Vector4D m_systemPosition;
    Vector4D m_globalAcceleration;
    //Time Color Updater
    Vector4D m_initialColor;
    Vector4D m_finalColor;
    //Box Generator
    Vector4D m_randomPosMin;
    Vector4D m_randomPosMax;
    //Random Velocity Generator
    Vector4D m_randomVelMin;
    Vector4D m_randomVelMax;
    //Attractors Updater
    Vector4D m_attractorPos[MAX_ATTRACTORS];
    Vector4D m_attractorForceX_radiusY[MAX_ATTRACTORS];
    //Repellers Updater
    Vector4D m_RepellerPos[MAX_REPELLERS];
    Vector4D m_RepellerForceX_radiuusY[MAX_REPELLERS];
    ////Plane Collition Updater

    //Time Scale Updater
    float m_initialScale;
    float m_finaleScale;

    float m_particleMaxLife;
    Int32 m_particlesToEmit;
    Int32  aliveParticles;
    Int32 m_maxParticles;
    Int32 m_numAttractors;
    Int32 m_numRepellers;
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
    bool m_bActive{false};
  protected:
    virtual void update(float dt, size_t start, size_t end, Particle* p, const ParticleEmitterAttributes& attr) = 0;
  };
  class DR_PARTICLES_EXPORT ParticleGenerator {
  public:
    bool m_bActive{false};
  protected:
    virtual void generate(size_t start, size_t end, Particle* p) = 0;
  };

  class DR_PARTICLES_EXPORT BoxGenerator : public ParticleGenerator {
  public:
    Vector3D m_initialPositionRandomMin{ 0,0,0 };
    Vector3D m_initialPositionRandomMax{ 0,0,0 };
  private:
    friend class ParticleEmitter;
    void generate(size_t start, size_t end, Particle* p) override;
  };
  class DR_PARTICLES_EXPORT SphereGenerator : public ParticleGenerator {
  public:
    float m_radiusRandomMax{0};
    float m_radiusRandomMin{0};
    Vector3D m_centerPosition{0,0,0};
  private:
    friend class ParticleEmitter;
    void generate(size_t start, size_t end, Particle* p) override;
  };

  class DR_PARTICLES_EXPORT RandomVelocityGenerator : public ParticleGenerator {
  public:
    Vector3D m_initialVelocityRandomMin{ 0,0,0 };
    Vector3D m_initialVelocityRandomMax{ 0,0,0 };
  private:
    friend class ParticleEmitter;
    void generate(size_t start, size_t end, Particle* p) override;
  };
  class DR_PARTICLES_EXPORT RandomScaleFactorGenerator : public ParticleGenerator {
  public:
    float m_scaleFactorRandomMin{ 1 };
    float m_scaleFactorRandomMax{ 1 };
  private:
    friend class ParticleEmitter;
    void generate(size_t start, size_t end, Particle* p) override;
  };
  class DR_PARTICLES_EXPORT TimeColorUpdater : public ParticleUpdater {
  public:
    Vector3D m_initialColor{ 0,0,0 };
    Vector3D m_finalColor{ 0,0,0 };
  private:
    friend class ParticleEmitter;
    void update(float dt, size_t start, size_t end, Particle* p, const ParticleEmitterAttributes& attr) override;
  };
  class DR_PARTICLES_EXPORT TimeScaleUpdater : public ParticleUpdater {
  public:
    float m_initialScale{ 0 };
    float m_finaleScale{ 0 };
  private:
    friend class ParticleEmitter;
    void update(float dt, size_t start, size_t end, Particle* p, const ParticleEmitterAttributes& attr) override;
  };
  class DR_PARTICLES_EXPORT EulerUpdater : public ParticleUpdater {
  public:
    float m_gravityScale { 0 };
    Vector3D m_windForce { 0, 0, 0 };
  private:
    friend class ParticleEmitter;
    void update(float dt, size_t start, size_t end, Particle* p, const ParticleEmitterAttributes& attr) override;
    Vector3D m_globalAcceleration{ 0, 0, 0 };
  };
  class DR_PARTICLES_EXPORT VelocityLimiter : public ParticleUpdater {
  public:
    float m_initialSpeedLimit{ 0 };
    float m_finalSpeedLimit{ 0 };
  private:
    friend class ParticleEmitter;
    void update(float dt, size_t start, size_t end, Particle* p, const ParticleEmitterAttributes& attr) override;
  };
  struct DR_PARTICLES_EXPORT Attractor {
    float m_atractionForce{ 0 };
    Vector3D m_position{ 0, 0, 0 };
    float m_radius;
  };
  class DR_PARTICLES_EXPORT AttractorUpdater : public ParticleUpdater {
  public:
    void add(const Vector3D& _pos, float _force, float _radius);
    void remove(Int32 _id);
    Attractor& get(Int32 _id);
    Int32 size();
  private:
    friend class ParticleEmitter;
    void update(float dt, size_t start, size_t end, Particle* p, const ParticleEmitterAttributes& attr) override;
    std::vector<Attractor> m_attractors;
  };
  struct DR_PARTICLES_EXPORT Repeller {
    float m_repellerForce{ 0 };
    Vector3D m_position{ 0, 0, 0 };
    float m_radius;
  };
  class DR_PARTICLES_EXPORT RepellerUpdater : public ParticleUpdater {
  public:
    void add(const Vector3D& _pos, float _force, float _radius);
    void remove(Int32 _id);
    Repeller& get(Int32 _id);
    Int32 size();
  private:
    friend class ParticleEmitter;
    void update(float dt, size_t start, size_t end, Particle* p, const ParticleEmitterAttributes& attr) override;
    std::vector<Repeller> m_repellers;
  };

  class DR_PARTICLES_EXPORT ParticleEmitter {
  public:
    enum GENERATORS {
      kBOX,
      kSPHERE,
      kRANDOM_VELOCITY,
      kGENERATOR_COUNT
    };
    enum UPDATERS {
      kTIME_COLOR,
      kTIME_SCALE,
      kATTRACTORS,
      kREPELLERS,
      kDEPTH_COLLITION,
      kEULER,
      kUPDATER_COUNT
    };
#if (DR_PARTICLES_METHOD == DR_PARTICLES_GPU)
    static const Int32 MAX_PARTICLES = 10000000;
#else
    static const Int32 MAX_PARTICLES = 150000;
#endif
    void
      init(const ParticleEmitterAttributes& _attributes);
    void
      update();
    void
      emit();

    template <typename T>
    T& getUpdater(UPDATERS _updater) {
      return *static_cast<T*>(m_updaters[_updater]);
    }
    template <typename T>
    T& getGenerator(GENERATORS _generator) {
      return *static_cast<T*>(m_generator[_generator]);
    }

    Particle m_particles;
    size_t m_aliveParticles{0};

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

    std::vector<ParticleUpdater*> m_updaters;
    std::vector<ParticleGenerator*> m_generator;
  };
}