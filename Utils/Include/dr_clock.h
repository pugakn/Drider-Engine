#pragma once

#include <chrono>
#include <../../Core/Include/dr_module.h>
#include "dr_time.h"
#include "dr_util_prerequisites.h"

namespace driderSDK {

class DR_UTIL_EXPORT Clock : public Module<Clock>
{
  friend class Application;
  using ChronoClock = std::chrono::high_resolution_clock;
  using TimePoint = ChronoClock::time_point;
 public: 
  static const 
  Time& getGlobalTime();
  
  static const 
  Time& getTickDuration();
 private:
  void
  pause();

  void
  resume();

  void 
  update(); 
  virtual void 
  onStartUp();
 private:
  TimePoint m_lastUpdate;
  TimePoint m_start;
  TimePoint m_pauseStart;
  Time m_pausedTime;
  Time m_elapsedTime;
  Time m_globalTime;
};

}
