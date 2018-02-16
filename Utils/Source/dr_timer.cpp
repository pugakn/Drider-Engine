#include "dr_timer.h"
#include "dr_clock.h"

namespace driderSDK {

Timer::Timer(const Time& _duration, bool _canPause, bool _useAbs) 
  : m_canPause(_canPause),
    m_useAbs(_useAbs), 
    m_duration(_duration)
{}

Timer::Timer(bool _canPause, bool _useAbs) 
  : Timer(Time(Microseconds(0)), _canPause, _useAbs)
{}

void 
Timer::reset(const Time& duration) {
  m_duration = duration;
}

bool 
Timer::done() {

  Time now;

  if (m_useAbs) {
    now = Clock::nowAbs(m_canPause);
  }
  else {
    now = Clock::now(m_canPause);
  }

  Time dif = now - m_duration;

  return dif >= Time(Microseconds(0));
}
}