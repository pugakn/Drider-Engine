#include "dr_time.h"

namespace driderSDK {

float
Time::getElapsed() {
  return instance().m_timer.getSeconds();
}

float
Time::getElapsedMilli() {
  return instance().m_timer.getMilliseconds();
}

float
Time::getElapsedMicro() {
  return instance().m_timer.getMicroseconds();
}

float 
Time::getDelta() {
  return instance().m_elapsed;
}

float 
Time::getDeltaMilli() {
  return instance().m_elapsed * 1000;
}

float 
Time::getDeltaMicro() {
  return instance().m_elapsed * 1000000;
}

void 
Time::onStartUp() {
  m_timer.init();
  m_lastUpdate = 0;
  m_elapsed = 0;
}


void
Time::update() {
  float now = m_timer.getSeconds();

  m_elapsed = now - m_lastUpdate;

  m_lastUpdate = now;
}

}