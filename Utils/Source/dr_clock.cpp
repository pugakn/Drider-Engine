#include "dr_clock.h"

namespace driderSDK {

Time
Clock::now(bool pausedTime) {

  Time global = instance().m_globalTime;

  if (pausedTime && instance().isPaused()) {
    global -= global - instance().m_pauseStart;
  }

  return pausedTime ? global - instance().m_pausedTime : 
                      global;
}

Time 
Clock::nowAbs(bool pausedTime) {
  
  Time global = instance()._now();

  if (pausedTime && instance().isPaused()) {
    global -= global - instance().m_pauseStart;
  }

  return pausedTime ? global - instance().m_pausedTime : 
                      global;
}

const Time& 
Clock::delta() {
  return instance().m_elapsedTime;
}

void
Clock::onStartUp() {
  using namespace std::chrono;
  m_start = ChronoClock::now();
  m_elapsedTime.fromMicroseconds(0);
  m_pausedTime = m_pauseStart = m_lastUpdate = m_globalTime = m_elapsedTime;
}

void 
Clock::pause() {
  if (!isPaused()) {
    m_pauseStart = _now();
    m_elapsedTime.fromMicroseconds(0);
  }  
}

void 
Clock::resume() {
  if (isPaused()) {
    auto now = m_lastUpdate = _now();
    m_pausedTime += now - m_pauseStart;
  }
}

void
Clock::update() {
  using namespace std::chrono;

  auto now = _now();
    
  m_globalTime = now;

  if (!isPaused()) {
    
    m_elapsedTime = now - m_lastUpdate;
        
    m_lastUpdate = now;
  }
}

bool 
Clock::isPaused() {
  return (m_pauseStart > m_lastUpdate);
}

Time 
Clock::_now() {
  using namespace std::chrono;
  auto t = TimePoint(ChronoClock::now() - m_start);
  auto asMicro = duration_cast<Time::Duration>(t.time_since_epoch());
  return Time(Microseconds(asMicro.count()));
}

}