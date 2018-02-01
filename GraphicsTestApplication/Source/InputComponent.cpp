#include "InputComponent.h"
#include <dr_gameObject.h>

namespace driderSDK {

void InputComponent::onCreate() {}

void InputComponent::onUpdate() {
  if (m_keysDown.count(KeyboardButtonID::KC_UP)) {
    m_gameObject.transform.move({1.0f,0.0f, 0.0f});
  }
  if (m_keysDown.count(KeyboardButtonID::KC_DOWN)) {
    m_gameObject.transform.move({-1.0f,0.0f, 0.0f});
  }
}

void InputComponent::onRender() {}

void InputComponent::onDestroy() {}

bool InputComponent::keyPressed(const KeyboardButtonID::E& key) {
  m_keysDown.insert(key);
  return true;
}

bool InputComponent::keyReleased(const KeyboardButtonID::E& key) {
  m_keysDown.erase(key);
  return true;
}

}