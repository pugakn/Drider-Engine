#include "InputComponent.h"
#include <dr_gameObject.h>
#include <iostream>

namespace driderSDK {

void InputComponent::onCreate() {}

void InputComponent::onUpdate() {}

void InputComponent::onRender() {}

void InputComponent::onDestroy() {}

bool InputComponent::buttonPressed(const JoystickInputState& state, 
                                   int button) {
  
  std::cout << "Button " << button << " pressed" << std::endl;
  
  return true;
}

bool InputComponent::buttonReleased(const JoystickInputState& state, 
                                    int button) {

  std::cout << "Button " << button << " released" << std::endl;

  return true;
}

bool InputComponent::axisMoved(const JoystickInputState& state, 
                               int axis) {

  std::cout << "Joystick " << axis << " moved" << std::endl;

  return true;
}

}