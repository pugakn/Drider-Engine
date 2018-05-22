#pragma once
#include "dr_particles_prerequisites.h"
#include <dr_vector3d.h>
#include <dr_pool.h>
#include <vector>

#include <iostream>
namespace driderSDK {
  struct DR_PARTICLES_EXPORT Particle {
    float m_lifeTime = 0.0f;
    bool m_isActive = false;
    Vector3D m_position;
    Vector3D m_rotation;
    float m_scale;
    float m_mass;
    Vector3D m_color;

    Vector3D m_velocity;
    Vector3D m_acceleration;

    Particle() = default;
    Particle(const Particle& p) = default;
    ~Particle(){}
  };
  //struct DR_PARTICLES_EXPORT ParticleAttributes {
  //  float m_particleMaxLife = 0.0f;
  //  bool m_isActive = false;

  //  float m_particleMass{ 0 };
  //};
  struct DR_PARTICLES_EXPORT ParticleEmitterAttributes {
    UInt32 m_maxParticles = 0;

    float m_initialTime = 0.0f;
    float m_rate = 0.0f; 
    float m_trailDist = 0.0f;
    float m_particleMaxLife = 0.0f;
    float m_systemMaxLife = 0.0f; //imp
    bool m_isActive = false;

    Vector3D m_initialVelocity{ 0,0,0 };
    Vector3D m_velocityRandomMin{ 0,0,0 };
    Vector3D m_velocityRandomMax{ 0,0,0 };
    float m_initialSpeedLimit{ 0 };
    float m_finalSpeedLimit{ 0 };

    float m_initialScale{ 0 };
    float m_finaleScale{ 0 };
    float m_scaleRandomMin{ 0 };
    float m_scaleRandomMax{ 0 };

    Vector3D m_rotationRandomMin{ 0,0,0 };
    Vector3D m_rotationRandomMax{ 0,0,0 };

    Vector3D m_position{ 0,0,0 };
    Vector3D m_rotation{ 0,0,0 };
    Vector3D m_positionRandomMin{ 0,0,0 };
    Vector3D m_positionRandomMax{ 0,0,0 };

    Vector3D m_initialColor{ 0,0,0 };
    Vector3D m_finalColor{ 0,0,0 };
    Vector3D m_colorRandomMin{ 0,0,0 };
    Vector3D m_colorRandomMax{ 0,0,0 };

    UInt32 m_numParticlesToEmit = 0;
    float m_particleMass{ 0 };
    //Volumen/Area
    //Evento
    //Factores aleatorios
  };

  class DR_PARTICLES_EXPORT ParticleEmitter {
  public:
    void 
    init(const ParticleEmitterAttributes& _attributes);
    void
    update();
    void
    emit();
    
    PoolAllocator<Particle> m_particles;
  private:
    ParticleEmitterAttributes m_attributes;
    std::vector<Vector3D> m_forces;
    float m_lifeTime = 0.0f;
    float m_timeAccum = 0.0f;

    float _proportionMul;
    float _proportion;
    float _speedLimitMax;

    Vector3D _forcesSum;
  };
}