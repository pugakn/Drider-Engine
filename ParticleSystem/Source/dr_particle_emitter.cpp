#include "dr_particle_emitter.h"
#include <dr_time.h>
#include <dr_random.h>
namespace driderSDK {
  void 
  ParticleEmitter::init(const ParticleEmitterAttributes & _attributes)
  {
    m_forces.push_back(Vector3D(0,-160.81,0));
    m_attributes = _attributes;
    m_lifeTime = m_attributes.m_initialTime;
    m_timeAccum = m_attributes.m_initialTime;
    m_particles.init(m_attributes.m_maxParticles);

    Particle particle;
    particle.m_velocity = m_attributes.m_initialVelocity;
    particle.m_position = m_attributes.m_position;
    particle.m_color = m_attributes.m_initialColor;
    particle.m_isActive = false;
    particle.m_lifeTime = 0.0f;
    particle.m_rotation = m_attributes.m_rotation;
    particle.m_scale = m_attributes.m_initialScale;
    m_particles.allocate(m_attributes.m_maxParticles, particle);
    _proportionMul = 1.0f / m_attributes.m_particleMaxLife;
    emit();
  }
  void 
  ParticleEmitter::update()
  {
    if (!m_attributes.m_isActive) {
      return;
    }
    if (m_lifeTime >= m_attributes.m_systemMaxLife)
    {
      m_attributes.m_isActive = false;
      for (size_t i = 0; i < m_particles.size(); ++i) {
        m_particles[i].m_isActive = false;
      }
      return;
    }
    float tm = Time::getDelta();
    m_lifeTime += tm;
    m_timeAccum += tm;
    //Update particles
    _forcesSum *= 0;
    for (auto &it : m_forces) {
      _forcesSum += it;
    }

    for (size_t i = 0; i < m_particles.size(); ++i) {
      Particle& p = m_particles[i];
      if (p.m_isActive) {
        _proportion = p.m_lifeTime * _proportionMul;
        _speedLimitMax = Math::lerp(m_attributes.m_initialSpeedLimit, m_attributes.m_finalSpeedLimit, _proportion);
        //consumir tiempo
        if (p.m_lifeTime > m_attributes.m_particleMaxLife)
        {
          p.m_isActive = false;
        }
        p.m_lifeTime += tm;
        //fuerzas
        p.m_acceleration = _forcesSum;
        //cambios
        p.m_velocity += p.m_acceleration * tm;
        //Limits
        float speed = p.m_velocity.length();
        if (speed > _speedLimitMax) {
          p.m_velocity = p.m_velocity.normalize() * _speedLimitMax;
        }
        //cambios
        p.m_position += p.m_velocity * tm;
        p.m_color = Math::lerp(m_attributes.m_initialColor,m_attributes.m_finalColor,_proportion);
        p.m_scale = Math::lerp(m_attributes.m_initialScale, m_attributes.m_finaleScale, _proportion);
      }
    }
    //Emit particles
    emit();
  }
  void 
  ParticleEmitter::emit()
  {
    if (m_timeAccum >= m_attributes.m_rate) {
      m_timeAccum = 0.0f;
      //Emit particles
      for (size_t i = 0; i < m_attributes.m_numParticlesToEmit; ++i) {
        Particle particle;
        particle.m_velocity = m_attributes.m_initialVelocity +
          Random::RandomRange(m_attributes.m_velocityRandomMin,
            m_attributes.m_velocityRandomMax);
        particle.m_position = m_attributes.m_position +
          Random::RandomRange(m_attributes.m_positionRandomMin,
            m_attributes.m_positionRandomMax);
        particle.m_color = m_attributes.m_initialColor +
          Random::RandomRange(m_attributes.m_colorRandomMin,
            m_attributes.m_colorRandomMax);
        particle.m_scale = m_attributes.m_initialScale +
          Random::RandomRange(m_attributes.m_scaleRandomMin,
            m_attributes.m_scaleRandomMax);
        particle.m_rotation = m_attributes.m_rotation +
          Random::RandomRange(m_attributes.m_rotationRandomMin,
            m_attributes.m_rotationRandomMax);
        particle.m_isActive = true;
        particle.m_lifeTime = 0.0f;
        particle.m_mass = m_attributes.m_particleMass;
        m_particles.allocate(1, std::move(particle));
      }
    }
  }
}