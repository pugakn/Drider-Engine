#pragma once
#include "dr_particles_prerequisites.h"
#include <dr_vector3d.h>
#include <dr_pool.h>
#include <vector>
#include <dr_matrix4x4.h>
#include <dr_vector4d.h>
#include <iostream>

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
    static const Int32 MAX_PARTICLES = 100000;
    void
      init(const ParticleEmitterAttributes& _attributes);
    void
      update();
    void
      emit();
    Particle m_particles;
    struct CBuffer {
      Matrix4x4 WVP;
      Vector4D color;
    };
    CBuffer* m_buffer;
    size_t m_aliveParticles{0};

    std::vector<ParticleUpdater*> m_updaters;
    std::vector<ParticleGenerator*> m_generator;
    Vector3D m_position;
    Vector3D m_rotation{0,0,0};
    Matrix4x4 m_localTransform;
  private:
    ParticleEmitterAttributes m_attributes;
    float m_lifeTime = 0.0f;
    float m_timeAccum = 0.0f;
  };
}