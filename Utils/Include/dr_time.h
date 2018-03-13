#pragma once
#include <dr_module.h>
#include "dr_timer.h"
#include "dr_util_prerequisites.h"

#include "dr_export_script.h"
#include <..\..\Script\Include\dr_script_engine.h>

namespace driderSDK {

class DR_UTIL_EXPORT Time : public Module<Time>
{
  friend class Application;
 public:
  /**
  * Gets the elapsed time since the begin of the app.
  * 
  * @return
  *   Total time in seconds.
  */
  static float
  getElapsed();

  /**
  * Gets the elapsed time since the begin of the app.
  * 
  * @return
  *   Total time in milliseconds.
  */
  static float
  getElapsedMilli();

  /**
  * Gets the elapsed time since the begin of the app.
  * 
  * @return
  *   Total time in microseconds.
  */
  static float
  getElapsedMicro();

  /**
  * Gets the duration of the frame (delta time).
  *  
  * @return 
  *   Elapsed time on seconds.
  */
  static float
  getDelta();

  /**
  * Gets the duration of the frame (delta time).
  *  
  * @return 
  *   Elapsed time on milliseconds.
  */
  static float
  getDeltaMilli();

  /**
  * Gets the duration of the frame (delta time).
  *  
  * @return 
  *   Elapsed time on microseconds.
  */
  static float
  getDeltaMicro();
  
  static void 
  update();

  static BEGINING_DEFAULT_REGISTER

  result = REGISTER_GLO_FOO_0P(&Time::getDelta, getDelta, float, "float")

  END_REGISTER

 private:  
  
  void
  onStartUp();

 private:
  Timer m_timer;
  float m_elapsed;
  float m_lastUpdate;
};

}