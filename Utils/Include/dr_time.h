#include <dr_module.h>
#include "dr_timer.h"
#include "dr_util_prerequisites.h"

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
  float
  getElapsed();

  /**
  * Gets the elapsed time since the begin of the app.
  * 
  * @return
  *   Total time in milliseconds.
  */
  float
  getElapsedMilli();

  /**
  * Gets the elapsed time since the begin of the app.
  * 
  * @return
  *   Total time in microseconds.
  */
  float
  getElapsedMicro();

  /**
  * Gets the duration of the frame (delta time).
  *  
  * @return 
  *   Elapsed time on seconds.
  */
  float
  getDelta();

  /**
  * Gets the duration of the frame (delta time).
  *  
  * @return 
  *   Elapsed time on milliseconds.
  */
  float
  getDeltaMilli();

  /**
  * Gets the duration of the frame (delta time).
  *  
  * @return 
  *   Elapsed time on microseconds.
  */
  float
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