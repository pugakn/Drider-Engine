#include "dr_node.h"

namespace driderSDK {

Node::Node(const TString& _name, WeakNode _parent) 
  : m_parent(_parent),
    m_name(_name) {
}

void 
Node::update() {

  updateImpl();

  for (auto& child : m_children) {
    child->update();
  }
}

void 
Node::addChild(SharedNode child) {

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
Node::setParent(SharedNode parent) {
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

void 
Node::setName(const TString& name) {
  m_name = name;
}

void 
Node::removeChild(SharedNode child) {

  auto childIt = std::find(m_children.begin(), m_children.end(), child);

  assert(childIt != m_children.end() && "Is not my child!");

  child->setParent(nullptr);

  m_children.erase(childIt);
}

void 
Node::removeChildren() {

  for (auto& child : m_children) {
    child->setParent(nullptr);
  }
  
  m_children.clear();
}

const TString& 
Node::getName() {
  return m_name;
}

Node::SharedNode 
Node::getParent() {
  return m_parent.lock();
}

std::vector<Node::SharedNode> 
Node::getChildrenWithName(const TString & childrenNames) {
  std::vector<SharedNode> children;
  
  for (auto& child : m_children) {
    if (child->getName() == childrenNames) {
      children.push_back(child);
    }
  }

  return children;
}

Node::SharedNode 
Node::getChild(const TString & childName) {
  for (auto& child : m_children) {
    if (childName == child->getName()) {
      return child;
    }
  }

  return SharedNode();
}

Node::SharedNode 
Node::getChild(SizeT index) {
  SharedNode child;

  if (index < m_children.size()) {
    child = m_children[index];
  }

  return child;
}

SizeT Node::getChildrenCount() {
  return m_children.size();
}

const Transform&
Node::getWorldTransform() const{
  return m_finalTransform;
}

void 
Node::draw() {
  for (auto& child : m_children) {
    child->draw();
  }
}

void 
Node::updateImpl() {
  m_finalTransform = getParent()->m_finalTransform * transform;
}

}