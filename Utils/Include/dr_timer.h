#pragma once

#include "dr_time.h"
#include "dr_util_prerequisites.h"
/*
posible use of callbacks
#include <functional>
*/

namespace driderSDK {

class DR_UTIL_EXPORT Timer 
{
 public:
  /**
  * Constructor using a duration
  * 
  * @param duration
  *  The duration of the timer.
  * 
  * @param canPause
  *  Tells if timer can be affected by the paused time
  *  of the app.
  * 
  * @param useAbs
  *  Tells if it should use the absolute time of the aplication
  *  (Clock::nowAbs) or the regular (Clock::now)
  */
  Timer(const Time& _duration, bool _canPause = false, bool _useAbs = false);

  /**
  * Default constructor
  */
  Timer(bool _canPause = false, bool _useAbs = false);

  /**
  * Resets the duration of the timer.
  * 
  * @param duration
  *  The duration of the timer.
  *
  * @param canPause
  *  Tells if timer can be affected by the paused time
  *  of the app.
  * 
  * @param useAbs
  *  Tells if it should use the absolute time of the aplication
  *  (Clock::nowAbs) or the regular (Clock::now)
  */
  void 
  reset(const Time& duration);

  /**
  * Gets the state of the timer.
  * 
  * @return
  *   True if the duration of the timer already elapsed,
  *   fase otherwise.
  */
  bool 
  done();
 private:
  Time m_duration;   
  bool m_canPause;
  bool m_useAbs;
};

}
