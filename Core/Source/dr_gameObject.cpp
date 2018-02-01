#include "dr_gameObject.h"
#include "dr_gameComponent.h"

namespace driderSDK {

GameObject::GameObject(const TString& name, 
                       WeakNode parent)
  : Node(name, parent) 
{}

void 
GameObject::addComponent(ComponentPtr component) {
  m_components.push_back(std::move(component));
}

void
GameObject::updateImpl() {

  Node::updateImpl();

  for (auto& component : m_components) {
    component->onUpdate();
  }
}

void 
GameObject::draw() {

  for(auto& component : m_components) {
    component->onRender();
  }

  Node::draw();
}

}