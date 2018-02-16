#include "dr_timer.h"

namespace driderSDK {

void 
Timer::init(float seconds) {
  m_startingTime = static_cast<Int64>(seconds * 1000000);
  m_start = Clock::now();
}

float 
Timer::getSeconds() const {
  return std::chrono::duration_cast<Seconds>(getElapsed()).count();
}

float 
Timer::getMilliseconds() const {
  return std::chrono::duration_cast<Milliseconds>(getElapsed()).count();
}

float 
Timer::getMicroseconds() const {
  return std::chrono::duration_cast<Microseconds>(getElapsed()).count();
}

Timer::Duration 
Timer::getElapsed() const {
  return Duration(Clock::now() - m_start) + 
         std::chrono::microseconds(m_startingTime);
}

}