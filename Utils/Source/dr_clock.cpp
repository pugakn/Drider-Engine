#include "dr_clock.h"

namespace driderSDK {

const Time& 
Clock::getGlobalTime() {
  return instance().m_globalTime;
}

const Time& 
Clock::getTickDuration() {
  return instance().m_elapsedTime;
}

void 
Clock::pause() {
  m_pauseStart = ChronoClock::now();
}

void 
Clock::resume() {
  auto now = ChronoClock::now();
  auto dur = std::chrono::duration_cast<Time::Duration>(now - m_pauseStart);
  m_pausedTime += Time(Time::Microseconds(dur.count()));
  m_pauseStart = m_start;
}

void
Clock::update() {
  using namespace std::chrono;
  
  if(m_pauseStart > m_lastUpdate) {
    return;
  }

  auto now = ChronoClock::now();

  auto duration = duration_cast<Time::Duration>(now - m_start);

  m_globalTime.fromMicroseconds(duration.count());

  m_globalTime -= m_pausedTime;

  duration = duration_cast<Time::Duration>(now - m_lastUpdate);

  m_elapsedTime.fromMicroseconds(duration.count());
  
  m_lastUpdate = now;
}

void 
Clock::onStartUp() {
  using namespace std::chrono;
  m_lastUpdate = m_start = ChronoClock::now();
  m_pausedTime.fromMicroseconds(0);
  m_globalTime.fromMicroseconds(0);
  m_elapsedTime.fromMicroseconds(0);
}

}