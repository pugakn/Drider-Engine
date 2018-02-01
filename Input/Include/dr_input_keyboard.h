#pragma once
#include "dr_input_prerequisites.h"
#include <OIS\OISKeyboard.h>
#include <dr_vector2di.h>
#include <vector>
#include <memory>
#include "dr_input_defines.h"
#include "dr_input_object.h"
namespace driderSDK {
  class KeyboardInput;
  /**
  * Helper class to receive events
  */
  class HelperKeyboardListener : public ::OIS::KeyListener
  {
  public:
    ~HelperKeyboardListener() {}
    explicit HelperKeyboardListener(KeyboardInput* m_pKeyboard);
    bool keyPressed(const OIS::KeyEvent &arg) override;
    bool keyReleased(const OIS::KeyEvent &arg) override;
  private:
    KeyboardInput* m_pKeyboard;
  };

  /**
  * Override this to receive events
  */
  class DR_INPUT_EXPORT IKeyboardListener
  {
  public:
    /**
    * Called when a button is pressed
    *
    * @param key
    *   The pressed key
    *
    */
    virtual bool 
    keyPressed(const KeyboardButtonID::E &key) = 0;
    /**
    * Called when a button is pressed
    *
    * @param key
    *   The released key
    *
    */
    virtual bool 
    keyReleased(const KeyboardButtonID::E &key) = 0;
  };

  /**
  * Manager class for input device
  */
  class DR_INPUT_EXPORT KeyboardInput : public InputObject {
  public:
    KeyboardInput(): m_helperListener(this) {}
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
    * Check if a key is down
    *
    * @param key
    *   The key to check
    *
    * @return 
    *   true if the key is down
    *
    */
    bool 
    isKeyDown(KeyboardButtonID::E key) const;
    /**
    * Check if a modifier
    *
    * @param mod
    *   The modifier to check
    *
    * @return
    *   true if the modifdier is down
    *
    */
    bool 
    isModifierDown(KeyboardModifier::E mod) const;
    /**
    * Register a listener
    *
    * @param listener
    *   The listener
    *
    */
    void 
    setEventCallback(IKeyboardListener *listener);
    std::vector<IKeyboardListener*> m_listeners;
  private:
    HelperKeyboardListener m_helperListener;
    OIS::Keyboard* m_keyboard;
  };
}