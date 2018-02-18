#include "InputComponent.h"
#include <dr_camera.h>
#include <dr_gameObject.h>
#include <dr_quaternion.h>
#include <dr_matrix4x4.h>
#include <dr_time.h>
#include <iostream>

namespace driderSDK {

InputComponent::InputComponent(GameObject& _gameObject, 
                               JoystickInput* _joystick) 
  : GameComponent(_gameObject),
    m_joystick(_joystick),
    m_direction(0,0,1.0f)
{}

void InputComponent::onCreate() {}

void InputComponent::onUpdate() {
  
  constexpr Int32 maxAxes = 32767;
  constexpr Int32 minAxes = -32768;
    
  if(m_joystick) {

     auto state = m_joystick->getState();

    float dirX = static_cast<float>(state.axes[1]) / maxAxes;
    float dirY = static_cast<float>(state.axes[0]) / maxAxes;

    float rotY = static_cast<float>(state.axes[2]) / maxAxes;
    float rotX = static_cast<float>(state.axes[3]) / maxAxes;
    
    float velocity = 150.f * Time::instance().getDelta();
    float angVelocity = 90.0f * Time::instance().getDelta();

    if (Math::abs(dirY) < 0.1f)
      dirY = 0.0f;
 
    if (Math::abs(dirX) < 0.1f) {
      dirX = 0.0f;
    }

    if (Math::abs(rotY) < 0.1f)
      rotY = 0.0f;
 
    if (Math::abs(rotX) < 0.1f) {
      rotX = 0.0f;
    }


    Vector3D angles = m_gameObject.transform.getRotation();

    m_direction.z = cos(angles.x)*cos(angles.y);
    m_direction.x = -sin(angles.y);
    m_direction.y = sin(angles.x)*cos(angles.y);
    m_direction.normalize();
    
    Vector3D forward = m_direction * velocity * -dirY;
    Vector3D right = m_direction.cross({0.0f, 1.f, 0.0f}) * velocity * -dirX;
 
    int x = 0;
    m_gameObject.transform.move(forward + right);
    m_gameObject.transform.rotate({0.0f, 
                                   -rotX * angVelocity * Math::DEGREE_TO_RADIAN, 0});
  }

  

}

void InputComponent::onRender() {}

void InputComponent::onDestroy() {}

bool InputComponent::buttonPressed(const JoystickInputState& state, 
                                   Int32 button) {
  
  std::cout << button << std::endl;
  
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