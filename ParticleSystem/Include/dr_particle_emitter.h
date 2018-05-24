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
  Particle() = default;
  Particle(const Particle& p) = default;
  ~Particle() {}

  float m_lifeTime = 0.0f;
  bool m_isActive = false;

  Vector3D m_position;
  Vector3D m_rotation;

  float m_scale;
  float m_scaleFactor;

  Vector3D m_color;

  Vector3D m_velocity;
  Vector3D m_acceleration;
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
  float m_systemMaxLife = 0.0f; 
  bool m_isActive = false;

  Vector3D m_initialPositionRandomMin{ 0,0,0 };
  Vector3D m_initialPositionRandomMax{ 0,0,0 };

  Vector3D m_initialVelocityRandomMin{ 0,0,0 };
  Vector3D m_initialVelocityRandomMax{ 0,0,0 };
  float m_initialSpeedLimit{ 0 };
  float m_finalSpeedLimit{ 0 };

  float m_initialScale{ 0 };
  float m_finaleScale{ 0 };
  float m_scaleFactorRandomMin{ 1 };
  float m_scaleFactorRandomMax{ 1 };

  Vector3D m_rotation{ 0,0,0 };
  Vector3D m_rotationRandomMin{ 0,0,0 };
  Vector3D m_rotationRandomMax{ 0,0,0 };


  Vector3D m_initialColor{ 0,0,0 };
  Vector3D m_finalColor{ 0,0,0 };
  Vector3D m_colorRandomMin{ 0,0,0 };
  Vector3D m_colorRandomMax{ 0,0,0 };

  UInt32 m_numParticlesToEmit = 0;
  //Volumen/Area
  //Evento
};

class DR_PARTICLES_EXPORT ParticleEmitter {
public:
  static const Int32 MAX_PARTICLES = 150000;//65536;
  void 
  init(const ParticleEmitterAttributes& _attributes);
  void
  update();
  void
  emit();
    
  PoolAllocator<Particle> m_particles;
  struct CBuffer {
    Matrix4x4 WVP;
    Vector4D color;
  };
  CBuffer* m_buffer;
  size_t m_aliveParticles = 0;
private:
  ParticleEmitterAttributes m_attributes;
  std::vector<Vector3D> m_forces;
  float m_lifeTime = 0.0f;
  float m_timeAccum = 0.0f;

  //Internal
  float _proportionMul;
  float _proportion;
  float _speedLimitMax;
  Vector3D _forcesSum;
};
}