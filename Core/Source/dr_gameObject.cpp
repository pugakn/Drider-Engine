#include "dr_gameObject.h"

#include <algorithm>

#include <dr_string_utils.h>

#include "dr_gameComponent.h"
#include "dr_script_component.h"

namespace driderSDK {

GameObject* Ref_GameObject() {
  return new GameObject();
}

GameObject::GameObject(const TString& name)
  : NameObject(name), 
    m_tag(_T("UNTAGGED")),
    m_isStatic(false),
    m_change(false)
    DR_DEBUG_ONLY_PARAM(m_destroyed(false))
{
  refCount = 1;
}

GameObject::~GameObject() {
  DR_DEBUG_ONLY(
  if (!m_destroyed) {
    Logger::addLog(_T("Object called destructor without begin destoyed() first"));
  });
}

void
GameObject::update() {

  m_change = m_localTransform.changed() ||
             getParent()->changed();

  m_localTransform.m_change = false;

  if (m_change) {
    m_finalTransform = m_localTransform * getParent()->m_finalTransform;  
  }   

  updateImpl();

  for (auto& component : m_components) {
    
    component->onUpdate();

    //If the local transform changed inside the component update
    if (m_localTransform.changed()) {

      //Recalculate the final transform
      m_finalTransform = m_localTransform * getParent()->m_finalTransform;

      m_localTransform.m_change = false;
    }

  }

  if (!m_componentsToRemove.empty()) {

    for (const auto& cmpToRem : m_componentsToRemove) {
      removeComponentP(cmpToRem);
    }

    m_componentsToRemove.clear();
  }

  for (auto& child : m_children) {
    child->update();
  }

  m_finalTransform.m_change = false;
}

void 
GameObject::render() {

  for (auto& component : m_components) {
    component->onRender();
  }

  /*for (auto& child : m_children) {
    child->render();
  }*/
}

void
GameObject::destroy() {
  
  DR_DEBUG_ONLY(m_destroyed = true);

  for (auto& c : m_components) {
    c->onDestroy();
  }

  m_components.clear();

  for (auto& child : m_children) {
    child->destroy();
  }

  m_children.clear();
}

//void 
//GameObject::destroy() {}

void
GameObject::removeComponent(const TString& compName) {
  m_componentsToRemove.insert(compName);
}

void
GameObject::removeComponentP(const TString& compName) {

  for (auto it = m_components.begin(); it != m_components.end(); ++it) {
    if ((*it)->getName() == compName) {
      (*it)->onDestroy();
      m_components.erase(it);
      return;
    }
  }

  DR_DEBUG_ONLY(Logger::addLog(_T("Trying to remove unexisting component: ") + 
                               compName));
}

void
GameObject::addComponent(ComponentPtr component) {

  DR_ASSERT(&component->getGameObject() == this);

  m_components.push_back(std::move(component));

  std::stable_partition(m_components.begin(),
                        m_components.end(),
                        ComponentPartition{});
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
GameObject::getParent() const {
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

const Transform&
GameObject::getWorldTransform() const {
  return m_finalTransform;
}

Transform& 
GameObject::getTransform() {
  return m_localTransform;
}

const TString& 
GameObject::getTag() const {
  return m_tag.getName();
}

void 
GameObject::setTag(const TString& _tag) {
  m_tag.setName(_tag);
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

GameObject*
GameObject::findObject(const TString& nodeName) {
  auto gO = findNode(nodeName);
  return gO.get();
}

SizeT 
GameObject::getChildrenCount() const {
  return m_children.size();
}

void 
GameObject::setStatic(bool _static) {
  m_isStatic = _static;
}

bool 
GameObject::isStatic() const {
  return m_isStatic;
}

bool 
GameObject::changed() const {
  return m_change;
}

TString 
GameObject::getValidName(TString name) {

  Int32 index = m_componentNames[name]++;

  if (index) {
    name += StringUtils::toTString(index);
  }

  return std::move(name);
}

GameObject::SharedGameObj 
GameObject::createInstance() {
  return std::make_shared<GameObject>();
}

bool 
ComponentPartition::operator()(const std::unique_ptr<GameComponent>& l) const {
  return static_cast<bool>(dynamic_cast<ScriptComponent*>(l.get()));
}

GameObject&
GameObject::operator=(GameObject& ref) {
  auto thisPtr = shared_from_this();
  ref.copyData(thisPtr);

  m_tag = ref.m_tag;

  if (auto parent = ref.getParent()) {
    m_parent = parent;
    parent->addChild(thisPtr);
  }

  m_localTransform = ref.m_localTransform;

  m_localTransform.invalidate();

  m_isStatic = ref.m_isStatic;

  static_cast<EnableObject&>(*thisPtr) = ref;

  static_cast<NameObject&>(*thisPtr) = ref;

  setName(ref.getName() + _T(" clone"));
  /**
  dup->m_finalTransform = m_finalTransform;
  dup->m_finalTransform.invalidate();
  **/
  for (auto& component : ref.m_components) {
    component->cloneIn(*thisPtr);
  }

  /*********************************/
  for (auto& child : ref.m_children) {
    //addChild(child->clone(false));
    auto c = child->createInstance();
    *c = *child;
  }

  return *this;

}

bool 
GameObject::operator==(GameObject& ref) {
  return (m_tag.getName() == ref.m_tag.getName());
}

GameObject*
GameObject::getChildByIndex(Int32 index) {
  return getChild(index).get();
}

}