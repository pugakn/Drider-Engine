#pragma once
#include "dr_input_prerequisites.h"
#include <OIS/OISInputManager.h>
#include <vector>
#include "dr_module.h"
#include "dr_input_object.h"
class OIS::InputManager;
namespace driderSDK {
class MouseInput;
class KeyboardInput;
class JoystickInput;
class DR_INPUT_EXPORT InputManager : public Module<InputManager> {
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
  init(SizeT winHandle,bool nonExlusive = true);
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
  * Get the number of devices connected
  *
  * @return 
  *   The total devices connected
  *
  */
  Int32
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

  MouseInput* 
  getMouse();
  KeyboardInput* 
  getKeyboard();
  JoystickInput* 
  getJoystick(UInt32 id);
  UInt32 
  getNumOfJoysticks();
private:
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
  * Register all free devices
  *
  */
  void
  registerAllActiveDevices();


  std::vector<MouseInput*> m_mouseDevices;
  std::vector<KeyboardInput*> m_keyboardDevices;
  std::vector<JoystickInput*> m_joystickDevices;
  std::vector<InputObject*> m_objects;
  OIS::InputManager* m_mngr;
};
}