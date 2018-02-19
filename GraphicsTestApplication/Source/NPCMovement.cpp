#include "NPCMovement.h"
#include <dr_gameObject.h>

void driderSDK::NPCMovement::onCreate() {}

void driderSDK::NPCMovement::onUpdate() {
    
  auto end = m_buttonsDown.end();

  if(m_buttonsDown.find(KEY_CODE::kUP) != end) {
    m_gameObject.transform.move({0.0f, 0.0f, -1.f});
  }
  else if(m_buttonsDown.find(KEY_CODE::kDOWN) != end) {
    m_gameObject.transform.move({0.0f, 0.0f, 1.f});
  }
  else if(m_buttonsDown.find(KEY_CODE::kLEFT) != end) {
    m_gameObject.transform.move({1.0f, 0.f, 0.f});
  }
  else if(m_buttonsDown.find(KEY_CODE::kRIGHT) != end) {
    m_gameObject.transform.move({-1.0f, 0.f, 0.f});
  }

}

void driderSDK::NPCMovement::onRender() {}

void driderSDK::NPCMovement::onDestroy() {}

bool driderSDK::NPCMovement::keyPressed(const KEY_CODE::E & key) {

  m_buttonsDown.insert(key);

  return true;
}

bool driderSDK::NPCMovement::keyReleased(const KEY_CODE::E & key) {

  m_buttonsDown.erase(key);

  return true;
}
