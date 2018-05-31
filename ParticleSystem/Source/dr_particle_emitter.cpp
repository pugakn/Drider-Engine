#include "dr_particle_emitter.h"
#include <dr_time.h>
#include <dr_random.h>
#include <dr_camera_manager.h>
#include <dr_camera.h>
#include <dr_device.h>
#include <dr_device_context.h>
#include <dr_graphics_api.h>
#include <dr_file.h>
#include <dr_string_utils.h>
namespace driderSDK {
  void
    ParticleEmitter::init(const ParticleEmitterAttributes & _attributes)
  {
    //TEST
    Device& device = GraphicsAPI::getDevice();
    //
    ParticleUpdater* pU;
    pU = new TimeColorUpdater();
    ((TimeColorUpdater*)pU)->m_initialColor = (Vector3D(1, 0, 0));
    ((TimeColorUpdater*)pU)->m_finalColor = (Vector3D(0, 0, 1));
    m_updaters.push_back(pU);
    pU = new TimeScaleUpdater();
    ((TimeScaleUpdater*)pU)->m_initialScale = 5;
    ((TimeScaleUpdater*)pU)->m_finaleScale = 0;
    m_updaters.push_back(pU);
    //pU = new AttractorUpdater();
    //((AttractorUpdater*)pU)->m_position = {250,-500,0};
    //((AttractorUpdater*)pU)->m_radius = 1000;
    //((AttractorUpdater*)pU)->m_atractionForce = 200;
    //m_updaters.push_back(pU);
    //pU = new RepellerUpdater();
    //((RepellerUpdater*)pU)->m_position = { 0,-250, 0 };
    //((RepellerUpdater*)pU)->m_radius = 500;
    //((RepellerUpdater*)pU)->m_repellerForce = 100;
    //m_updaters.push_back(pU);


    pU = new EulerUpdater();
    m_updaters.push_back(pU);
    //pU = new VelocityLimiter();
    //((VelocityLimiter*)pU)->m_initialSpeedLimit = 1200;
    //((VelocityLimiter*)pU)->m_finalSpeedLimit = 25;
    //m_updaters.push_back(pU);

    ParticleGenerator* pG = new BoxGenerator();
    ((BoxGenerator*)pG)->m_initialPositionRandomMin = (Vector3D(-500, 0, -500));
    ((BoxGenerator*)pG)->m_initialPositionRandomMax = (Vector3D(500, 50, 500));
    m_generator.push_back(pG);
    pG = new RandomVelocityGenerator();
    ((RandomVelocityGenerator*)pG)->m_initialVelocityRandomMin = (Vector3D(-0, 200, -0));
    ((RandomVelocityGenerator*)pG)->m_initialVelocityRandomMax = (Vector3D(0, 200, 0));
    m_generator.push_back(pG);
    pG = new RandomScaleFactorGenerator();
    ((RandomScaleFactorGenerator*)pG)->m_scaleFactorRandomMin = 1;
    ((RandomScaleFactorGenerator*)pG)->m_scaleFactorRandomMax = 1;
    m_generator.push_back(pG);


    m_attributes = _attributes;
    m_lifeTime = m_attributes.m_initialTime;
    m_timeAccum = m_attributes.m_initialTime;
#if (DR_PARTICLES_METHOD == DR_PARTICLES_GPU)
    driderSDK::File file;
    String shaderSource;

    file.Open(_T("particle_init_cs.hlsl"));
    shaderSource = StringUtils::toString(file.GetAsString(file.Size()));
    file.Close();
    m_initCS = device.createShaderFromMemory(shaderSource.data(),
      shaderSource.size(),
      DR_SHADER_TYPE_FLAG::kCompute);
    shaderSource.clear();

    file.Open(_T("particle_emit_cs.hlsl"));
    shaderSource = StringUtils::toString(file.GetAsString(file.Size()));
    file.Close();
    m_emitCS = device.createShaderFromMemory(shaderSource.data(),
      shaderSource.size(),
      DR_SHADER_TYPE_FLAG::kCompute);
    shaderSource.clear();

    file.Open(_T("particle_update_cs.hlsl"));
    shaderSource = StringUtils::toString(file.GetAsString(file.Size()));
    file.Close();
    m_updateCS = device.createShaderFromMemory(shaderSource.data(),
      shaderSource.size(),
      DR_SHADER_TYPE_FLAG::kCompute);
    shaderSource.clear();

    file.Open(_T("particle_setup_indirect_cs.hlsl"));
    shaderSource = StringUtils::toString(file.GetAsString(file.Size()));
    file.Close();
    m_setupDrawArgsCS = device.createShaderFromMemory(shaderSource.data(),
      shaderSource.size(),
      DR_SHADER_TYPE_FLAG::kCompute);
    shaderSource.clear();
    
    DrBufferDesc desc;
    desc.usage = DR_BUFFER_USAGE::kDefault;
    desc.type = DR_BUFFER_TYPE::kRWSTRUCTURE;
    desc.sizeInBytes = sizeof(UInt32) * m_attributes.m_maxParticles;
    desc.stride = sizeof(UInt32);
    m_aliveBuffer = (StructureBuffer*)device.createBuffer(desc);

    desc.type = DR_BUFFER_TYPE::kRWSTRUCTURE;
    desc.sizeInBytes = sizeof(UInt32) *  m_attributes.m_maxParticles;
    desc.stride = sizeof(UInt32);
    m_deadBuffer = (StructureBuffer*)device.createBuffer(desc);

    desc.type = DR_BUFFER_TYPE::kRWSTRUCTURE;
    desc.sizeInBytes = sizeof(GPUParticle) *  m_attributes.m_maxParticles;
    desc.stride = sizeof(GPUParticle);
    m_poolBuffer = (StructureBuffer*)device.createBuffer(desc);


    m_cpuCbuff.m_particlesToEmit = m_attributes.m_numParticlesToEmit;
    m_cpuCbuff.dt = 1/60.0f;
    m_cpuCbuff.m_finalColor = { 0,0,1,1 };
    m_cpuCbuff.m_globalAcceleration = { 0,0,0,0 };
    m_cpuCbuff.m_initialColor = { 1,0,0,1 };
    m_cpuCbuff.m_particleMaxLife = m_attributes.m_particleMaxLife;
    m_cpuCbuff.m_initialScale = 5;
    m_cpuCbuff.m_finaleScale = 0;
    m_cpuCbuff.m_maxParticles = m_attributes.m_maxParticles;
    desc.type = DR_BUFFER_TYPE::kCONSTANT;
    desc.sizeInBytes = sizeof(GPUParticleSystemCBuff);
    desc.stride = sizeof(GPUParticleSystemCBuff);
    m_cbuffer = (ConstantBuffer*)device.createBuffer(desc, (byte*)&m_cpuCbuff);

    desc.type = DR_BUFFER_TYPE::kCONSTANT;
    desc.sizeInBytes = sizeof(UInt32)*4;
    desc.stride = sizeof(UInt32);
    m_cbufferAliveCount = (ConstantBuffer*)device.createBuffer(desc);

    desc.type = DR_BUFFER_TYPE::kCONSTANT;
    desc.sizeInBytes = sizeof(UInt32) * 4;
    desc.stride = sizeof(UInt32);
    m_cbufferDeadCount = (ConstantBuffer*)device.createBuffer(desc);

    float dt[4] = {0,0,0,0};
    desc.type = DR_BUFFER_TYPE::kCONSTANT;
    desc.sizeInBytes = sizeof(float)*4;
    desc.stride = sizeof(float);
    m_cbufferDT = (ConstantBuffer*)device.createBuffer(desc,(byte*)&dt[0]);
    

    m_initCS->set(GraphicsAPI::getDeviceContext());
    m_poolBuffer->set(GraphicsAPI::getDeviceContext(), DR_SHADER_TYPE_FLAG::kCompute, 0);
    m_deadBuffer->set(GraphicsAPI::getDeviceContext(), DR_SHADER_TYPE_FLAG::kCompute, 1);
    m_cbuffer->set(GraphicsAPI::getDeviceContext(), DR_SHADER_TYPE_FLAG::kCompute, 0);

    const Int32 numThreadsPerBlock = 1024;
    GraphicsAPI::getDeviceContext().dispatch(
      Math::alignValue(m_attributes.m_maxParticles, numThreadsPerBlock) / numThreadsPerBlock, 1, 1);
    GraphicsAPI::getDeviceContext().setUAVsNull();
    GraphicsAPI::getDeviceContext().copyAtomicCounter(*m_aliveBuffer, *m_cbufferAliveCount);
    GraphicsAPI::getDeviceContext().copyAtomicCounter(*m_deadBuffer, *m_cbufferDeadCount);

    {
      DrBufferDesc desc;
      desc.usage = DR_BUFFER_USAGE::kDefault;
      desc.type = DR_BUFFER_TYPE::kINDIRECT_DRAW_INSTANCED_INDEXED;
      m_drawIndirectBuffer = (IndirectArgsBuffer*)device.createBuffer(desc);
    }

#endif
#if (DR_PARTICLES_METHOD == DR_PARTICLES_CPU)
    //m_buffer = new CBuffer[MAX_PARTICLES]; //Mem leak xdxdxd

    m_particles.m_acceleration = new Vector3D[m_attributes.m_maxParticles];//Mem leak xdxdxd
    m_particles.m_color = new Vector3D[m_attributes.m_maxParticles];//Mem leak xdxdxd
    m_particles.m_isActive = new bool[m_attributes.m_maxParticles];//Mem leak xdxdxd
    m_particles.m_lifeTime = new float[m_attributes.m_maxParticles];//Mem leak xdxdxd
    m_particles.m_position = new Vector3D[m_attributes.m_maxParticles];//Mem leak xdxdxd
    m_particles.m_rotation = new Vector3D[m_attributes.m_maxParticles];//Mem leak xdxdxd
    m_particles.m_scale = new float[m_attributes.m_maxParticles];//Mem leak xdxdxd
    m_particles.m_scaleFactor = new float[m_attributes.m_maxParticles];//Mem leak xdxdxd
    m_particles.m_velocity = new Vector3D[m_attributes.m_maxParticles];//Mem leak xdxdxd
    m_particles.m_speedLimit = new float[m_attributes.m_maxParticles];//Mem leak xdxdxd

    m_cpuRenderBuffer = new RenderStructureBuffer[m_attributes.m_maxParticles]; // Mem leak xdxdxd
    {
      DrBufferDesc desc;
      desc.usage = DR_BUFFER_USAGE::kDefault;
      desc.type = DR_BUFFER_TYPE::kSTRUCTURE;
      desc.sizeInBytes = sizeof(RenderStructureBuffer) * m_attributes.m_maxParticles;
      desc.stride = sizeof(RenderStructureBuffer);
      m_renderBuffer = (StructureBuffer*)device.createBuffer(desc);
    }
#endif
    emit();
  }
  void
  ParticleEmitter::update()
  {
    if (!m_attributes.m_isActive) {
      return;
    }
#if (DR_PARTICLES_METHOD == DR_PARTICLES_CPU)
    if (m_lifeTime >= m_attributes.m_systemMaxLife) {
      m_attributes.m_isActive = false;
      for (size_t i = 0; i < m_attributes.m_maxParticles; ++i) {
        m_particles.m_isActive[i] = false;
        m_aliveParticles = 0;
      }
      return;
    }
#endif
    float tm = Time::getDelta();
    m_lifeTime += tm;
    m_timeAccum += tm;
    m_localTransform.identity();
    m_localTransform.Translation(m_position);
    m_localTransform.Rotation(m_rotation.x, m_rotation.y, m_rotation.z);
#if (DR_PARTICLES_METHOD == DR_PARTICLES_CPU)
    for (size_t i = 0; i < m_aliveParticles; ++i) {
      if (m_particles.m_lifeTime[i] > m_attributes.m_particleMaxLife)
      {
        m_particles.m_isActive[i] = false;
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
      it->update(tm, 0, m_aliveParticles, &m_particles, m_attributes);
    }
#endif
#if (DR_PARTICLES_METHOD == DR_PARTICLES_GPU)
    float dt[4] = { tm,0,0,0 };
    m_cbufferDT->updateFromBuffer(GraphicsAPI::getDeviceContext(),(byte*)&dt[0]);

    m_updateCS->set(GraphicsAPI::getDeviceContext());
    m_poolBuffer->set(GraphicsAPI::getDeviceContext(), DR_SHADER_TYPE_FLAG::kCompute, 0);
    m_deadBuffer->set(GraphicsAPI::getDeviceContext(), DR_SHADER_TYPE_FLAG::kCompute, 1);
    m_aliveBuffer->set(GraphicsAPI::getDeviceContext(), DR_SHADER_TYPE_FLAG::kCompute, 2,0);

    m_cbuffer->set(GraphicsAPI::getDeviceContext(), DR_SHADER_TYPE_FLAG::kCompute,0);
    m_cbufferAliveCount->set(GraphicsAPI::getDeviceContext(), DR_SHADER_TYPE_FLAG::kCompute, 1);
    m_cbufferDT->set(GraphicsAPI::getDeviceContext(), DR_SHADER_TYPE_FLAG::kCompute, 2);

    const Int32 numThreadsPerBlock = 256;
    GraphicsAPI::getDeviceContext().dispatch(
      Math::alignValue(m_attributes.m_maxParticles, numThreadsPerBlock) / numThreadsPerBlock, 1, 1);
    GraphicsAPI::getDeviceContext().setUAVsNull();
    GraphicsAPI::getDeviceContext().setResourcesNull();
    GraphicsAPI::getDeviceContext().copyAtomicCounter(*m_deadBuffer, *m_cbufferDeadCount);
    GraphicsAPI::getDeviceContext().copyAtomicCounter(*m_aliveBuffer, *m_cbufferAliveCount);
#endif
    emit();
#if (DR_PARTICLES_METHOD == DR_PARTICLES_CPU)
    for (size_t i = 0; i < m_aliveParticles; ++i) {
      m_cpuRenderBuffer[i].position = m_particles.m_position[i];
      m_cpuRenderBuffer[i].color = m_particles.m_color[i];
      m_cpuRenderBuffer[i].scale = m_particles.m_scale[i];
    }
    static_cast<StructureBuffer*>(m_renderBuffer)->updateFromBuffer(GraphicsAPI::getDeviceContext(), (byte*)m_cpuRenderBuffer);
#endif
  }
  void
    ParticleEmitter::emit()
  {
    if (m_timeAccum > m_attributes.m_rate) {
      m_timeAccum -= m_attributes.m_rate;
#if (DR_PARTICLES_METHOD == DR_PARTICLES_CPU)
      size_t endID = std::min(m_aliveParticles + m_attributes.m_numParticlesToEmit, m_attributes.m_maxParticles);
      size_t tmp = m_aliveParticles;
      for (size_t i = m_aliveParticles; i < endID; ++i) {
        m_particles.m_isActive[i] = true;
        m_particles.m_lifeTime[i] = 0.0f;
        m_particles.m_speedLimit[i] = Math::MAX_FLOAT;
        m_aliveParticles++;
      }
      for (auto &it : m_generator) {
        it->generate(tmp, endID, &m_particles);
      }
      for (auto &it : m_updaters) {
        it->update(0,tmp, endID, &m_particles,m_attributes);
      }
#endif
#if (DR_PARTICLES_METHOD == DR_PARTICLES_GPU)
      m_emitCS->set(GraphicsAPI::getDeviceContext());
      m_poolBuffer->set(GraphicsAPI::getDeviceContext(), DR_SHADER_TYPE_FLAG::kCompute, 0);
      m_deadBuffer->set(GraphicsAPI::getDeviceContext(), DR_SHADER_TYPE_FLAG::kCompute, 1);
      m_aliveBuffer->set(GraphicsAPI::getDeviceContext(), DR_SHADER_TYPE_FLAG::kCompute, 2);

      m_cbuffer->set(GraphicsAPI::getDeviceContext(), DR_SHADER_TYPE_FLAG::kCompute,0);
      m_cbufferDeadCount->set(GraphicsAPI::getDeviceContext(), DR_SHADER_TYPE_FLAG::kCompute,1);
      m_cbufferDT->set(GraphicsAPI::getDeviceContext(), DR_SHADER_TYPE_FLAG::kCompute, 2);

      const Int32 numThreadsPerBlock = 1024;
      GraphicsAPI::getDeviceContext().dispatch(
        Math::alignValue(m_attributes.m_numParticlesToEmit, numThreadsPerBlock) / numThreadsPerBlock, 1, 1);
      GraphicsAPI::getDeviceContext().setUAVsNull();


      GraphicsAPI::getDeviceContext().copyAtomicCounter(*m_deadBuffer, *m_cbufferDeadCount);
      GraphicsAPI::getDeviceContext().copyAtomicCounter(*m_aliveBuffer, *m_cbufferAliveCount);


      m_setupDrawArgsCS->set(GraphicsAPI::getDeviceContext());
      m_drawIndirectBuffer->set(GraphicsAPI::getDeviceContext(), DR_SHADER_TYPE_FLAG::kCompute, 0);
      m_cbufferAliveCount->set(GraphicsAPI::getDeviceContext(), DR_SHADER_TYPE_FLAG::kCompute, 0);

      GraphicsAPI::getDeviceContext().dispatch(1, 1, 1);
      GraphicsAPI::getDeviceContext().setUAVsNull();
#endif
    }
  }
  void BoxGenerator::generate(size_t start, size_t end, Particle * p)
  {
    //[[unroll]]
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
  void TimeColorUpdater::update(float dt, size_t start, size_t end, Particle * p, const ParticleEmitterAttributes & attr)
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
  void TimeScaleUpdater::update(float dt, size_t start, size_t end, Particle * p, const ParticleEmitterAttributes & attr)
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
  void EulerUpdater::update(float dt, size_t start, size_t end, Particle * p, const ParticleEmitterAttributes & attr)
  {
    //std::cout << start << "   +++    " << end << std::endl;
    //Int32 mod = end % 16;
    //end = (Int32)end / 16;
    [[unroll]]
    for (size_t i = start; i < end; ++i) {

      //for (size_t i = start; i < end; ++i) {
      p->m_velocity[i] += (p->m_acceleration[i] + m_globalAcceleration) * dt;
      if (p->m_velocity[i].length() > p->m_speedLimit[i]) {
        p->m_velocity[i] = p->m_velocity[i].normalize() * p->m_speedLimit[i];
      }
      p->m_position[i] += p->m_velocity[i] * dt;
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

      //p->m_position[i] += p->m_velocity[i] * tm;
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
  void VelocityLimiter::update(float dt, size_t start, size_t end, Particle * p, const ParticleEmitterAttributes & attr)
  {
    float _proportionMul = 1.0f / attr.m_particleMaxLife;
    [[unroll]]
    for (size_t i = start; i < end; ++i) {
      float _proportion = p->m_lifeTime[i] * _proportionMul;
      float m1Proportion = (1.0f - _proportion);
      p->m_speedLimit[i] = (m_initialSpeedLimit * m1Proportion) + (m_finalSpeedLimit * _proportion);
    }
  }
  void AttractorUpdater::update(float dt, size_t start, size_t end, Particle * p, const ParticleEmitterAttributes & attr)
  {
    for (size_t i = start; i < end; ++i) {
      Vector3D v = m_position - p->m_position[i];
      float l = v.length();
      if (l <= m_radius) {
        p->m_acceleration[i] += v.normalize() * (m_atractionForce * (m_radius/l));
      }
    }
  }
  void RepellerUpdater::update(float dt, size_t start, size_t end, Particle * p, const ParticleEmitterAttributes & attr)
  {
    for (size_t i = start; i < end; ++i) {
      Vector3D v = p->m_position[i] - m_position;
      float l = v.length();
      if (l <= m_radius) {
        p->m_acceleration[i] += v.normalize() * (m_repellerForce * (m_radius / l));
      }
    }
  }
  void SphereGenerator::generate(size_t start, size_t end, Particle * p)
  {
  }
}