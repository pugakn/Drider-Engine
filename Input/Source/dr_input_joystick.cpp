#include "dr_input_joystick.h"
namespace driderSDK {
  HelperJoystickListener::HelperJoystickListener(JoystickInput * pJoystick)
  {
    m_pJoystick = pJoystick;
  }
  bool HelperJoystickListener::buttonPressed(const OIS::JoyStickEvent & arg, int button)
  {
    JoystickInputState st;
    st.m_buttons = arg.state.mButtons;
    for (auto &it : arg.state.mAxes) {
      st.m_axes.push_back(it.abs);
      st.m_axesRelative.push_back(it.rel);
    }
    for (auto &it : m_pJoystick->m_listeners) {
      it->buttonPressed(st,button);
    }
    return true;
  }
  bool HelperJoystickListener::buttonReleased(const OIS::JoyStickEvent & arg, int button)
  {
    JoystickInputState st;
    st.m_buttons = arg.state.mButtons;
    for (auto &it : arg.state.mAxes) {
      st.m_axes.push_back(it.abs);
      st.m_axesRelative.push_back(it.rel);
    }
    for (auto &it : m_pJoystick->m_listeners) {
      it->buttonReleased(st, button);
    }
    return true;
  }
  bool HelperJoystickListener::axisMoved(const OIS::JoyStickEvent & arg, int axis)
  {
    JoystickInputState st;
    st.m_buttons = arg.state.mButtons;
    for (auto &it : arg.state.mAxes) {
      st.m_axes.push_back(it.abs);
      st.m_axesRelative.push_back(it.rel);
    }
    for (auto &it : m_pJoystick->m_listeners) {
      it->axisMoved(st, axis);
    }
    return true;
  }


  void JoystickInput::capture()
  {
    m_joystick->capture();
    OIS::JoyStickState st = m_joystick->getJoyStickState();
    m_state.m_buttons = st.mButtons;
    for (auto &it : st.mAxes) {
      m_state.m_axes.push_back(it.abs);
      m_state.m_axesRelative.push_back(it.rel);
    }
  }
  void JoystickInput::internalInit(OIS::Object * obj)
  {
    m_obj = obj;
    m_joystick = reinterpret_cast<OIS::JoyStick*>(m_obj);
    m_joystick->setEventCallback(&m_helperListener);
  }
  void JoystickInput::setEventCallback(IJoystickInputListener * joystickListener)
  {
    m_listeners.push_back(joystickListener);
  }
  JoystickInputState JoystickInput::getState() const
  {
    return m_state;;
  }
}