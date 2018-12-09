#include "dr_particle_emitter.h"
#include <dr_time.h>
#include <dr_random.h>
//#include <dr_camera_manager.h>
//#include <dr_camera.h>
#include <dr_device.h>
#include <dr_device_context.h>
#include <dr_graphics_api.h>
#include <dr_file.h>
#include <dr_string_utils.h>
namespace driderSDK {
void
ParticleEmitter::init(const ParticleEmitterAttributes& _attributes) {
  Device& device = GraphicsAPI::getDevice();
  m_updaters.resize(UPDATERS::kUPDATER_COUNT);
  m_generator.resize(GENERATORS::kGENERATOR_COUNT);
  //Updaters
  ParticleUpdater* pU;
  pU = new TimeColorUpdater();
  m_updaters[UPDATERS::kTIME_COLOR]  = (pU);
  pU = new TimeScaleUpdater();
  m_updaters[UPDATERS::kTIME_SCALE] = (pU);
  pU = new AttractorUpdater();
  m_updaters[UPDATERS::kATTRACTORS] = (pU);
  pU = new EulerUpdater();
  m_updaters[UPDATERS::kEULER] = (pU);
  pU = new VortexUpdater();
  m_updaters[UPDATERS::kVORTEX] = (pU);
  pU = new PlaneColliderUpdater();
  m_updaters[UPDATERS::kPLANE_COLLISION] = (pU);

  //Generators
  ParticleGenerator* pG = new BoxGenerator();
  m_generator[GENERATORS::kBOX] = (pG);
  pG = new RandomVelocityGenerator();
  m_generator[GENERATORS::kRANDOM_VELOCITY] = (pG);
  //pG = new RandomScaleFactorGenerator();
  //((RandomScaleFactorGenerator*)pG)->m_scaleFactorRandomMin = 1;
  //((RandomScaleFactorGenerator*)pG)->m_scaleFactorRandomMax = 1;
  //m_generator[GENERATORS::kSca] = (pG);


  m_attributes = _attributes;
  m_lifeTime = m_attributes.m_initialTime;
  m_timeAccum = m_attributes.m_initialTime;
#if (DR_PARTICLES_METHOD == DR_PARTICLES_GPU)
  driderSDK::File file;
  String shaderSource;

  file.Open(_T("Resources\\Shaders\\particle_init_cs.hlsl"));
  shaderSource.clear();
  shaderSource += "#define MAX_ATTRACTORS " +
                  std::to_string(MAX_ATTRACTORS) + "\n";
  shaderSource += "#define MAX_VORTEX " +
                  std::to_string(MAX_VORTEX) + "\n";
  shaderSource += StringUtils::toString(file.GetAsString(file.Size()));
  file.Close();
  m_initCS = device.createShaderFromMemory(shaderSource.data(),
                                           shaderSource.size(),
                                           DR_SHADER_TYPE_FLAG::kCompute);
  shaderSource.clear();

  file.Open(_T("Resources\\Shaders\\particle_emit_cs.hlsl"));
  shaderSource += "#define DR_NUM_THREADS_PER_BLOCK " +
                  std::to_string(EMIT_NUM_THREADS_PER_BLOCK) + "\n";
  shaderSource += "#define MAX_ATTRACTORS " +
                  std::to_string(MAX_ATTRACTORS) + "\n";
  shaderSource += "#define MAX_VORTEX " +
                  std::to_string(MAX_VORTEX) + "\n";
  shaderSource += StringUtils::toString(file.GetAsString(file.Size()));
                  file.Close();
  m_emitCS = device.createShaderFromMemory(shaderSource.data(),
                                           shaderSource.size(),
                                           DR_SHADER_TYPE_FLAG::kCompute);
  shaderSource.clear();

  file.Open(_T("Resources\\Shaders\\particle_update_cs.hlsl"));
  shaderSource += "#define DR_NUM_THREADS_PER_BLOCK " +
                  std::to_string(UPDATE_NUM_THREADS_PER_BLOCK) +
                  "\n";
  shaderSource += "#define DR_NUM_PARTICLES_PER_THREAD " +
                  std::to_string(UPDATE_NUM_PARTICLES_PER_THREAD) +
                  "\n";
  shaderSource += "#define MAX_ATTRACTORS "
                  + std::to_string(MAX_ATTRACTORS)
                  + "\n";
  shaderSource += "#define MAX_VORTEX " +
                  std::to_string(MAX_VORTEX) +
                  "\n";
  shaderSource += StringUtils::toString(file.GetAsString(file.Size()));
  file.Close();
  m_updateCS = device.createShaderFromMemory(shaderSource.data(),
                                             shaderSource.size(),
                                             DR_SHADER_TYPE_FLAG::kCompute);
  shaderSource.clear();
  
  file.Open(_T("Resources\\Shaders\\particle_setup_indirect_cs.hlsl"));
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

  m_cpuCbuff.m_systemPosition = { 100,-500,0,1 };
  m_cpuCbuff.m_particlesToEmit = m_attributes.m_numParticlesToEmit;
  m_cpuCbuff.m_finalColor = { 0,0,1,1 };
  m_cpuCbuff.m_globalAcceleration = { 0,0,0,0 };
  m_cpuCbuff.m_initialColor = { 1,0,0,1 };
  m_cpuCbuff.m_particleMaxLife = m_attributes.m_particleMaxLife;
  m_cpuCbuff.m_randomPosMin = {-500,0,-500};
  m_cpuCbuff.m_randomPosMax = { 500,0,500 };
  m_cpuCbuff.m_randomVelMin = { -0,200,-0 };
  m_cpuCbuff.m_randomVelMax = { -0,200,-0 };
  m_cpuCbuff.m_initialScale = 10;
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
  GraphicsAPI::getDeviceContext().dispatch(Math::alignValue(m_attributes.m_maxParticles, numThreadsPerBlock) / numThreadsPerBlock, 1, 1);
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
ParticleEmitter::update() {
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
    if (m_particles.m_lifeTime[i] > m_attributes.m_particleMaxLife) {
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
  DeviceContext& dc = GraphicsAPI::getDeviceContext();
  m_cbufferDT->updateFromBuffer(dc,(byte*)&dt[0]);

  const bool paramsChanged = true; //TODO: 
  if (paramsChanged) {
    m_cpuCbuff.m_systemPosition = { 0,-0,0,1 };
    m_cpuCbuff.m_particleMaxLife = m_attributes.m_particleMaxLife;
    m_cpuCbuff.m_particlesToEmit = m_attributes.m_numParticlesToEmit;
    m_cpuCbuff.m_maxParticles = m_attributes.m_maxParticles;

    //Active
    m_cpuCbuff.m_bColliderUpdaterActive = static_cast<PlaneColliderUpdater*>(m_updaters[UPDATERS::kPLANE_COLLISION])->m_bActive;
    m_cpuCbuff.m_bAttractorUpdaterActive = static_cast<AttractorUpdater*>(m_updaters[UPDATERS::kATTRACTORS])->m_bActive;
    m_cpuCbuff.m_bBoxGeneratorActive = static_cast<BoxGenerator*>(m_generator[GENERATORS::kBOX])->m_bActive;
    m_cpuCbuff.m_bVortexActive = static_cast<VortexUpdater*>(m_updaters[UPDATERS::kVORTEX])->m_bActive;
    //m_cpuCbuff.m_bColliderUpdaterActive = 
    m_cpuCbuff.m_bEulerUpdaterActive = static_cast<EulerUpdater*>(m_updaters[UPDATERS::kEULER])->m_bActive;
    m_cpuCbuff.m_bRandVelocityGeneratorActive = static_cast<RandomVelocityGenerator*>(m_generator[GENERATORS::kRANDOM_VELOCITY])->m_bActive;
    m_cpuCbuff.m_bTimeColorUpdaterActive = static_cast<TimeColorUpdater*>(m_updaters[UPDATERS::kTIME_COLOR])->m_bActive;
    m_cpuCbuff.m_bTimeScaleUpdaterActive = static_cast<TimeScaleUpdater*>(m_updaters[UPDATERS::kTIME_SCALE])->m_bActive;
    //Updaters
    m_cpuCbuff.m_initialColor = static_cast<TimeColorUpdater*>(m_updaters[UPDATERS::kTIME_COLOR])->m_initialColor;
    m_cpuCbuff.m_finalColor = static_cast<TimeColorUpdater*>(m_updaters[UPDATERS::kTIME_COLOR])->m_finalColor;
    m_cpuCbuff.m_initialScale = static_cast<TimeScaleUpdater*>(m_updaters[UPDATERS::kTIME_SCALE])->m_initialScale;
    m_cpuCbuff.m_finaleScale = static_cast<TimeScaleUpdater*>(m_updaters[UPDATERS::kTIME_SCALE])->m_finaleScale;
    static_cast<EulerUpdater*>(m_updaters[UPDATERS::kEULER])->m_globalAcceleration = static_cast<EulerUpdater*>(m_updaters[UPDATERS::kEULER])->m_windForce;
    static_cast<EulerUpdater*>(m_updaters[UPDATERS::kEULER])->m_globalAcceleration.y += -9.81 * static_cast<EulerUpdater*>(m_updaters[UPDATERS::kEULER])->m_gravityScale;
    m_cpuCbuff.m_globalAcceleration = static_cast<EulerUpdater*>(m_updaters[UPDATERS::kEULER])->m_globalAcceleration;
    
    m_cpuCbuff.m_numAttractors = static_cast<AttractorUpdater*>(m_updaters[UPDATERS::kATTRACTORS])->size();
    for (SizeT i = 0; i < m_cpuCbuff.m_numAttractors; ++i) {
      m_cpuCbuff.m_attractorPos[i] = static_cast<AttractorUpdater*>(m_updaters[UPDATERS::kATTRACTORS])->get(i).m_position;
      m_cpuCbuff.m_attractorForceX_radiusY[i].x = static_cast<AttractorUpdater*>(m_updaters[UPDATERS::kATTRACTORS])->get(i).m_atractionForce;
      m_cpuCbuff.m_attractorForceX_radiusY[i].y = static_cast<AttractorUpdater*>(m_updaters[UPDATERS::kATTRACTORS])->get(i).m_radius;
    }
    m_cpuCbuff.m_numVortex = static_cast<VortexUpdater*>(m_updaters[UPDATERS::kVORTEX])->size();
    for (SizeT i = 0; i < m_cpuCbuff.m_numVortex; ++i) {
      m_cpuCbuff.m_VortexPos[i] = static_cast<VortexUpdater*>(m_updaters[UPDATERS::kVORTEX])->get(i).m_position;
      m_cpuCbuff.m_VortexForceX_radiuusY[i].x = static_cast<VortexUpdater*>(m_updaters[UPDATERS::kVORTEX])->get(i).m_force;
      m_cpuCbuff.m_VortexForceX_radiuusY[i].y = static_cast<VortexUpdater*>(m_updaters[UPDATERS::kVORTEX])->get(i).m_radius;
      m_cpuCbuff.m_VortexUP[i] = static_cast<VortexUpdater*>(m_updaters[UPDATERS::kVORTEX])->get(i).m_up;
    }
    m_cpuCbuff.m_planeNormal_k = static_cast<PlaneColliderUpdater*>(m_updaters[UPDATERS::kPLANE_COLLISION])->m_normal;
    m_cpuCbuff.m_planeNormal_k.w = static_cast<PlaneColliderUpdater*>(m_updaters[UPDATERS::kPLANE_COLLISION])->m_k;
    m_cpuCbuff.m_planePoint = static_cast<PlaneColliderUpdater*>(m_updaters[UPDATERS::kPLANE_COLLISION])->m_point;


    //Generators
    m_cpuCbuff.m_randomPosMin = static_cast<BoxGenerator*>(m_generator[GENERATORS::kBOX])->m_initialPositionRandomMin;
    m_cpuCbuff.m_randomPosMax = static_cast<BoxGenerator*>(m_generator[GENERATORS::kBOX])->m_initialPositionRandomMax;
    m_cpuCbuff.m_randomVelMin = static_cast<RandomVelocityGenerator*>(m_generator[GENERATORS::kRANDOM_VELOCITY])->m_initialVelocityRandomMin;
    m_cpuCbuff.m_randomVelMax = static_cast<RandomVelocityGenerator*>(m_generator[GENERATORS::kRANDOM_VELOCITY])->m_initialVelocityRandomMax;
    m_cbuffer->updateFromBuffer(dc, (byte*)&m_cpuCbuff);
  }

  m_updateCS->set(dc);
  m_poolBuffer->set(dc, DR_SHADER_TYPE_FLAG::kCompute, 0);
  m_deadBuffer->set(dc, DR_SHADER_TYPE_FLAG::kCompute, 1);
  m_aliveBuffer->set(dc, DR_SHADER_TYPE_FLAG::kCompute, 2,0);

  m_cbuffer->set(dc, DR_SHADER_TYPE_FLAG::kCompute,0);
  m_cbufferAliveCount->set(dc, DR_SHADER_TYPE_FLAG::kCompute, 1);
  m_cbufferDT->set(dc, DR_SHADER_TYPE_FLAG::kCompute, 2);

  dc.dispatch(
    Math::alignValue(m_attributes.m_maxParticles / static_cast<float>(UPDATE_NUM_PARTICLES_PER_THREAD), 
                     UPDATE_NUM_THREADS_PER_BLOCK) / UPDATE_NUM_THREADS_PER_BLOCK, 1, 1);
  dc.setUAVsNull();
  dc.setResourcesNull();
  dc.copyAtomicCounter(*m_deadBuffer, *m_cbufferDeadCount);
  dc.copyAtomicCounter(*m_aliveBuffer, *m_cbufferAliveCount);
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
ParticleEmitter::emit() {
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
    DeviceContext& dc = GraphicsAPI::getDeviceContext();
    m_emitCS->set(dc);
    m_poolBuffer->set(dc, DR_SHADER_TYPE_FLAG::kCompute, 0);
    m_deadBuffer->set(dc, DR_SHADER_TYPE_FLAG::kCompute, 1);
    m_aliveBuffer->set(dc, DR_SHADER_TYPE_FLAG::kCompute, 2);

    m_cbuffer->set(dc, DR_SHADER_TYPE_FLAG::kCompute,0);
    m_cbufferDeadCount->set(dc, DR_SHADER_TYPE_FLAG::kCompute,1);
    m_cbufferDT->set(dc, DR_SHADER_TYPE_FLAG::kCompute, 2);

    dc.dispatch(
      Math::alignValue(m_attributes.m_numParticlesToEmit, EMIT_NUM_THREADS_PER_BLOCK) / EMIT_NUM_THREADS_PER_BLOCK, 1, 1);
    dc.setUAVsNull();


    dc.copyAtomicCounter(*m_deadBuffer, *m_cbufferDeadCount);
    dc.copyAtomicCounter(*m_aliveBuffer, *m_cbufferAliveCount);


    m_setupDrawArgsCS->set(dc);
    m_drawIndirectBuffer->set(dc, DR_SHADER_TYPE_FLAG::kCompute, 0);
    m_cbufferAliveCount->set(dc, DR_SHADER_TYPE_FLAG::kCompute, 0);

    dc.dispatch(1, 1, 1);
    dc.setUAVsNull();
#endif
  }
}

void
BoxGenerator::generate(size_t start, size_t end, Particle* p) {
  //[[unroll]]
  for (size_t i = start; i < end; ++i) {
    //p->m_position[i] = Random::RandomRange(m_initialPositionRandomMin, m_initialPositionRandomMax);
    float lerp = Random::get(0.0f, 1.0f);
    p->m_position[i] = Math::lerp(m_initialPositionRandomMin, m_initialPositionRandomMax, lerp);
  }
}

void
RandomVelocityGenerator::generate(size_t start, size_t end, Particle* p) {
  for (size_t i = start; i < end; ++i) {
    //p->m_velocity[i] = Random::RandomRange(m_initialVelocityRandomMin, m_initialVelocityRandomMax);
    float lerp = Random::get(0.0f, 1.0f);
    Math::lerp(m_initialVelocityRandomMin, m_initialVelocityRandomMax, lerp);
  }
}

void
TimeColorUpdater::update(float dt,
                         size_t start,
                         size_t end,
                         Particle* p,
                         const ParticleEmitterAttributes& attr) {
  float _proportionMul = 1.0f / attr.m_particleMaxLife;
  //end /= 16;
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

void
TimeScaleUpdater::update(float dt,
                         size_t start,
                         size_t end,
                         Particle* p,
                         const ParticleEmitterAttributes& attr) {
    float _proportionMul = 1.0f / attr.m_particleMaxLife;
    //end /= 16;
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

void
EulerUpdater::update(float dt,
                     size_t start,
                     size_t end,
                     Particle* p,
                     const ParticleEmitterAttributes& attr) {
  //std::cout << start << "   +++    " << end << std::endl;
  //Int32 mod = end % 16;
  //end = (Int32)end / 16;
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

void
RandomScaleFactorGenerator::generate(size_t start, size_t end, Particle* p) {
  for (size_t i = start; i < end; ++i) {
    //p->m_scaleFactor[i] = Random::RandomRange(m_scaleFactorRandomMin, m_scaleFactorRandomMax);
    p->m_scaleFactor[i] = Random::get(m_scaleFactorRandomMin, m_scaleFactorRandomMax);
  }
}

void
VelocityLimiter::update(float dt,
                        size_t start,
                        size_t end,
                        Particle* p,
                        const ParticleEmitterAttributes& attr) {
  float _proportionMul = 1.0f / attr.m_particleMaxLife;

  for (size_t i = start; i < end; ++i) {
    float _proportion = p->m_lifeTime[i] * _proportionMul;
    float m1Proportion = (1.0f - _proportion);
    p->m_speedLimit[i] = (m_initialSpeedLimit * m1Proportion) + (m_finalSpeedLimit * _proportion);
  }
}

void
AttractorUpdater::add(const Vector3D& _pos, float _force, float _radius) {
  if (size() >= MAX_ATTRACTORS) {
    return;
  }
  Attractor atr;
  atr.m_position = _pos;
  atr.m_atractionForce = _force;
  atr.m_radius = _radius;
  m_attractors.push_back(atr);
}

void
AttractorUpdater::remove(Int32 _id) {
  m_attractors.erase(m_attractors.begin() + _id);
}

Attractor&
AttractorUpdater::get(Int32 _id) {
  return m_attractors[_id];
}

Int32
AttractorUpdater::size() {
  return m_attractors.size();
}

void
AttractorUpdater::update(float dt,
                         size_t start,
                         size_t end,
                         Particle* p,
                         const ParticleEmitterAttributes& attr) {
  for (auto &it : m_attractors) {
    for (size_t i = start; i < end; ++i) {
      Vector3D v = it.m_position - p->m_position[i];
      float l = v.length();
      if (l <= it.m_radius) {
        p->m_acceleration[i] += v.normalize() * (it.m_atractionForce * (it.m_radius / l));
      }
    }
  }
}

void
RepellerUpdater::add(const Vector3D& _pos, float _force, float _radius) {
  if (size() >= MAX_REPELLERS)
    return;
  Repeller rep;
  rep.m_position = _pos;
  rep.m_repellerForce = _force;
  rep.m_radius = _radius;
  m_repellers.push_back(rep);
}

void
RepellerUpdater::remove(Int32 _id) {
  m_repellers.erase(m_repellers.begin() + _id);
}

Repeller&
RepellerUpdater::get(Int32 _id) {
  return m_repellers[_id];
}

Int32
RepellerUpdater::size() {
  return m_repellers.size();
}

void
RepellerUpdater::update(float dt,
                        size_t start,
                        size_t end,
                        Particle* p,
                        const ParticleEmitterAttributes& attr) {
  for (auto &it : m_repellers) {
    for (size_t i = start; i < end; ++i) {
      Vector3D v = p->m_position[i] - it.m_position;
      float l = v.length();
      if (l <= it.m_radius) {
        p->m_acceleration[i] += v.normalize() * (it.m_repellerForce * (it.m_radius / l));
      }
    }
  }
}

void
SphereGenerator::generate(size_t start, size_t end, Particle* p) {
}

void
PlaneColliderUpdater::update(float dt,
                             size_t start,
                             size_t end,
                             Particle* p,
                             const ParticleEmitterAttributes& attr) {
}

void
VortexUpdater::add(const Vector3D& _pos,
                   const Vector3D& _up,
                   float _force,
                   float _radius) {
  if (size() >= MAX_VORTEX)
    return;
  Vortex vortex;
  vortex.m_position = _pos;
  vortex.m_up = _up;
  vortex.m_force = _force;
  vortex.m_radius = _radius;
  m_vortex.push_back(vortex);
}
void
VortexUpdater::remove(Int32 _id) {
  m_vortex.erase(m_vortex.begin() + _id);
}

Vortex&
VortexUpdater::get(Int32 _id) {
  return m_vortex[_id];
}

Int32
VortexUpdater::size() {
  return m_vortex.size();
}

void
VortexUpdater::update(float dt,
                      size_t start,
                      size_t end,
                      Particle* p,
                      const ParticleEmitterAttributes& attr) {
}

}