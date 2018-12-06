#include "dr_gameComponent.h"

#include <dr_logger.h>

#include "dr_gameObject.h"

#include <chrono>

namespace driderSDK {
GameComponent::GameComponent(GameObject& gameObject_, const TString& _name) 
  : NameObject(gameObject_.getValidName(_name)), 
    m_gameObject(gameObject_),
    m_isKilled(false)
{}

GameComponent::~GameComponent() {
  
  /*DR_DEBUG_ONLY(Logger::addLog(m_gameObject.getName() + _T(": ") + 
                               getName() + _T(" component destroyed")));*/

}

GameObject& 
GameComponent::getGameObject() {
  return m_gameObject;
}

GameObject*
GameComponent::getGameObjectPtr() {
  return &m_gameObject;
}

void
GameComponent::setName(const TString& name) {
  setName(m_gameObject.getValidName(name));
}

void 
GameComponent::kill() {
  m_isKilled = true;
}

bool 
GameComponent::isKilled() const {
  return m_isKilled;
}

}