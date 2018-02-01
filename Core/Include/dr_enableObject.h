#pragma once

#include "dr_core_prerequisites.h"

namespace driderSDK {

class DR_CORE_EXPORT EnableObject
{
public:
  /**
  * Default constructor.
  * Sets the object as enabled by default without calling onEnabled.
  */
  EnableObject();

  /**
  * Sets the object enabled state.
  *
  * @param enabled
  *  The state of the object. If true the function onEnabled will
  *  be called, onDisabled otherwise.
  */
  void 
  setEnabled(bool enabled);

  /**
  * Checks if the object is enabled.
  * 
  * @return
  *   True if is enabled, false otherwise.   
  */
  bool 
  isEnabled();
protected:
  /**
  * Function called when the object is enabled.
  */
  virtual void 
  onEnabled() {}

  /**
  * Function called when the object is disabled.
  */
  virtual void 
  onDisabled(){}
private:
  bool m_enabled;
};

}
