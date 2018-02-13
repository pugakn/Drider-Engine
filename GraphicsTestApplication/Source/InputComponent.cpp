#include "InputComponent.h"
#include <dr_camera.h>
#include <dr_gameObject.h>
#include <dr_quaternion.h>
#include <dr_matrix4x4.h>
#include <dr_matrix3x3.h>
#include <iostream>

namespace driderSDK {

InputComponent::InputComponent(GameObject& _gameObject, 
                               JoystickInput* _joystick) 
  : GameComponent(_gameObject),
    m_joystick(_joystick),
    m_dir(Math::FORCE_INIT::kZero)
{}

void InputComponent::onCreate() {}

void InputComponent::onUpdate() {
  
  constexpr Int32 maxAxes = 32767;
  constexpr Int32 minAxes = -32768;
    
  JoystickInputState* state = nullptr;
  JoystickInputState st;
  if(m_joystick) {
    st = m_joystick->getState();

    float rowX = static_cast<float>(st.axes[1]) / maxAxes;
    float rowY = static_cast<float>(st.axes[0]) / maxAxes;

    if (Math::abs(rowX) > 0.1f) {
      m_dir.x = rowX;
    }
    else { 
      m_dir.x = 0;
    }

    if (Math::abs(rowY) > 0.1f) {
      m_dir.y = rowY;
    } 
    else {
      m_dir.y = 0;
    }


    float horizontal = static_cast<float>(st.axes[3]) / maxAxes;
    float vertical = static_cast<float>(st.axes[2]) / maxAxes;

    state = &st;

  }

  if (auto cam = dynamic_cast<Camera*>(&m_gameObject)) {
    cam->pan(-m_dir.y, -m_dir.x);


    if (state) {

      //Down
      if(state->buttons[4]) {
        
      }
      //Up
      else if (state->buttons[5]) {

      }
    }
    
    
  }

}

void InputComponent::onRender() {}

void InputComponent::onDestroy() {}

bool InputComponent::buttonPressed(const JoystickInputState& state, 
                                   Int32 button) {
  
  
  
  return true;
}

bool InputComponent::buttonReleased(const JoystickInputState& state, 
                                    Int32 button) {

  

  return true;
}

bool InputComponent::axisMoved(const JoystickInputState& state, 
                               Int32 axis) {
  //0 vertical
  //1 horizontals

  /*std::cout << "New input" << std::endl;

  for (SizeT i = 0; i < state.axes.size(); ++i) {
    std::cout << "Axis " << i << " val:" << state.axes[i] << std::endl;
  }*/

  return true;
}

}