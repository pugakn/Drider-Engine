#include "dr_node.h"

namespace driderSDK {

Node::Node(const TString & _name, WeakNode _parent) 
  : m_parent(_parent),
    m_finalTransform(Math::kIdentity),
    m_name(_name) {
}

void Node::update(const Matrix4x4& accumulatedTransform) {

  m_finalTransform = accumulatedTransform * 
                     transform.getTransformMatrix();

  for (auto& child : m_childs) {
    child->update(m_finalTransform);
  }

}

void Node::addChild(SharedNode child) {
  auto childIt = std::find(m_childs.begin(), m_childs.end(), child);
  //if child already exist in children list don't add
  if (child && childIt == m_childs.end()) {
    m_childs.push_back(child);
    auto thisPtr = shared_from_this();
    auto childParent = child->getParent();

    if (childParent && childParent != thisPtr) {
      //remove child from children list of previus parent
      childParent->removeChild(child);
    }

    child->setParent(thisPtr);
  }
}

void Node::setParent(SharedNode parent) {
  auto lastParent = getParent();
  auto thisPtr = shared_from_this();

  if (parent && lastParent != parent) {
    if (lastParent) {
      lastParent->removeChild(thisPtr);
    }

    parent->addChild(thisPtr);
    m_parent = parent;
  }
}

void Node::setName(const TString& name) {
  m_name = name;
}

void Node::removeChild(const TString& childName) {
  for (auto child = m_childs.begin(); child != m_childs.end(); ++child) {
    if ((*child)->getName() == childName) {
      m_childs.erase(child);
      return;
    }
  }
}

void Node::removeChild(SharedNode child) {

  auto childIt = std::find(m_childs.begin(), m_childs.end(), child);

  if (childIt != m_childs.end()) {
    m_childs.erase(childIt);
  }

}

TString Node::getName() {
  return m_name;
}

Node::SharedNode Node::getParent() {
  return m_parent.lock();
}

Node::SharedNode Node::getChild(const TString & childName) {
  for (auto& child : m_childs) {
    if (childName == child->getName()) {
      return child;
    }
  }

  return SharedNode();
}

const Matrix4x4& Node::getWorldTransform() const{
  return m_finalTransform;
}

}