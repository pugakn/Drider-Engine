#pragma once

#include <memory>
#include <vector>
#include <dr_vector2di.h>
#include <OIS/OISMouse.h>
#include "dr_input_defines.h"
#include "dr_input_object.h"
#include "dr_input_prerequisites.h"

namespace driderSDK {
class MouseInput;
/**
* Helper class to receive events
*/
class HelperMouseListener : public OIS::MouseListener
{
 public:
  ~HelperMouseListener() {}
  explicit HelperMouseListener(MouseInput* _mouse);
  bool 
  mouseMoved(const OIS::MouseEvent &arg) override;
  bool 
  mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) override;
  bool 
  mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) override;
 private:
  MouseInput* m_mouse;
};

/**
* Structure with the mouse state
*/
struct DR_INPUT_EXPORT MouseInputState 
{
  Int32 pressedButtons;
  Vector2DI cursorPosition;
  Vector2DI relativePosition;
};

/**
* Override this to receive events
*/
class DR_INPUT_EXPORT IMouseInputListener
{
 public:
  /**
  * Called when the mouse moves
  *
  * @param state
  *   The current state
  *
  */
  virtual bool 
  mouseMoved(const MouseInputState &state) = 0;
  /**
  * Called when a button is pressed
  *
  * @param state
  *   The current state
  *
  * @param pressedId
  *   The pressed button id
  *
  */
  virtual bool 
  mousePressed(const MouseInputState &state, MouseButtonID::E pressedId) = 0;
  /**
  * Called when a button is released
  *
  * @param state
  *   The current state
  *
  * @param pressedId
  *   The released button id
  *
  */
  virtual bool 
  mouseReleased(const MouseInputState &state, MouseButtonID::E pressedId) = 0;
};

/**
* Manager class for input device
*/
class DR_INPUT_EXPORT MouseInput : public InputObject{
 public:
  MouseInput() 
    : InputObject(InputObjectType::kMouse),
      m_helperListener(this) 
  {}
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
  * Check if a button is down
  *
  * @param button
  *   The button to check
  *
  * @return
  *   true if the button is down
  *
  */
  bool 
  isButtonDown(MouseButtonID::E button) const;
  /**
  * Register a mouseListener
  *
  * @param listener
  *   The listener
  *
  */
  void
  setEventCallback(IMouseInputListener *mouseListener);
  /**
  * Get the current state
  *
  * @return
  *   The current state
  *
  */
  MouseInputState 
  getState() const;

 private:
  friend HelperMouseListener;
  MouseInputState m_state;
  std::vector<IMouseInputListener*> m_listeners;
  OIS::Mouse* m_mouse;
  HelperMouseListener m_helperListener;
};
}