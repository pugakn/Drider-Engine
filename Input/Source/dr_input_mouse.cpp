#include "dr_input_mouse.h"

namespace driderSDK {

HelperMouseListener::HelperMouseListener(MouseInput* _mouse) : m_mouse(_mouse)
{}

bool 
HelperMouseListener::mouseMoved(const OIS::MouseEvent& arg) {
  m_mouse->m_state.pressedButtons = arg.state.buttons;
  m_mouse->m_state.cursorPosition.x = arg.state.X.abs;
  m_mouse->m_state.cursorPosition.y = arg.state.Y.abs;
  m_mouse->m_state.relativePosition.x = arg.state.X.rel;
  m_mouse->m_state.relativePosition.y = arg.state.Y.rel;
  for (auto &it : m_mouse->m_listeners) {
    it->mouseMoved(m_mouse->m_state);
  }
  return true;
}
bool 
HelperMouseListener::mousePressed(const OIS::MouseEvent& arg, 
                                  OIS::MouseButtonID id) {
  m_mouse->m_state.pressedButtons = arg.state.buttons;
  m_mouse->m_state.cursorPosition.x = arg.state.X.abs;
  m_mouse->m_state.cursorPosition.y = arg.state.Y.abs;
  m_mouse->m_state.relativePosition.x = arg.state.X.rel;
  m_mouse->m_state.relativePosition.y = arg.state.Y.rel;
  for (auto &it : m_mouse->m_listeners) {
    it->mousePressed(m_mouse->m_state, (driderSDK::MouseButtonID::E)id);
  }
  return true;
}
bool 
HelperMouseListener::mouseReleased(const OIS::MouseEvent& arg, 
                                   OIS::MouseButtonID id) {
  m_mouse->m_state.pressedButtons = arg.state.buttons;
  m_mouse->m_state.cursorPosition.x = arg.state.X.abs;
  m_mouse->m_state.cursorPosition.y = arg.state.Y.abs;
  m_mouse->m_state.relativePosition.x = arg.state.X.rel;
  m_mouse->m_state.relativePosition.y = arg.state.Y.rel;
  for (auto &it : m_mouse->m_listeners) {
    it->mouseReleased(m_mouse->m_state, (driderSDK::MouseButtonID::E)id);
  }
  return true;
}
void
MouseInput::internalInit(OIS::Object* mouse) {
  obj = mouse;
  m_mouse = reinterpret_cast<OIS::Mouse*>(obj);
  m_mouse->setEventCallback(&m_helperListener);
  const OIS::MouseState &ms = m_mouse->getMouseState();
  ms.width = 4096;   //Hardcoder max 4k
  ms.height = 4096;  //Hardcoder max 4k
}

void 
MouseInput::capture() {
  obj->capture();
  OIS::MouseState st = m_mouse->getMouseState();
  m_state.pressedButtons = st.buttons;
  m_state.cursorPosition.x = st.X.abs;
  m_state.cursorPosition.y = st.Y.abs;
  m_state.relativePosition.x = st.X.rel;
  m_state.relativePosition.y = st.Y.rel;
}

bool
MouseInput::isButtonDown(MouseButtonID::E button) const {
  return m_mouse->getMouseState().buttonDown((OIS::MouseButtonID)button);
}

void
MouseInput::setEventCallback(IMouseInputListener* mouseListener) {
  m_listeners.push_back(mouseListener);
}

MouseInputState
MouseInput::getState() const {
  return m_state;
}
}