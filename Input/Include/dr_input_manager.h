#pragma once

#include <vector>
#include <OIS/OISInputManager.h>
#include <OIS/OISMouse.h>
#include <OIS/OISKeyboard.h>
#include <dr_memory.h>
#include <dr_module.h>
#include "dr_input_prerequisites.h"


namespace driderSDK {

class Mouse;
class Keyboard;

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
  * Captures all the devices input.
  */
  static void
  capture();
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
 //Private Members
private:
  SizeT m_windowHandle;
  std::unique_ptr<Mouse> m_mouse;
  std::unique_ptr<Keyboard> m_keyboard;
  std::vector<OIS::Object*> m_objects;
  OIS::InputManager* m_manager;  
};

}
