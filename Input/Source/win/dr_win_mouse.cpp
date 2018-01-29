#include "dr_input_mouse.h"
#include <OIS\OISMouse.h>
#include <OIS\win32\Win32Mouse.h>
namespace driderSDK {

void
MouseInput::internalInit(OIS::Mouse* mouse)
{
  //m_mouse = std::make_unique<OIS::Win32Mouse>();
  m_mouse = mouse;
  m_mouse->setEventCallback(&m_helperListener);
  
}

void MouseInput::capture()
{
  m_mouse->capture();
  OIS::MouseState st = m_mouse->getMouseState();
  m_state.m_pressedButtons = st.buttons;
  m_state.m_cursorPosition.x = st.X.abs;
  m_state.m_cursorPosition.y = st.Y.abs;
}

bool
MouseInput::isButtonDown(MouseButtonID::E button) const
{
  return m_mouse->getMouseState().buttonDown((OIS::MouseButtonID)button);
}

void 
MouseInput::setEventCallback(IMouseInputListener * mouseListener)
{
  m_listeners.push_back(mouseListener);
}

MouseInputState 
MouseInput::getState() const
{
  return m_state;
}
}