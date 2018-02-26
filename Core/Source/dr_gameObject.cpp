#include "dr_gameObject.h"
#include <algorithm>
#include "dr_gameComponent.h"

namespace driderSDK {

GameObject::GameObject(const TString& name)
  : m_name(name)
{
}

GameObject::~GameObject() 
{}

void 
GameObject::init() {}

void
GameObject::update() {

  updateImpl();

  for (auto& component : m_components) {
    component->onUpdate();
  }

  for (auto& child : m_children) {
    child->update();
  }
}

void 
GameObject::render() {

  for (auto& component : m_components) {
    component->onRender();
  }

  for (auto& child : m_children) {
    child->render();
  }
}

//void 
//GameObject::destroy() {}

void 
GameObject::addComponent(ComponentPtr component) {
  m_components.push_back(std::move(component));
}

void 
GameObject::addChild(SharedGameObj child) {
  //if child already exist in children list don't add
  if (child && 
      std::find(m_children.begin(), m_children.end(), child) == m_children.end()) {

    m_children.push_back(child);
    auto thisPtr = shared_from_this();
    auto childPastParent = child->getParent();

    if (childPastParent) {
      //remove child from children list of previus parent
      childPastParent->removeChild(child);
    }

    child->setParent(thisPtr);
  }
}

void 
GameObject::setParent(SharedGameObj parent) {
  auto lastParent = getParent();
  auto thisPtr = shared_from_this();

  if (!parent) {
    m_parent.reset();
  }
  else if (lastParent != parent) {

    if (lastParent) {
      lastParent->removeChild(thisPtr);
    }

    parent->addChild(thisPtr);
    m_parent = parent;
  }
}

GameObject::SharedGameObj 
GameObject::getParent() {
  return m_parent.lock();
}

void GameObject::removeChild(SharedGameObj child) {
  auto it = std::find(m_children.begin(), m_children.end(), child);

  if (it != m_children.end()) {
    (*it)->setParent(nullptr);
    m_children.erase(it);
  }
}

void GameObject::removeChildren() {

  for (auto& child : m_children) {
    child->setParent(nullptr);
  }

  m_children.clear();
}

void GameObject::setName(const TString& name) {
  m_name = name;
}

const TString& 
GameObject::getName() {
  return m_name;
}

const Transform&
GameObject::getWorldTransform() const {
  return m_finalTransform;
}

Transform& 
GameObject::getTransform() {
  return m_localTransform;
}

GameObject::ChildrenList 
GameObject::getChildrenWithName(const TString& childrenNames) {
  std::vector<SharedGameObj> children;
  
  for (auto& child : m_children) {
    if (child->getName() == childrenNames) {
      children.push_back(child);
    }
  }

  return children;
}

const GameObject::ChildrenList& 
GameObject::getChildren() {
  return m_children;
}

GameObject::SharedGameObj 
GameObject::getChild(const TString& childName) {

  SharedGameObj ch;

  for (auto& child : m_children) {
    if (childName == child->getName()) {
      ch = child;
      break;
    }
  }

  return ch;
}

GameObject::SharedGameObj 
GameObject::getChild(SizeT index) {
  
  DR_ASSERT(index < m_children.size());
  
  return m_children[index];
}

GameObject::SharedGameObj 
GameObject::findNode(const TString & nodeName) {
  
  SharedGameObj node = getChild(nodeName);

  if (!node) {
    for (auto& child : m_children) {
      if (node = child->findNode(nodeName)) {
        break;
      }
    }
  }

  return node;
}

SizeT 
GameObject::getChildrenCount() {
  return m_children.size();
}

void GameObject::updateImpl() {
  m_finalTransform = getParent()->m_finalTransform * m_localTransform;
}

}