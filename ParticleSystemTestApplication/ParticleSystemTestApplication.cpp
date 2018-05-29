// ParticleSystemTestApplication.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"

#include <dr_particle_emitter.h>
#include <dr_vector3d.h>
#include <dr_time.h>

int
main() {
  driderSDK::Time::startUp();
  driderSDK::ParticleEmitter emitter;
  driderSDK::ParticleEmitterAttributes attr;
  attr.m_maxParticles = 10;
  attr.m_initialTime = 2;
  attr.m_rithm = 2;
  attr.m_particleMaxLife = 1;
  attr.m_numParticlesToEmit = 1;
  attr.m_initialVelocity = driderSDK::Vector3D(0,1,0);
  attr.m_position = driderSDK::Vector3D(0, 0, 0);
  emitter.init(attr);

  while (true) {
    driderSDK::Time::update();
    emitter.update();
  }
    return 0;
}