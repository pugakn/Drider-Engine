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
  class HelperJoystickListener : public ::OIS::JoyStickListener
  {
  public:
    ~HelperJoystickListener() {}
    explicit HelperJoystickListener(JoystickInput* pJoystick);
    bool
      buttonPressed(const OIS::JoyStickEvent &arg, int button) override;
    bool
      buttonReleased(const OIS::JoyStickEvent &arg, int button) override;
    bool
      axisMoved(const OIS::JoyStickEvent &arg, int axis) override;
  private:
    JoystickInput* m_pJoystick;
  };


  struct DR_INPUT_EXPORT JoystickInputState {
    std::vector<bool> m_buttons;
    std::vector<int> m_axes;
    std::vector<int> m_axesRelative;
  };

  //Override this
  class DR_INPUT_EXPORT IJoystickInputListener
  {
  public:
    virtual bool
      buttonPressed(const JoystickInputState &state, int button) = 0;
    virtual bool
      buttonReleased(const JoystickInputState &state, int button) =  0;
    virtual bool
      axisMoved(const JoystickInputState &state, int axis) = 0;
  };

  class DR_INPUT_EXPORT JoystickInput : public InputObject {
  public:
    JoystickInput() : m_helperListener(this) {}
    void
    capture() override;
    void
    internalInit(OIS::Object* obj) override;

    void
    setEventCallback(IJoystickInputListener *joystickListener);

    JoystickInputState 
    getState() const;


    std::vector<IJoystickInputListener*> m_listeners;
  private:
    JoystickInputState m_state;
    OIS::JoyStick* m_joystick;
    HelperJoystickListener m_helperListener;
  };
}