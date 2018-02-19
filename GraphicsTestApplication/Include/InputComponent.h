//#pragma once
//
//#include <unordered_set>
//#include <dr_gameComponent.h>
//#include <dr_input_joystick.h>
//#include <dr_vector2d.h>
//
//namespace driderSDK {
//
//class InputComponent : public GameComponent, public IJoystickInputListener
//{
// public:
//  
//  InputComponent(GameObject& _gameObject, JoystickInput* _joystick);
//  
// private:
//  // Inherited via GameComponent
//  virtual void 
//  onCreate() override;
//
//  virtual void 
//  onUpdate() override;
//
//  virtual void 
//  onRender() override;
//
//  virtual void 
//  onDestroy() override;
//  
//  // Inherited via IJoystickInputListener
//  virtual bool 
//  buttonPressed(const JoystickInputState& state, 
//                Int32 button) override;
//
//  virtual bool 
//  buttonReleased(const JoystickInputState& state, 
//                 Int32 button) override;
//
//  virtual bool 
//  axisMoved(const JoystickInputState& state, 
//            Int32 axis) override;
//
// private:
//
//  void 
//  toggleSprint();
//  
//  static constexpr float runSpeed = 300.f;
//  static constexpr float walkSpeed = 170.f;
//
//  bool m_running;
//
//  float m_velocity;
//
//  JoystickInput* m_joystick;
//
//  Vector3D m_direction;
//};
//
//}
//
