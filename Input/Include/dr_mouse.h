#pragma once

#include <functional>
#include <unordered_map>
#include <vector>
#include <OIS\OISMouse.h>
#include <dr_vector2di.h>
#include "dr_input_prerequisites.h"

namespace driderSDK {


class DR_INPUT_EXPORT Mouse
{
  struct Pass{};

public:
  /*struct EventArgs 
  {
    Int32 pressedButtons;
    Vector2DI absolutePos;
    Vector2DI relativePos;
  };*/

  using Callback = std::function<void()>;
  using CallbackList = std::vector<Callback>;
  using CallbackMap = std::unordered_map<Int32, CallbackList>;
  using Callbacks = std::vector<CallbackMap>;

  /**
  * Default constructor using a private object
  * so it can only be created by friend classes.
  */
  Mouse(Pass);

  Mouse(const Mouse&) = delete;

  Mouse(Mouse&&) = delete;

  ~Mouse();

  /**
  * Gets the position of the mouse.
  * 
  * @return 
  *   The position of the mouse.
  */
  static Vector2DI 
  getPosition();

  /**
  * Gets the delta of the movement from the last position.
  *
  * @return
  *   Delta of the movement.
  */
  static Vector2DI
  getDisplacement();

  /**
  * Gets the status (down/up) of a button.
  * 
  * @param button
  *  The button from which the status will be checked.
  *
  * @return
  *   True if the button is pressed, false otherwise.
  */
  static bool
  isButtonDown(MOUSE_BUTTON::E button);

  /**
  * Adds a callback
  * 
  * @param trigger
  *  The event that will trigger the callback.
  * 
  * @param button
  *  The button that will validate the trigger.
  */
  static void
  addCallback(MOUSE_INPUT_EVENT::E trigger, 
              MOUSE_BUTTON::E button, 
              Callback callback);

  Mouse& operator=(const Mouse&) = delete;

  Mouse& operator=(Mouse&&) = delete;

private:  

  friend class InputManager; 
  
  class Helper : public OIS::MouseListener
  {
  public:
    Helper(Mouse& mouse);
  private:
    // Inherited via MouseListener
    virtual bool 
    mouseMoved(const OIS::MouseEvent& arg) override;

    virtual bool 
    mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id) override;

    virtual bool 
    mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id) override;

    Mouse& m_parent;
  };
  
  /**
  * Internaly calls all the callbacks of an specific type 
  * and key.
  * 
  * @param trigger
  *  The type of the callback.
  * 
  * @param key
  *  The key that validates the callback.
  */
  void
  callCallbacks(MOUSE_INPUT_EVENT::E trigger, Int32 key);

  Helper m_helper;
  const OIS::Mouse* m_mouseOIS;
  Callbacks m_callbacks;
};

}
