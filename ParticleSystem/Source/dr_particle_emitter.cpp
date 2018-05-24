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
      m_aliveParticles = 0;
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
  for (size_t i = 0; i < m_aliveParticles; ++i) {
    Particle* __restrict p = &m_particles[i];
    //consumir tiempo
    if (p->m_lifeTime > m_attributes.m_particleMaxLife)
    {
      p->m_isActive = false;
      std::swap(*p, m_particles[m_aliveParticles - 1]);
      m_aliveParticles--;
      continue;
    }
    _proportion = p->m_lifeTime * _proportionMul;
    float m1Proportion = (1.0f - _proportion);
    _speedLimitMax = (m_attributes.m_initialSpeedLimit * m1Proportion) + (m_attributes.m_finalSpeedLimit * _proportion);
    p->m_lifeTime += tm;
    //fuerzas
    p->m_acceleration = _forcesSum;
    p->m_velocity += p->m_acceleration * tm;
    //Limits
    float speed = p->m_velocity.length();
    if (speed > _speedLimitMax) {
      p->m_velocity = p->m_velocity.normalize() * _speedLimitMax;
    }
    //cambios
    p->m_position += p->m_velocity * tm;
    p->m_scale = (m_attributes.m_initialScale * m1Proportion) + (m_attributes.m_finaleScale * _proportion) * p->m_scaleFactor;

    //BUFFER
    m_buffer[i].color = (m_attributes.m_initialColor * m1Proportion) + (m_attributes.m_finalColor * _proportion);
    m_buffer[i].WVP.identity();
    //Scale
    m_buffer[i].WVP.vector0.x = p->m_scale;
    m_buffer[i].WVP.vector1.y = p->m_scale;
    m_buffer[i].WVP.vector2.z = p->m_scale;
    //Rotation
    //trensform.Rotation(p.m_rotation.x, p.m_rotation.y, p.m_rotation.z);
    //Traslation
    m_buffer[i].WVP.vector3 = p->m_position;
    m_buffer[i].WVP.vector3.w = 1.0;
    m_buffer[i].WVP = m_buffer[i].WVP * CameraManager::getActiveCamera()->getVP();
    //m_buffer[i].color = p.m_color;
  }
  emit();
}
void 
ParticleEmitter::emit()
{
  if (m_timeAccum >= m_attributes.m_rate) {
    m_timeAccum -= m_attributes.m_rate;
    size_t endID = std::min(m_aliveParticles + m_attributes.m_numParticlesToEmit, m_attributes.m_maxParticles);
    for (size_t i = m_aliveParticles; i < endID; ++i) {
      Particle* __restrict p = &m_particles[i];
      p->m_isActive = true;
      p->m_lifeTime = 0.0f;
      p->m_rotation = m_attributes.m_rotation;
      p->m_color = m_attributes.m_initialColor;
      p->m_scale = m_attributes.m_initialScale;
      p->m_velocity = Random::RandomRange(m_attributes.m_initialVelocityRandomMin,
        m_attributes.m_initialVelocityRandomMax);
      p->m_position = Random::RandomRange(m_attributes.m_initialPositionRandomMin,
        m_attributes.m_initialPositionRandomMax);
      p->m_scaleFactor = Random::RandomRange(m_attributes.m_scaleFactorRandomMin,
        m_attributes.m_scaleFactorRandomMax);
      m_aliveParticles++;
    }
  }
}
}