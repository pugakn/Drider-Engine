#include "dr_input_joystick.h"

namespace driderSDK {
HelperJoystickListener::HelperJoystickListener(JoystickInput* _joystick)
  : m_joystick(_joystick)
{}

bool 
HelperJoystickListener::buttonPressed(const OIS::JoyStickEvent& arg, 
                                      Int32 button) {
  JoystickInputState st;
  st.buttons = arg.state.mButtons;
  for (auto &it : arg.state.mAxes) {
    st.axes.push_back(it.abs);
  }
  for (auto &it : m_joystick->m_listeners) {
    it->buttonPressed(st,button);
  }
  return true;
}
bool 
HelperJoystickListener::buttonReleased(const OIS::JoyStickEvent& arg, 
                                       Int32 button) {
  JoystickInputState st;
  st.buttons = arg.state.mButtons;
  for (auto &it : arg.state.mAxes) {
    st.axes.push_back(it.abs);
  }
  for (auto &it : m_joystick->m_listeners) {
    it->buttonReleased(st, button);
  }
  return true;
}
bool 
HelperJoystickListener::axisMoved(const OIS::JoyStickEvent& arg, Int32 axis) {
  JoystickInputState st;
  st.buttons = arg.state.mButtons;
  for (auto &it : arg.state.mAxes) {
    st.axes.push_back(it.abs);
  }
  for (auto &it : m_joystick->m_listeners) {
    it->axisMoved(st, axis);
  }
  return true;
}

void 
JoystickInput::capture() {
  m_joystick->capture();
  OIS::JoyStickState st = m_joystick->getJoyStickState();
  m_state.buttons = st.mButtons;
  for (auto &it : st.mAxes) {
    m_state.axes.push_back(it.abs);
  }
}
void 
JoystickInput::internalInit(OIS::Object* joystick) {
  obj = joystick;
  m_joystick = reinterpret_cast<OIS::JoyStick*>(obj);
  m_joystick->setEventCallback(&m_helperListener);
}
void 
JoystickInput::setEventCallback(IJoystickInputListener* joystickListener) {
  m_listeners.push_back(joystickListener);
}
JoystickInputState 
JoystickInput::getState() const {
  return m_state;;
}

}