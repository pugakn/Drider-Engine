#include "dr_keyboard.h"
#include "dr_input_manager.h"

namespace driderSDK {

Keyboard::Keyboard(Pass) : m_callbacks(2), m_keyboardOIS(nullptr)
{}

Keyboard::~Keyboard() 
{}

bool Keyboard::isKeyDown(KEY_CODE::E key) {

  Keyboard* keyboard = InputManager::getKeyboard();

  OIS::KeyCode keyOIS = static_cast<OIS::KeyCode>(key);

  return keyboard->m_keyboardOIS->isKeyDown(keyOIS);
}

void 
Keyboard::addCallback(KEYBOARD_EVENT::E trigger, 
                      KEY_CODE::E key, 
                      Callback callback) {

  Keyboard* keyboard = InputManager::getKeyboard();

  keyboard->m_callbacks[trigger][key].push_back(callback);
}

bool 
Keyboard::keyPressed(const OIS::KeyEvent& arg) {
  
  callCallbacks(KEYBOARD_EVENT::kKeyPressed, 
                static_cast<KEY_CODE::E>(arg.key));

  return true;
}

bool 
Keyboard::keyReleased(const OIS::KeyEvent& arg) {

  callCallbacks(KEYBOARD_EVENT::kKeyReleased, 
                static_cast<KEY_CODE::E>(arg.key));

  return true;
}

void Keyboard::callCallbacks(KEYBOARD_EVENT::E trigger, 
                             KEY_CODE::E key) {

  auto& callbacks = m_callbacks[trigger][key];

  for (auto& callback : callbacks) {
    callback();
  }
}

}