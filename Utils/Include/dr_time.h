#include <dr_module.h>
#include "dr_timer.h"
#include "dr_util_prerequisites.h"

#define STATIC_MODULE static

namespace driderSDK {

class DR_UTIL_EXPORT Time : public Module<Time>
{
 public:

  /**
  * Gets the elapsed time since the begin of the app.
  * 
  * @return
  *   Total time in seconds.
  */
  STATIC_MODULE float
  getElapsed();

  /**
  * Gets the elapsed time since the begin of the app.
  * 
  * @return
  *   Total time in milliseconds.
  */
  STATIC_MODULE float
  getElapsedMilli();

  /**
  * Gets the elapsed time since the begin of the app.
  * 
  * @return
  *   Total time in microseconds.
  */
  STATIC_MODULE float
  getElapsedMicro();

  /**
  * Gets the duration of the frame (delta time).
  *  
  * @return 
  *   Elapsed time on seconds.
  */
  STATIC_MODULE float
  getDelta();

  /**
  * Gets the duration of the frame (delta time).
  *  
  * @return 
  *   Elapsed time on milliseconds.
  */
  STATIC_MODULE float
  getDeltaMilli();

  /**
  * Gets the duration of the frame (delta time).
  *  
  * @return 
  *   Elapsed time on microseconds.
  */
  STATIC_MODULE float
  getDeltaMicro();
 //private:
  
  void 
  update();

  void
  onStartUp();
 private:
  Timer m_timer;
  float m_elapsed;
  float m_lastUpdate;
};

}