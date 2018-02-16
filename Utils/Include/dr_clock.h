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
  /**
  * Gets the time since the start of the application. 
  * It will be the same until the clock is 
  * updated (next frame), i.e. on Frame 0 all calls 
  * to now() will return the same vaule until next Frame.
  *
  * @param pausedTime
  *  Specifies if the time should take in count the 
  *  paused time.
  *
  * @return 
  *   The time of the application.
  */
  static 
  Time now(bool pausedTime = false);

  /**
  * Gets the time since the start of the application.
  * It will be different between each call, i.e if 
  * it the function gets called in two consecutive lines
  * the result may vary.
  * 
  * @param pausedTime
  *  Specifies if the time should take in count the 
  *  paused time.
  *
  * @return 
  *   The time of the app.
  * 
  */
  static
  Time nowAbs(bool pausedTime = false);
  
  /**
  * Gets the duration of the frame.
  * 
  * @return
  *   The time of the frame duration. 0 if the clock is paused
  */
  static const 
  Time& delta();
 //private:
  /**
  * Initilizes the clock
  */
  virtual void 
  onStartUp();

  /**
  * Pauses the clock
  */
  void
  pause();

  /**
  * Resumes the clock
  */
  void
  resume();

  /**
  * Updates the time for the clock, it must be called every frame
  */
  void 
  update(); 

  /**
  * Gets the state of the clock
  * 
  * @return 
  *   True if the clock is paused, false otherwise.
  */
  bool 
  isPaused();

  /**
  * Gets the elapsed time from the start of the app
  * to the call moment, without taking in count pauses
  * and updates (it will be diferent on every call).
  * 
  * @return 
  *   The time elapsed
  */
  Time
  _now();
 private:
  TimePoint m_start;
  Time m_lastUpdate;
  Time m_pauseStart;
  Time m_pausedTime;
  Time m_elapsedTime;
  Time m_globalTime;
};

}
