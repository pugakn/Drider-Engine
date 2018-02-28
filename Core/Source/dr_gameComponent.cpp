#include "dr_gameComponent.h"

namespace driderSDK {

GameComponent::GameComponent(GameObject& gameObject_) 
  : m_gameObject(gameObject_) 
{}

GameObject & GameComponent::getGameObject() {
  return m_gameObject;
}

}