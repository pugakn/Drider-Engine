#pragma once

#include <functional>
#include <unordered_map>
#include <OIS\OISJoyStick.h>
#include <dr_vector2d.h>
#include "dr_input_prerequisites.h"

namespace driderSDK {


class DR_INPUT_EXPORT Joystick
{

  struct Pass{};

 public:
  using AnyAxisCallback = std::function<void(Joystick&, JOYSTICK_AXIS::E)>;
  using AnyAxisCallbacks = std::vector<AnyAxisCallback>;
  using AnyButtonCallback = std::function<void(Joystick&, JOYSTICK_BUTTON::E)>;
  using AnyButtonCallbackList = std::vector<AnyButtonCallback>;
  using AnyButtonCallbacks = std::vector<AnyButtonCallbackList>;
  using Callback = std::function<void(Joystick&)>;
  using CallbackList = std::vector<Callback>;
  using CallbackMap = std::unordered_map<Int32, CallbackList>;
  using Callbacks = std::vector<CallbackMap>;

  Joystick(Pass);

  Joystick(const Joystick&) = delete;

  Joystick(Joystick&&) = delete;

  ~Joystick();

  /**
  * Gets the number of joysticks connected.
  */
  static Int32 
  getCount();

  static Joystick*
  get(Int32 index);
  /**
  * Gets the status (up/down) of a button.
  *
  * @param button
  *  The button from which the status will be checked.
  * 
  * @return
  *   True if the button is down, false otherwise.
  */
  bool
  isButtonDown(JOYSTICK_BUTTON::E button);
  
  /**
  * Gets the value of an axis
  * 
  * @param axis
  *  Axis from which the value is taken.
  * 
  * @return
  *   Value of the specified axis
  */
  float
  getAxis(JOYSTICK_AXIS::E axis);

  /**
  * Callback used when a button is pressed/released
  */
  void
  addButtonCallback(JOYSTICK_EVENT::E trigger, 
                    JOYSTICK_BUTTON::E button, 
                    Callback callback);

  /**
  * Callback used when an axis is moved/modified.
  */
  void
  addAxisCallback(JOYSTICK_AXIS::E axis, 
                  Callback callback);

  void
  addAnyButtonCallback(JOYSTICK_EVENT::E trigger,
                       AnyButtonCallback callback);

  void
  addAnyAxisCallback(AnyAxisCallback callback);

  Joystick& operator=(const Joystick&) = delete;

  Joystick& operator=(Joystick&&) = delete;

 private:

  friend class InputManager;

  struct Helper : public OIS::JoyStickListener
  {
  public:
    Helper(Joystick& _parent);

  private:
    virtual bool 
    buttonPressed(const OIS::JoyStickEvent& arg, 
                  Int32 button) override;

    virtual bool 
    buttonReleased(const OIS::JoyStickEvent& arg, 
                   Int32 button) override;

    virtual bool 
    axisMoved(const OIS::JoyStickEvent& arg, 
              Int32 axis) override;

    Joystick& m_parent;
  };

  Int32 
  getButtonID(JOYSTICK_BUTTON::E button);

  Int32
  getAxisID(JOYSTICK_AXIS::E axis);

  JOYSTICK_BUTTON::E
  getButtonFromID(Int32 id);

  JOYSTICK_AXIS::E
  getAxisFromID(Int32 id);

  void
  callButtonCallbacks(JOYSTICK_EVENT::E trigger, Int32 key);

  void
  callAxisCallbacks(Int32 key);


  Helper m_helper;
  const OIS::JoyStick* m_joystickOIS;
  AnyButtonCallbacks m_anyButtonCallbacks;
  AnyAxisCallbacks m_anyAxisCallbacks;
  CallbackMap m_axisCallbacks;
  Callbacks m_buttonsCallbacks;
};

}
