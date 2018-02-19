#pragma once

#include <OIS/OISInputManager.h>
#include <OIS/OISMouse.h>
#include <OIS/OISKeyboard.h>
#include <dr_memory.h>
#include <dr_module.h>
#include "dr_input_prerequisites.h"


namespace driderSDK {

class Mouse;
class Keyboard;

class InputManager : public Module<InputManager>
{
 //Public Methods
public:

  InputManager(SizeT _windowHandle);
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
  OIS::Mouse* m_mouseOIS;
  OIS::Keyboard* m_keyboardOIS;
  OIS::InputManager* m_manager;  
};

}
