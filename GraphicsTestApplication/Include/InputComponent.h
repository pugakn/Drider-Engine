#pragma once

#include <unordered_set>
#include <dr_gameComponent.h>
#include <dr_input_joystick.h>

namespace driderSDK {

class InputComponent : public GameComponent, public IJoystickInputListener
{
 public:
  using GameComponent::GameComponent;
 private:
  // Inherited via GameComponent
  virtual void onCreate() override;

  virtual void onUpdate() override;

  virtual void onRender() override;

  virtual void onDestroy() override;
  
  
  // Inherited via IJoystickInputListener
  virtual bool buttonPressed(const JoystickInputState& state, 
                             Int32 button) override;

  virtual bool buttonReleased(const JoystickInputState& state, 
                              Int32 button) override;

  virtual bool axisMoved(const JoystickInputState& state, 
                         Int32 axis) override;
};

}

