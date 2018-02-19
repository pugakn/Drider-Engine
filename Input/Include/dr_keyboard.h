#pragma once

#include <functional>
#include <unordered_map>
#include <vector>
#include <OIS\OISKeyboard.h>
#include "dr_input_prerequisites.h"

namespace driderSDK {

class Keyboard : private OIS::KeyListener
{
  struct Pass{};

public:
 
  using Callback = std::function<void()>;
  using CallbackList = std::vector<Callback>;
  using CallbackMap = std::unordered_map<KEY_CODE::E, CallbackList>;
  using Callbacks = std::vector<CallbackMap>;

  /**
  * Default constructor using a private object
  * so it can only be created by friend classes.
  */
  Keyboard(Pass);

  Keyboard(const Keyboard&) = delete;

  Keyboard(Keyboard&&) = delete;

  ~Keyboard();

  /**
  * Gets the status (pressed/released) of a key.
  * 
  * @param key
  *  The key from which the status will be checked.
  *
  * @return
  *   True if the key is pressed, false otherwise.
  */
  static bool
  isKeyDown(KEY_CODE::E key);

  /**
  * Adds a callback
  * 
  * @param trigger
  *  The event that will trigger the callback.
  * 
  * @param key
  *  The key that will validate the trigger.
  */
  static void
  addCallback(KEYBOARD_EVENT::E trigger, 
              KEY_CODE::E key, 
              Callback callback);

  Keyboard& operator=(const Keyboard&) = delete;

  Keyboard& operator=(Keyboard&&) = delete;

private:
  friend class InputManager;
  
  // Inherited via KeyListener
  virtual bool 
  keyPressed(const OIS::KeyEvent& arg) override;

  // Inherited via KeyListener
  virtual bool 
  keyReleased(const OIS::KeyEvent& arg) override;

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
  callCallbacks(KEYBOARD_EVENT::E trigger, KEY_CODE::E key);

  const OIS::Keyboard* m_keyboardOIS;

  Callbacks m_callbacks;
};

}