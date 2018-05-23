#include "dr_particle_emitter.h"
#include <dr_time.h>
#include <dr_random.h>
#include <dr_camera_manager.h>
#include <dr_camera.h>
namespace driderSDK {
void 
ParticleEmitter::init(const ParticleEmitterAttributes & _attributes)
{
  m_buffer = new CBuffer[MAX_PARTICLES]; //Memn leak xdxdxd
  m_forces.push_back(Vector3D(0,-160.81,0));
  m_attributes = _attributes;
  m_lifeTime = m_attributes.m_initialTime;
  m_timeAccum = m_attributes.m_initialTime;
  m_particles.init(m_attributes.m_maxParticles);

  Particle particle;
  particle.m_isActive = false;
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
  if (m_lifeTime >= m_attributes.m_systemMaxLife) {
    m_attributes.m_isActive = false;
    for (size_t i = 0; i < m_particles.size(); ++i) {
      m_particles[i].m_isActive = false;
    }
    return;
  }
  float tm = Time::getDelta();
  m_lifeTime += tm;
  m_timeAccum += tm;
  _forcesSum *= 0;
  for (auto &it : m_forces) {
    _forcesSum += it;
  }
  m_bufferSize = 0;
  for (size_t i = 0; i < m_particles.size(); ++i) {
    Particle& p = m_particles[i];
    if (p.m_isActive) {
      _proportion = p.m_lifeTime * _proportionMul;
      _speedLimitMax = Math::lerp(m_attributes.m_initialSpeedLimit, m_attributes.m_finalSpeedLimit, _proportion);
      //consumir tiempo
      if (p.m_lifeTime > m_attributes.m_particleMaxLife)
        p.m_isActive = false;
      p.m_lifeTime += tm;
      //fuerzas
      p.m_acceleration = _forcesSum;
      p.m_velocity += p.m_acceleration * tm;
      //Limits
      float speed = p.m_velocity.length();
      if (speed > _speedLimitMax) {
        p.m_velocity = p.m_velocity.normalize() * _speedLimitMax;
      }
      //cambios
      p.m_position += p.m_velocity * tm;
      p.m_color = Math::lerp(m_attributes.m_initialColor,m_attributes.m_finalColor,_proportion);
      p.m_scale = Math::lerp(m_attributes.m_initialScale, m_attributes.m_finaleScale, _proportion) * p.m_scaleFactor;

      //BUFFER
      _trensform.identity();
      //Scale
      _trensform.vector0.x = p.m_scale;
      _trensform.vector1.y = p.m_scale;
      _trensform.vector2.z = p.m_scale;
      //Rotation
      //trensform.Rotation(p.m_rotation.x, p.m_rotation.y, p.m_rotation.z);
      //Traslation
      _trensform.vector3 = p.m_position;
      _trensform.vector3.w = 1.0;

      m_buffer[m_bufferSize].WVP = _trensform * CameraManager::getActiveCamera()->getVP();
      m_buffer[m_bufferSize].color = p.m_color;
      m_bufferSize++;
    }
  }
  emit();
}
void 
ParticleEmitter::emit()
{
  if (m_timeAccum >= m_attributes.m_rate) {
    m_timeAccum -= m_attributes.m_rate;
    Particle particle;
    particle.m_rotation = m_attributes.m_rotation;
    particle.m_color = m_attributes.m_initialColor;
    particle.m_scale = m_attributes.m_initialScale;
    particle.m_isActive = true;
    particle.m_lifeTime = 0.0f;
    Particle* mem = m_particles.allocate(m_attributes.m_numParticlesToEmit, std::move(particle));
    for (size_t i = 0; i < m_attributes.m_numParticlesToEmit; ++i) {
      mem[i].m_velocity = Random::RandomRange(m_attributes.m_initialVelocityRandomMin,
          m_attributes.m_initialVelocityRandomMax);
      mem[i].m_position = Random::RandomRange(m_attributes.m_initialPositionRandomMin,
          m_attributes.m_initialPositionRandomMax);
      mem[i].m_scaleFactor = Random::RandomRange(m_attributes.m_scaleFactorRandomMin,
        m_attributes.m_scaleFactorRandomMax);
    }
  }
}
}