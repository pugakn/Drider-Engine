#include "dr_keyboard.h"
#include "dr_input_manager.h"

namespace driderSDK {

Keyboard::Keyboard(Pass) 
  : m_callbacks(2), 
    m_keyboardOIS(nullptr), 
    m_helper(*this)
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

void Keyboard::callCallbacks(KEYBOARD_EVENT::E trigger, 
                             KEY_CODE::E key) {

  auto& callbacks = m_callbacks[trigger][key];

  for (auto& callback : callbacks) {
    callback();
  }
}

Keyboard::Helper::Helper(Keyboard& keyboard) : m_parent(keyboard) 
{}

bool 
Keyboard::Helper::keyPressed(const OIS::KeyEvent& arg) {

  m_parent.callCallbacks(KEYBOARD_EVENT::kKeyPressed, 
                         static_cast<KEY_CODE::E>(arg.key));

  return true;
}

bool 
Keyboard::Helper::keyReleased(const OIS::KeyEvent& arg) {

  m_parent.callCallbacks(KEYBOARD_EVENT::kKeyReleased, 
                         static_cast<KEY_CODE::E>(arg.key));

  return true;
}

}