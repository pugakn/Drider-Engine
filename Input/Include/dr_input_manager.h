#pragma once

#include <vector>
#include <OIS/OISInputManager.h>
#include <dr_memory.h>
#include <dr_module.h>
#include "dr_input_prerequisites.h"


namespace driderSDK {

class Mouse;
class Keyboard;
class Joystick;

class DR_INPUT_EXPORT InputManager : public Module<InputManager>
{
 //Public Methods
 public:

  InputManager(SizeT _windowHandle);

  ~InputManager();
  /**
  * Gets the mouse.
  */
  static Mouse* 
  getMouse();

  /**
  * Gets the keyboard.
  */
  static Keyboard*
  getKeyboard();

  /**
  * Gets the specified keyboard
  */
  static Joystick*
  getJoystick(Int32 index);

  static Int32
  getJoystickCount();

  /**
  * Captures all the devices input.
  */
  static void
  update();

 //Private Methods
 private:
  void
  onStartUp();

  void
  onShutDown();

  void
  registerMouse();

  void
  registerKeyboard();

  void
  registerJoysticks();
 //Private Members
 private:
  SizeT m_windowHandle;
  std::unique_ptr<Mouse> m_mouse;
  std::unique_ptr<Keyboard> m_keyboard;
  std::vector<std::unique_ptr<Joystick>> m_joysticks;
  std::vector<OIS::Object*> m_objects;
  OIS::InputManager* m_manager;  
};

}
