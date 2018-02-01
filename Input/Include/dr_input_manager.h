#pragma once
#include "dr_input_prerequisites.h"
#include <OIS/OISInputManager.h>
#include <vector>
#include "dr_input_object.h"
class OIS::InputManager;
namespace driderSDK {
class DR_INPUT_EXPORT InputManager {
public:
  /**
  * Init the manager
  *
  * @param winHandle
  *   The window handler
  *
  * @param nonExlusive
  *   If true, the application will not take controll of input devices
  *
  */
  void 
  init(std::size_t winHandle,bool nonExlusive = true);
  /**
  * Destroy the manager
  *
  */
  void 
  destroy();
  /**
  * Call it to capture events from all devices
  *
  */
  void 
  captureAll();
  /**
  * Creates an input object from a free device
  *
  * @param type
  *   The type of input object
  *
  * @return
  *   ID on m_objects
  *
  */
  UInt32 
  createInputObject(InputObjectType::E type);
  /**
  * Destroy a input object
  *
  * @param objID
  *   The input object id
  *
  */
  void 
  destroyInputObject(UInt32 objID);
  /**
  * Get the number of devices connected
  *
  * @return 
  *   The total devices connected
  *
  */
  int 
  getNumberOfDevices(InputObjectType::E type);
  /**
  * Get a input object by id
  *
  * @return
  *   The input object or nullptr
  *
  */
  InputObject* 
  getInputObjectByID(UInt32 id);

private:
  std::vector<InputObject*> m_objects;
  OIS::InputManager* m_mngr;
};
}