#include "NPCMovement.h"
#include <dr_gameObject.h>

void driderSDK::NPCMovement::onCreate() {}

void driderSDK::NPCMovement::onUpdate() {
    
  auto end = m_buttonsDown.end();

  if(m_buttonsDown.find(KeyboardButtonID::KC_UP) != end) {
    m_gameObject.transform.move({0.0f, 0.0f, -1.f});
  }
  else if(m_buttonsDown.find(KeyboardButtonID::KC_DOWN) != end) {
    m_gameObject.transform.move({0.0f, 0.0f, 1.f});
  }
  else if(m_buttonsDown.find(KeyboardButtonID::KC_LEFT) != end) {
    m_gameObject.transform.move({1.0f, 0.f, 0.f});
  }
  else if(m_buttonsDown.find(KeyboardButtonID::KC_RIGHT) != end) {
    m_gameObject.transform.move({-1.0f, 0.f, 0.f});
  }

}

void driderSDK::NPCMovement::onRender() {}

void driderSDK::NPCMovement::onDestroy() {}

bool driderSDK::NPCMovement::keyPressed(const KeyboardButtonID::E & key) {

  m_buttonsDown.insert(key);

  return true;
}

bool driderSDK::NPCMovement::keyReleased(const KeyboardButtonID::E & key) {

  m_buttonsDown.erase(key);

  return true;
}
