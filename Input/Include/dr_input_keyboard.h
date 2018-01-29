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

  //Override this
  class DR_INPUT_EXPORT IKeyboardListener
  {
  public:
    virtual bool keyPressed(const KeyboardButtonID::E &key) = 0;
    virtual bool keyReleased(const KeyboardButtonID::E &key) = 0;
  };

  class DR_INPUT_EXPORT KeyboardInput : public InputObject {
  public:
    KeyboardInput(): m_helperListener(this) {}
    void capture() override;
    void internalInit(OIS::Object* obj) override;

    bool isKeyDown(KeyboardButtonID::E key) const;
    bool isModifierDown(KeyboardModifier::E mod) const;
    void setEventCallback(IKeyboardListener *listener);
    std::vector<IKeyboardListener*> m_listeners;
  private:
    HelperKeyboardListener m_helperListener;
    OIS::Keyboard* m_keyboard;
  };
}