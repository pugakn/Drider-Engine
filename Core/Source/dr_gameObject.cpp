#include "dr_gameObject.h"

#include <algorithm>

#include <dr_string_utils.h>

//#include "dr_aabb_collider.h"
#include "dr_render_component.h"
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
    m_isStarted(false),
    m_change(true),
    m_isKilled(false)
    DR_DEBUG_ONLY_PARAM(m_destroyed(false))
{
  //refCount = 1;
}

GameObject::GameObject(const GameObject& other)
{
  *this = other;
}

GameObject::~GameObject() {
  DR_DEBUG_ONLY(
  if (!m_destroyed) {
    Logger::addLog(_T("Object called destructor without begin destoyed() first: ")
                   + getName());
  });
}

void 
GameObject::start() {
  
  m_change = m_localTransform.changed() ||
             getParent()->changed();

  m_localTransform.m_change = false;

  if (m_change) {
    m_finalTransform = m_localTransform * getParent()->m_finalTransform;  
    //m_finalTransform = getParent()->m_finalTransform * m_localTransform;  
  }   

  if (auto render = getComponent<RenderComponent>()) {
    render->onUpdate();
  }

  for (auto& component : m_components) {
    component->onStart();
  }

  for (auto& child : m_children) {
    child->start();
  }

  m_isStarted = true;
}

void
GameObject::update() {

  DR_DEBUG_ONLY(
  if(!m_isStarted) {
    Logger::addLog(_T("Warning: updating object that wasn't started ") + getName());
  });

  m_change = m_localTransform.changed() ||
             getParent()->changed();

  m_localTransform.m_change = false;

  if (m_change) {
    m_finalTransform = m_localTransform * getParent()->m_finalTransform;  
    //m_finalTransform = getParent()->m_finalTransform * m_localTransform;  
  }   

  updateImpl();

  Int32 ck = 0;

  for (auto& component : m_components) {
    
    if (component->isEnabled() && !component->isKilled()) {
   
      component->onUpdate();

      //If the local transform changed inside the component update
      if (m_localTransform.changed()) {

        //Recalculate the final transform
        m_finalTransform = m_localTransform * getParent()->m_finalTransform;
        //m_finalTransform = getParent()->m_finalTransform * m_localTransform;

        m_localTransform.m_change = false;
      }
    }

    if (component->isKilled()) {
      ck++;
    }

    if (isKilled()) {
      return;
    }
  }

  static auto shouldDestroyCmp = 
  [](ComponentPtr& obj) {
    if (obj->isKilled()) {
      obj->onDestroy();
      return true;
    }
    return false;
  };
    
  if (ck) {
    m_components.erase(std::remove_if(m_components.begin(), 
                                      m_components.end(),
                                      shouldDestroyCmp),
                       m_components.end());
  }

  ck = 0;

  for (auto& child : m_children) {

    if (child->isEnabled() && !child->isKilled()) {
      child->update();
    }

    if (child->isKilled()) {
      ck++;
    }
  }

  static auto shouldDestroy = 
  [](SharedGameObj obj) {
    if (obj->isKilled()) {
      obj->destroy();
      return true;
    }
    return false;
  };
  
  if (ck) {
    m_children.erase(std::remove_if(m_children.begin(), 
                                    m_children.end(), 
                                    shouldDestroy),
                     m_children.end());
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
  if (auto comp = getComponent(compName)) {
    comp->kill();
  }
}

//void
//GameObject::removeComponentP(const TString& compName) {
//
//  for (auto it = m_components.begin(); it != m_components.end(); ++it) {
//    if ((*it)->getName() == compName) {
//      (*it)->onDestroy();
//      m_components.erase(it);
//      return;
//    }
//  }
//
//  DR_DEBUG_ONLY(Logger::addLog(_T("Trying to remove unexisting component: ") + 
//                               compName));
//}

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
GameObject::getChild(UInt32 id) {
  
  SharedGameObj ch;

  for (auto& child : m_children) {
    if (id == child->getID()) {
      ch = child;
      break;
    }
  }

  return ch;
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

GameObject::SharedGameObj
GameObject::findNode(const UInt32 idNode) {

  SharedGameObj node = getChild(idNode);

  if (!node) {
    for (auto& child : m_children) {
      if (node = child->findNode(idNode)) {
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

void 
GameObject::kill() const {
  m_isKilled = true;
}

bool 
GameObject::isKilled() const {
  return m_isKilled;
}

GameObject&
GameObject::operator=(const GameObject& ref) {
  auto thisPtr = shared_from_this();

  ref.copyData(thisPtr);

  m_tag = ref.m_tag;

  if (auto parent = ref.getParent()) {
    m_parent = parent;
    parent->addChild(thisPtr);
  }

  m_localTransform = ref.m_localTransform;

  m_finalTransform = ref.m_finalTransform;

  m_isStatic = ref.m_isStatic;

  m_change = ref.m_change;

  m_isStarted = ref.m_isStarted;

  m_isKilled = ref.m_isKilled;

  static_cast<EnableObject&>(*thisPtr) = ref;

  static_cast<NameObject&>(*thisPtr) = ref;

  setName(ref.getName() + _T(" clone"));
  
  m_components.clear();

  m_componentNames.clear();

  for (auto& component : ref.m_components) {
    auto cmp = component->cloneIn(*thisPtr);
    if (cmp->getName() != component->getName()) {
      cmp->setName(component->getName());
    }
  }

  /*********************************/
  m_children.clear();

  for (auto& child : ref.m_children) {
    auto c = child->createInstance();
    *c = *child;
    addChild(c);
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