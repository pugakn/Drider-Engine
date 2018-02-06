#pragma once
#include "dr_input_prerequisites.h"
#include <OIS\OISJoystick.h>
#include <dr_vector2di.h>
#include <dr_vector3d.h>
#include <vector>
#include "dr_input_defines.h"
#include "dr_input_object.h"
namespace driderSDK {
class JoystickInput;
/**
* Helper class to receive events
*/
class HelperJoystickListener : public ::OIS::JoyStickListener
{
public:
  ~HelperJoystickListener() {}
  explicit HelperJoystickListener(JoystickInput* pJoystick);
  bool
    buttonPressed(const OIS::JoyStickEvent &arg, Int32 button) override;
  bool
    buttonReleased(const OIS::JoyStickEvent &arg, Int32 button) override;
  bool
    axisMoved(const OIS::JoyStickEvent &arg, Int32 axis) override;
private:
  JoystickInput* m_pJoystick;
};

/**
* Structure with the joystick state
*/
struct DR_INPUT_EXPORT JoystickInputState {
  std::vector<bool> m_buttons;
  std::vector<Int32> m_axes;
};

/**
* Override this to receive events
*/
class DR_INPUT_EXPORT IJoystickInputListener
{
public:
  /**
  * Called when a button is pressed
  *
  * @param state
  *   The current state of the joystick
  *
  * @param state
  *   The pressed button id
  *
  */
  virtual bool
    buttonPressed(const JoystickInputState &state, Int32 button) = 0;
  /**
  * Called when a button is released
  *
  * @param state
  *   The current state of the joystick
  *
  * @param state
  *   The pressed button id
  *
  */
  virtual bool
    buttonReleased(const JoystickInputState &state, Int32 button) =  0;
  /**
  * Called when an axis is moved
  *
  * @param state
  *   The current state of the joystick
  *
  * @param axis
  *   The moved axis id
  *
  */
  virtual bool
    axisMoved(const JoystickInputState &state, Int32 axis) = 0;
};

/**
* Manager class for input device
*/
class DR_INPUT_EXPORT JoystickInput : public InputObject {
public:
  JoystickInput() : m_helperListener(this) {}
  /**
  * Call it to capture events
  *
  */
  void
  capture() override;
  /**
  * Do not call it! you could die 
  *
  */
  void
  internalInit(OIS::Object* obj) override;

  /**
  * Register a listener
  *
  * @param joystickListener
  *   The listener
  *
  */
  void
  setEventCallback(IJoystickInputListener *joystickListener);

  /**
  * Get the current state
  *
  * @return 
  *   The current state
  *
  */
  JoystickInputState 
  getState() const;


  std::vector<IJoystickInputListener*> m_listeners;
private:
  JoystickInputState m_state;
  OIS::JoyStick* m_joystick;
  HelperJoystickListener m_helperListener;
};
}