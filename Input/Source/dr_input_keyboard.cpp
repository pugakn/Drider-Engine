#include "dr_input_keyboard.h"

namespace driderSDK {
  HelperKeyboardListener::HelperKeyboardListener(KeyboardInput * m_pKeyboard)
  {
    m_pKeyboard = m_pKeyboard;
  }
  bool HelperKeyboardListener::keyPressed(const OIS::KeyEvent & arg)
  {
    for (auto &it : m_pKeyboard->m_listeners) {
      it->keyPressed(static_cast<KeyboardButtonID::E>(arg.key));
    }
    return true;
  }
  bool HelperKeyboardListener::keyReleased(const OIS::KeyEvent & arg)
  {
    for (auto &it : m_pKeyboard->m_listeners) {
      it->keyReleased(static_cast<KeyboardButtonID::E>(arg.key));
    }
    return true;
  }
  void KeyboardInput::capture()
  {
    m_obj->capture();
  }
  void KeyboardInput::internalInit(OIS::Object * obj)
  {
    m_obj = obj;
    m_keyboard = reinterpret_cast<OIS::Keyboard*>(m_obj);
    m_keyboard->setEventCallback(&m_helperListener);
  }
  bool KeyboardInput::isKeyDown(KeyboardButtonID::E key) const
  {
    return m_keyboard->isKeyDown(static_cast<OIS::KeyCode>(key));
  }
  bool KeyboardInput::isModifierDown(KeyboardModifier::E mod) const
  {
    return m_keyboard->isModifierDown(static_cast<OIS::Keyboard::Modifier>(mod));
  }
  void KeyboardInput::setEventCallback(IKeyboardListener * listener)
  {
    m_listeners.push_back(listener);
  }
}