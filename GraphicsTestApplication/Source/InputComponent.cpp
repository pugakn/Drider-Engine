#include "InputComponent.h"
#include <dr_gameObject.h>
#include <iostream>

namespace driderSDK {

void InputComponent::onCreate() {}

void InputComponent::onUpdate() {}

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

  constexpr Int32 maxAxes = 32767;
  constexpr Int32 minAxes = -32768;



  return true;
}

}