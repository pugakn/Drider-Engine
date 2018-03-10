#pragma once
#include <chrono>
#include "dr_util_prerequisites.h"

namespace driderSDK {

class DR_UTIL_EXPORT Timer 
{
 public:

  /**
  * Inits the timer to 0.
  * 
  * @param milliseconds
  *   The starting time of the timer.
  */
  void 
  init(float milliseconds = 0.0f);

  float 
  getSeconds() const;

  float 
  getMilliseconds() const;

  float
  getMicroseconds() const;

 private:
  using Clock = std::chrono::high_resolution_clock;
  using TimePoint = std::chrono::time_point<Clock>;
  using Duration = std::chrono::nanoseconds;
  using Seconds = std::chrono::duration<float>;
  using Milliseconds = std::chrono::duration<float, std::milli>;
  using Microseconds = std::chrono::duration<float, std::micro>;

  Duration
  getElapsed() const;
 private:
  TimePoint m_start;
  Int64 m_startingTime;
};

}