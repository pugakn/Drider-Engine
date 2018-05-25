#include "dr_particle_emitter.h"
#include <dr_time.h>
#include <dr_random.h>
#include <dr_camera_manager.h>
#include <dr_camera.h>

namespace driderSDK {
  void
    ParticleEmitter::init(const ParticleEmitterAttributes & _attributes)
  {
    ParticleUpdater* pU = new EulerUpdater();
    m_updaters.push_back(pU);
    pU = new TimeColorUpdater();
    ((TimeColorUpdater*)pU)->m_initialColor = (Vector3D(1, 1, 0));
    ((TimeColorUpdater*)pU)->m_finalColor = (Vector3D(0, 1, 1));
    m_updaters.push_back(pU);
    pU = new TimeScaleUpdater();
    ((TimeScaleUpdater*)pU)->m_initialScale = 10;
    ((TimeScaleUpdater*)pU)->m_finaleScale = 1;
    m_updaters.push_back(pU);
    pU = new VelocityLimiter();
    ((VelocityLimiter*)pU)->m_initialSpeedLimit = 1200;
    ((VelocityLimiter*)pU)->m_finalSpeedLimit = 5;
    m_updaters.push_back(pU);

    ParticleGenerator* pG = new BoxGenerator();
    ((BoxGenerator*)pG)->m_initialPositionRandomMin = (Vector3D(0, 0, 0));
    ((BoxGenerator*)pG)->m_initialPositionRandomMax = (Vector3D(0, 0, 0));
    m_generator.push_back(pG);
    pG = new RandomVelocityGenerator();
    ((RandomVelocityGenerator*)pG)->m_initialVelocityRandomMin = (Vector3D(-1200, -1200, -1200));
    ((RandomVelocityGenerator*)pG)->m_initialVelocityRandomMax = (Vector3D(1200, 1200, 1200));
    m_generator.push_back(pG);
    pG = new RandomScaleFactorGenerator();
    ((RandomScaleFactorGenerator*)pG)->m_scaleFactorRandomMin = 0.8;
    ((RandomScaleFactorGenerator*)pG)->m_scaleFactorRandomMax = 5;
    m_generator.push_back(pG);

    m_buffer = new CBuffer[MAX_PARTICLES]; //Memn leak xdxdxd
    m_forces.push_back(Vector3D(0, -160.81, 0));
    m_attributes = _attributes;
    m_lifeTime = m_attributes.m_initialTime;
    m_timeAccum = m_attributes.m_initialTime;
    //m_particles.init(m_attributes.m_maxParticles);

    m_particles.m_acceleration = new Vector3D[m_attributes.m_maxParticles];
    m_particles.m_color = new Vector3D[m_attributes.m_maxParticles];
    m_particles.m_isActive = new bool[m_attributes.m_maxParticles];
    m_particles.m_lifeTime = new float[m_attributes.m_maxParticles];
    m_particles.m_position = new Vector3D[m_attributes.m_maxParticles];
    m_particles.m_rotation = new Vector3D[m_attributes.m_maxParticles];
    m_particles.m_scale = new float[m_attributes.m_maxParticles];
    m_particles.m_scaleFactor = new float[m_attributes.m_maxParticles];
    m_particles.m_velocity = new Vector3D[m_attributes.m_maxParticles];


    //Particle particle;
    //particle.m_isActive = false;
    //m_particles.allocate(m_attributes.m_maxParticles, particle);
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
      for (size_t i = 0; i < m_attributes.m_maxParticles; ++i) {
        m_particles.m_isActive[i] = false;
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
      //Particle* __restrict p = &m_particles[i];
      //consumir tiempo
      if (m_particles.m_lifeTime[i] > m_attributes.m_particleMaxLife)
      {
        m_particles.m_isActive[i] = false;
        m_particles.m_acceleration[i] = {0,0,0};
        m_particles.m_velocity[i] = { 0,0,0 };
        m_particles.m_color[i] = { 0,0,0 };
        m_particles.m_position[i] = { 0,0,0 };
        m_particles.m_rotation[i] = { 0,0,0 };
        m_particles.m_scale[i] = { 0 };
        m_particles.m_scaleFactor[i] = { 0 };

        std::swap(m_particles.m_acceleration[i], m_particles.m_acceleration[m_aliveParticles - 1]);
        std::swap(m_particles.m_color[i], m_particles.m_color[m_aliveParticles - 1]);
        std::swap(m_particles.m_isActive[i], m_particles.m_isActive[m_aliveParticles - 1]);
        std::swap(m_particles.m_lifeTime[i], m_particles.m_lifeTime[m_aliveParticles - 1]);
        std::swap(m_particles.m_position[i], m_particles.m_position[m_aliveParticles - 1]);
        std::swap(m_particles.m_rotation[i], m_particles.m_rotation[m_aliveParticles - 1]);
        std::swap(m_particles.m_scale[i], m_particles.m_scale[m_aliveParticles - 1]);
        std::swap(m_particles.m_scaleFactor[i], m_particles.m_scaleFactor[m_aliveParticles - 1]);
        std::swap(m_particles.m_velocity[i], m_particles.m_velocity[m_aliveParticles - 1]);
        m_aliveParticles--;
        continue;
      }
      m_particles.m_lifeTime[i] += tm;
    }
    for (auto &it : m_updaters) {
      it->update(0, m_aliveParticles, &m_particles, m_attributes);
    }
    for (size_t i = 0; i < m_aliveParticles; ++i) {
      //BUFFER
      m_buffer[i].WVP.identity();
      //Scale
      m_buffer[i].WVP.vector0.x = m_particles.m_scale[i];
      m_buffer[i].WVP.vector1.y = m_particles.m_scale[i];
      m_buffer[i].WVP.vector2.z = m_particles.m_scale[i];
      //Rotation
      //trensform.Rotation(p.m_rotation.x, p.m_rotation.y, p.m_rotation.z);
      //Traslation
      m_buffer[i].WVP.vector3 = m_particles.m_position[i];
      m_buffer[i].WVP.vector3.w = 1.0;
      m_buffer[i].WVP = m_buffer[i].WVP * CameraManager::getActiveCamera()->getVP();
      m_buffer[i].color = m_particles.m_color[i];
    }
    emit();
  }
  void
    ParticleEmitter::emit()
  {
    if (m_timeAccum > m_attributes.m_rate) {
      m_timeAccum -= m_attributes.m_rate;
      size_t endID = std::min(m_aliveParticles + m_attributes.m_numParticlesToEmit, m_attributes.m_maxParticles);
      for (auto &it : m_generator) {
        it->generate(m_aliveParticles, endID, &m_particles);
      }
      for (size_t i = m_aliveParticles; i < endID; ++i) {
        m_particles.m_lifeTime[i] = 0.0f;
        m_particles.m_isActive[i] = true;
        m_aliveParticles++;
      }
    }
  }
  void BoxGenerator::generate(size_t start, size_t end, Particle * p)
  {
    [[unroll]]
    for (size_t i = start; i < end; ++i) {
      p->m_position[i] = Random::RandomRange(m_initialPositionRandomMin,
        m_initialPositionRandomMax);
    }
  }
  void RandomVelocityGenerator::generate(size_t start, size_t end, Particle * p)
  {
    [[unroll]]
    for (size_t i = start; i < end; ++i) {
      p->m_velocity[i] = Random::RandomRange(m_initialVelocityRandomMin,
        m_initialVelocityRandomMax);
    }
  }
  void TimeColorUpdater::update(size_t start, size_t end, Particle * p, const ParticleEmitterAttributes & attr)
  {
    float _proportionMul = 1.0f / attr.m_particleMaxLife;
    //end /= 16;
    [[unroll]]
    for (size_t i = start; i < end; ++i ) {
      float _proportion = p->m_lifeTime[i] * _proportionMul;
      float m1Proportion = (1.0f - _proportion);

      p->m_color[i] = (m_initialColor * m1Proportion) + (m_finalColor * _proportion);
      //p->m_color[i+1] = (m_initialColor * m1Proportion) + (m_finalColor * _proportion);
      //p->m_color[i+2] = (m_initialColor * m1Proportion) + (m_finalColor * _proportion);
      //p->m_color[i+3] = (m_initialColor * m1Proportion) + (m_finalColor * _proportion);
      //p->m_color[i+4] = (m_initialColor * m1Proportion) + (m_finalColor * _proportion);
      //p->m_color[i+5] = (m_initialColor * m1Proportion) + (m_finalColor * _proportion);
      //p->m_color[i + 6] = (m_initialColor * m1Proportion) + (m_finalColor * _proportion);
      //p->m_color[i + 7] = (m_initialColor * m1Proportion) + (m_finalColor * _proportion);
      //p->m_color[i + 8] = (m_initialColor * m1Proportion) + (m_finalColor * _proportion);
      //p->m_color[i + 9] = (m_initialColor * m1Proportion) + (m_finalColor * _proportion);
      //p->m_color[i + 10] = (m_initialColor * m1Proportion) + (m_finalColor * _proportion);
      //p->m_color[i + 11] = (m_initialColor * m1Proportion) + (m_finalColor * _proportion);
      //p->m_color[i + 12] = (m_initialColor * m1Proportion) + (m_finalColor * _proportion);
      //p->m_color[i + 13] = (m_initialColor * m1Proportion) + (m_finalColor * _proportion);
      //p->m_color[i + 14] = (m_initialColor * m1Proportion) + (m_finalColor * _proportion);
      //p->m_color[i + 15] = (m_initialColor * m1Proportion) + (m_finalColor * _proportion);
    }
  }
  void TimeScaleUpdater::update(size_t start, size_t end, Particle * p, const ParticleEmitterAttributes & attr)
  {
      float _proportionMul = 1.0f / attr.m_particleMaxLife;
      //end /= 16;
      [[unroll]]
      for (size_t i = start; i < end; i = (i + 1) ) {
        float _proportion = p->m_lifeTime[i] * _proportionMul;
        float m1Proportion = (1.0f - _proportion);
        p->m_scale[i] = (m_initialScale * m1Proportion) + (m_finaleScale * _proportion) * p->m_scaleFactor[i];
      //p->m_scale[i+1] = (m_initialScale * m1Proportion) + (m_finaleScale * _proportion) * p->m_scaleFactor[i+1];
      //p->m_scale[i+2] = (m_initialScale * m1Proportion) + (m_finaleScale * _proportion) * p->m_scaleFactor[i+2];
      //p->m_scale[i+3] = (m_initialScale * m1Proportion) + (m_finaleScale * _proportion) * p->m_scaleFactor[i+3];
      //p->m_scale[i+4] = (m_initialScale * m1Proportion) + (m_finaleScale * _proportion) * p->m_scaleFactor[i+4];
      //p->m_scale[i+5] = (m_initialScale * m1Proportion) + (m_finaleScale * _proportion) * p->m_scaleFactor[i+5];
      //p->m_scale[i+6] = (m_initialScale * m1Proportion) + (m_finaleScale * _proportion) * p->m_scaleFactor[i+6];
      //p->m_scale[i + 7] = (m_initialScale * m1Proportion) + (m_finaleScale * _proportion) * p->m_scaleFactor[i + 7];
      //p->m_scale[i + 8] = (m_initialScale * m1Proportion) + (m_finaleScale * _proportion) * p->m_scaleFactor[i + 8];
      //p->m_scale[i + 9] = (m_initialScale * m1Proportion) + (m_finaleScale * _proportion) * p->m_scaleFactor[i + 9];
      //p->m_scale[i + 10] = (m_initialScale * m1Proportion) + (m_finaleScale * _proportion) * p->m_scaleFactor[i + 10];
      //p->m_scale[i + 11] = (m_initialScale * m1Proportion) + (m_finaleScale * _proportion) * p->m_scaleFactor[i + 11];
      //p->m_scale[i + 12] = (m_initialScale * m1Proportion) + (m_finaleScale * _proportion) * p->m_scaleFactor[i + 12];
      //p->m_scale[i + 13] = (m_initialScale * m1Proportion) + (m_finaleScale * _proportion) * p->m_scaleFactor[i + 13];
      //p->m_scale[i + 14] = (m_initialScale * m1Proportion) + (m_finaleScale * _proportion) * p->m_scaleFactor[i + 14];
      //p->m_scale[i + 15] = (m_initialScale * m1Proportion) + (m_finaleScale * _proportion) * p->m_scaleFactor[i + 15];
    }
  }
  void EulerUpdater::update(size_t start, size_t end, Particle * p, const ParticleEmitterAttributes & attr)
  {
    //std::cout << start << "   +++    " << end << std::endl;
    float tm = Time::getDelta();
    //Int32 mod = end % 16;
    //end = (Int32)end / 16;
    [[unroll]]
    for (size_t i = start; i < end; ++i) {

      //for (size_t i = start; i < end; ++i) {
      p->m_velocity[i] += m_Acceleration * tm;
      //p->m_velocity[i+1] += m_Acceleration * tm;
      //p->m_velocity[i+2] += m_Acceleration * tm;
      //p->m_velocity[i+3] += m_Acceleration * tm;
      //p->m_velocity[i+4] += m_Acceleration * tm;
      //p->m_velocity[i+5] += m_Acceleration * tm;
      //p->m_velocity[i + 6] += m_Acceleration * tm;
      //p->m_velocity[i + 7] += m_Acceleration * tm;
      //p->m_velocity[i + 8] += m_Acceleration * tm;
      //p->m_velocity[i + 9] += m_Acceleration * tm;
      //p->m_velocity[i + 10] += m_Acceleration * tm;
      //p->m_velocity[i + 11] += m_Acceleration * tm;
      //p->m_velocity[i + 12] += m_Acceleration * tm;
      //p->m_velocity[i + 13] += m_Acceleration * tm;
      //p->m_velocity[i + 14] += m_Acceleration * tm;
      //p->m_velocity[i + 15] += m_Acceleration * tm;

      p->m_position[i] += p->m_velocity[i] * tm;
      //p->m_position[i + 1] += p->m_velocity[i+1] * tm;
      //p->m_position[i + 2] += p->m_velocity[i+2] * tm;
      //p->m_position[i + 3] += p->m_velocity[i+3] * tm;
      //p->m_position[i + 4] += p->m_velocity[i+4] * tm;
      //p->m_position[i + 5] += p->m_velocity[i+5] * tm;
      //p->m_position[i + 6] += p->m_velocity[i+6] * tm;
      //p->m_position[i + 7] += p->m_velocity[i+7] * tm;
      //p->m_position[i + 8] += p->m_velocity[i+8] * tm;
      //p->m_position[i + 9] += p->m_velocity[i+9] * tm;
      //p->m_position[i + 10] += p->m_velocity[i+10] * tm;
      //p->m_position[i + 11] += p->m_velocity[i+11] * tm;
      //p->m_position[i + 12] += p->m_velocity[i+12] * tm;
      //p->m_position[i + 13] += p->m_velocity[i+13] * tm;
      //p->m_position[i + 14] += p->m_velocity[i+14] * tm;
      //p->m_position[i + 15] += p->m_velocity[i+15] * tm;
    }
    /*Int32 index = end * 16;
    switch ((Int32)mod)
    {
    case 15:
      p->m_velocity[index + mod] += m_Acceleration * tm;
      p->m_position[index + mod] += p->m_velocity[index + mod] * tm;
      --mod;
    case 14:
      p->m_velocity[index + mod] += m_Acceleration * tm;
      p->m_position[index + mod] += p->m_velocity[index + mod] * tm;
      --mod;
    case 13:
      p->m_velocity[index + mod] += m_Acceleration * tm;
      p->m_position[index + mod] += p->m_velocity[index + mod] * tm;
      --mod;
    case 12:
      p->m_velocity[index + mod] += m_Acceleration * tm;
      p->m_position[index + mod] += p->m_velocity[index + mod] * tm;
      --mod;
    case 11:
      p->m_velocity[index + mod] += m_Acceleration * tm;
      p->m_position[index + mod] += p->m_velocity[index + mod] * tm;
    case 10:
      p->m_velocity[index + mod] += m_Acceleration * tm;
      p->m_position[index + mod] += p->m_velocity[index + mod] * tm;
      --mod;
    case 9:
      p->m_velocity[index + mod] += m_Acceleration * tm;
      p->m_position[index + mod] += p->m_velocity[index + mod] * tm;
      --mod;
    case 8:
      p->m_velocity[index + mod] += m_Acceleration * tm;
      p->m_position[index + mod] += p->m_velocity[index + mod] * tm;
      --mod;
    case 7:
      p->m_velocity[index + mod] += m_Acceleration * tm;
      p->m_position[index + mod] += p->m_velocity[index + mod] * tm;
      --mod;
    case 6:
      p->m_velocity[index + mod] += m_Acceleration * tm;
      p->m_position[index + mod] += p->m_velocity[index + mod] * tm;
      --mod;
    case 5:
      p->m_velocity[index + mod] += m_Acceleration * tm;
      p->m_position[index + mod] += p->m_velocity[index + mod] * tm;
      --mod;
    case 4:
      p->m_velocity[index + mod] += m_Acceleration * tm;
      p->m_position[index + mod] += p->m_velocity[index + mod] * tm;
      --mod;
    case 3:
      p->m_velocity[index + mod] += m_Acceleration * tm;
      p->m_position[index + mod] += p->m_velocity[index + mod] * tm;
      --mod;
    case 2:
      p->m_velocity[index + mod] += m_Acceleration * tm;
      p->m_position[index + mod] += p->m_velocity[index + mod] * tm;
      --mod;
    case 1:
      p->m_velocity[index + mod] += m_Acceleration * tm;
      p->m_position[index + mod] += p->m_velocity[index + mod] * tm;
      --mod;
      break;
    }*/
  }
  void RandomScaleFactorGenerator::generate(size_t start, size_t end, Particle * p)
  {
    for (size_t i = start; i < end; ++i) {
      p->m_scaleFactor[i] = Random::RandomRange(m_scaleFactorRandomMin,
        m_scaleFactorRandomMax);
    }
  }
  void VelocityLimiter::update(size_t start, size_t end, Particle * p, const ParticleEmitterAttributes & attr)
  {
    float _proportionMul = 1.0f / attr.m_particleMaxLife;
    [[unroll]]
    for (size_t i = start; i < end; ++i) {
      float _proportion = p->m_lifeTime[i] * _proportionMul;
      float m1Proportion = (1.0f - _proportion);
      float _speedLimitMax = (m_initialSpeedLimit * m1Proportion) + (m_finalSpeedLimit * _proportion);
      float speed = p->m_velocity[i].length();
      if (speed > _speedLimitMax) {
        p->m_velocity[i] = p->m_velocity[i].normalize() * _speedLimitMax;
      }
    }
  }
}