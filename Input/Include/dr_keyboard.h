#pragma once

#include <functional>
#include <unordered_map>
#include <vector>
#include <OIS\OISKeyboard.h>
#include "dr_input_prerequisites.h"

#include <dr_export_script.h>
#include <..\..\Script\Include\dr_script_engine.h>

namespace driderSDK {

class DR_INPUT_EXPORT Keyboard
{
  struct Pass{};

 public:
 
  using AnyKeyCallback = std::function<void(KEY_CODE::E key)>;
  using AnyKeyCallbackList = std::vector<AnyKeyCallback>;
  using AnyKeyCallbacks = std::vector<AnyKeyCallbackList>;
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

  static bool
  keyDown(KEY_CODE::E key);

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

  /**
  * Adds a callback triggered with any key pressed/released.
  */
  static void
  addAnyKeyCallback(KEYBOARD_EVENT::E trigger,
                    AnyKeyCallback callback);

  static const char
  getAsChar(KEY_CODE::E key);

  Keyboard& operator=(const Keyboard&) = delete;

  Keyboard& operator=(Keyboard&&) = delete;


  static BEGINING_DEFAULT_REGISTER
  
  REGISTER_ENUM(KeyCode)
  REGISTER_ENUM_VALUE(KeyCode, KEY_CODE, kESCAPE)
  REGISTER_ENUM_VALUE(KeyCode, KEY_CODE, k0)
  REGISTER_ENUM_VALUE(KeyCode, KEY_CODE, k1)
  REGISTER_ENUM_VALUE(KeyCode, KEY_CODE, k2)
  REGISTER_ENUM_VALUE(KeyCode, KEY_CODE, k3)
  REGISTER_ENUM_VALUE(KeyCode, KEY_CODE, k4)
  REGISTER_ENUM_VALUE(KeyCode, KEY_CODE, k5)
  REGISTER_ENUM_VALUE(KeyCode, KEY_CODE, k6)
  REGISTER_ENUM_VALUE(KeyCode, KEY_CODE, k7)
  REGISTER_ENUM_VALUE(KeyCode, KEY_CODE, k8)
  REGISTER_ENUM_VALUE(KeyCode, KEY_CODE, k9)
  REGISTER_ENUM_VALUE(KeyCode, KEY_CODE, kA)
  REGISTER_ENUM_VALUE(KeyCode, KEY_CODE, kS)
  REGISTER_ENUM_VALUE(KeyCode, KEY_CODE, kD)
  REGISTER_ENUM_VALUE(KeyCode, KEY_CODE, kW)

  REGISTER_GLO_FOO_1P(&Keyboard::isKeyDown, isKeyDown, KEY_CODE::E, KeyCode, bool, "bool")
  REGISTER_GLO_FOO_1P(&Keyboard::keyDown, keyDown, KEY_CODE::E, KeyCode, bool, "bool")

  END_REGISTER

 private:

  friend class InputManager;
  
  class Helper : public OIS::KeyListener
  {
   public:
    Helper(Keyboard& keyboard);
   private:
    // Inherited via KeyListener
    virtual bool 
    keyPressed(const OIS::KeyEvent & arg) override;

    virtual bool 
    keyReleased(const OIS::KeyEvent & arg) override;

    Keyboard& m_parent;
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
  callCallbacks(KEYBOARD_EVENT::E trigger, KEY_CODE::E key);

  Helper m_helper;
  const OIS::Keyboard* m_keyboardOIS;
  Callbacks m_callbacks;
  AnyKeyCallbacks m_anyKeyCallbacks;
};

}