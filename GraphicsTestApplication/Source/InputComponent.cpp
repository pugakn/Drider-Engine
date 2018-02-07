#include "InputComponent.h"
#include <dr_camera.h>
#include <dr_gameObject.h>
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
  
  if(m_joystick) {
    auto state = m_joystick->getState();

    m_dir.x = static_cast<float>(state.axes[1]) / maxAxes;
    m_dir.y = static_cast<float>(state.axes[0]) / maxAxes;

  }

  if (auto cam = dynamic_cast<Camera*>(&m_gameObject)) {
    cam->move(m_dir.y, m_dir.x);
  }

}

void InputComponent::onRender() {}

void InputComponent::onDestroy() {}

bool InputComponent::buttonPressed(const JoystickInputState& state, 
                                   Int32 button) {
  
  std::cout << "Button " << button << " pressed" << std::endl;
  
  return true;
}

bool InputComponent::buttonReleased(const JoystickInputState& state, 
                                    Int32 button) {

  std::cout << "Button " << button << " released" << std::endl;

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