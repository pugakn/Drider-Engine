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
  for (auto &it : m_pMouse->m_listeners) {
    it->mouseReleased(st, (driderSDK::MouseButtonID::E)id);
  }
  return true;
}
}