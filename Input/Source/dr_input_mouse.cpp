#include "dr_input_mouse.h"

namespace driderSDK {
  HelperMouseListener::HelperMouseListener(MouseInput * pMouse)
{
  m_pMouse = pMouse;
}
bool HelperMouseListener::mouseMoved(const OIS::MouseEvent & arg)
{
  MouseInputState st;
  st.m_pressedButtons = arg.state.buttons;
  st.m_cursorPosition.x = arg.state.X.abs;
  st.m_cursorPosition.y = arg.state.Y.abs;
  st.m_relativePosition.x = arg.state.X.rel;
  st.m_relativePosition.y = arg.state.Y.rel;
  for (auto &it : m_pMouse->m_listeners) {
    it->mouseMoved(st);
  }
  return true;
}
bool HelperMouseListener::mousePressed(const OIS::MouseEvent & arg, OIS::MouseButtonID id)
{
  MouseInputState st;
  st.m_pressedButtons = arg.state.buttons;
  st.m_cursorPosition.x = arg.state.X.abs;
  st.m_cursorPosition.y = arg.state.Y.abs;
  st.m_relativePosition.x = arg.state.X.rel;
  st.m_relativePosition.y = arg.state.Y.rel;
  for (auto &it : m_pMouse->m_listeners) {
    it->mousePressed(st, (driderSDK::MouseButtonID::E)id);
  }
  return true;
}
bool HelperMouseListener::mouseReleased(const OIS::MouseEvent & arg, OIS::MouseButtonID id)
{
  MouseInputState st;
  st.m_pressedButtons = arg.state.buttons;
  st.m_cursorPosition.x = arg.state.X.abs;
  st.m_cursorPosition.y = arg.state.Y.abs;
  st.m_relativePosition.x = arg.state.X.rel;
  st.m_relativePosition.y = arg.state.Y.rel;
  for (auto &it : m_pMouse->m_listeners) {
    it->mouseReleased(st, (driderSDK::MouseButtonID::E)id);
  }
  return true;
}
void
MouseInput::internalInit(OIS::Object* mouse)
{
  m_obj = mouse;
  m_mouse = reinterpret_cast<OIS::Mouse*>(m_obj);
  m_mouse->setEventCallback(&m_helperListener);
}

void MouseInput::capture()
{
  m_obj->capture();
  OIS::MouseState st = m_mouse->getMouseState();
  m_state.m_pressedButtons = st.buttons;
  m_state.m_cursorPosition.x = st.X.abs;
  m_state.m_cursorPosition.y = st.Y.abs;
  m_state.m_relativePosition.x = st.X.rel;
  m_state.m_relativePosition.y = st.Y.rel;
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