#include "dr_joystick.h"
#include <dr_math.h>
#include "dr_input_manager.h"

namespace driderSDK {

Joystick::Joystick(Pass) 
  : m_helper(*this),
    m_joystickOIS(nullptr),
    m_buttonsCallbacks(2)
{}

Joystick::~Joystick() {}

Int32 Joystick::getCount() {
  return InputManager::getJoystickCount();
}

Joystick* Joystick::get(Int32 index) {
  return InputManager::getJoystick(index);
}

bool 
Joystick::isButtonDown(JOYSTICK_BUTTON::E button) {
  
  Int32 b = getButtonID(button);
  
  return  m_joystickOIS->getJoyStickState().mButtons[b];
}

float 
Joystick::getAxis(JOYSTICK_AXIS::E axis) {

  Int32 a = getAxisID(axis);

  Int32 val = m_joystickOIS->getJoyStickState().mAxes[a].abs;  

  return val / 32768.f;
}

void 
Joystick::addButtonCallback(JOYSTICK_EVENT::E trigger, 
                            JOYSTICK_BUTTON::E button, 
                            Callback callback) {
  Int32 id = getButtonID(button);

  m_buttonsCallbacks[trigger][id].push_back(callback);
}

void Joystick::addAxisCallback(JOYSTICK_AXIS::E axis, 
                               Callback callback) {
  Int32 id = getAxisID(axis);

  m_axisCallbacks[id].push_back(callback);
}

Int32 
Joystick::getButtonID(JOYSTICK_BUTTON::E button) {
  
  using ButtonMap = std::unordered_map<JOYSTICK_BUTTON::E, Int32>;
  using ControlMap = std::unordered_map<String, ButtonMap>; 
  static ControlMap controlMapping = 
  { {"XBOX", {{JOYSTICK_BUTTON::kA, 0},
              {JOYSTICK_BUTTON::kB, 1},
              {JOYSTICK_BUTTON::kX, 2},
              {JOYSTICK_BUTTON::kY, 3},
              {JOYSTICK_BUTTON::kRB, 5},
              {JOYSTICK_BUTTON::kLB, 4},
              {JOYSTICK_BUTTON::kLS, 8},
              {JOYSTICK_BUTTON::kRS, 9},
              {JOYSTICK_BUTTON::kBack, 6},
              {JOYSTICK_BUTTON::kStart, 7},
             }
    }
  };

  return controlMapping["XBOX"][button];
}

Int32 Joystick::getAxisID(JOYSTICK_AXIS::E axis) {

  using AxisMap = std::unordered_map<JOYSTICK_AXIS::E, Int32>;
  using ControlMap = std::unordered_map<String, AxisMap>; 
  static ControlMap controlMapping = 
  { {"XBOX", {{JOYSTICK_AXIS::kLT, 4},
              {JOYSTICK_AXIS::kRT, 5},
              {JOYSTICK_AXIS::kLSHor, 1},
              {JOYSTICK_AXIS::kLSVer, 0},
              {JOYSTICK_AXIS::kRSHor, 3},
              {JOYSTICK_AXIS::kRSVer, 2}
             }
    }
  };

  return controlMapping["XBOX"][axis];
}

void 
Joystick::callButtonCallbacks(JOYSTICK_EVENT::E trigger, Int32 key) {
  
  auto& callbacks = m_buttonsCallbacks[trigger][key];
  
  for (auto& callback : callbacks) {
    callback(*this);
  }
}

void 
Joystick::callAxisCallbacks(Int32 key) {

  for (auto& callback : m_axisCallbacks[key]) {
    callback(*this);
  }
}


Joystick::Helper::Helper(Joystick& _parent) : m_parent(_parent) 
{}

bool
Joystick::Helper::buttonPressed(const OIS::JoyStickEvent& arg, 
                                Int32 button) {
  
  m_parent.callButtonCallbacks(JOYSTICK_EVENT::kButtonPressed, button);

  return true;
}
bool 
Joystick::Helper::buttonReleased(const OIS::JoyStickEvent& arg, 
                                 Int32 button) {

  m_parent.callButtonCallbacks(JOYSTICK_EVENT::kButtonnReleased, button);

  return true;
}
bool Joystick::Helper::axisMoved(const OIS::JoyStickEvent& arg, 
                                 Int32 axis) {

  m_parent.callAxisCallbacks(axis);

  return true;
}

}