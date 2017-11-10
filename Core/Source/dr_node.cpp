#include "dr_node.h"

namespace driderSDK {
Node::Node(const TString & _name, WeakNode _parent) 
  : m_parent(_parent),
    m_finalTransform(Matrix4x4::identityMat4x4),
    m_name(_name) {
}

void Node::update(const Matrix4x4& accumulatedTransform) {

  transform.update();

  m_finalTransform = accumulatedTransform * 
                     transform.getTransformMatrix();

  for(auto& child : m_childs) {
    child->update(m_finalTransform);
  }

}

void Node::addChild(SharedNode child) {
  m_childs.push_back(child);
}

void Node::setParent(SharedNode parent) {
  m_parent = parent;
}

void Node::setName(const TString& name) {
  m_name = name;
}

void Node::removeChild(const TString& childName) {
  for(auto child = m_childs.begin(); child != m_childs.end(); ++child) {
    if((*child)->getName() == childName) {
      m_childs.erase(child);
      return;
    }
  }
}

TString Node::getName() {
  return m_name;
}

Node::WeakNode Node::getParent() {
  return m_parent;
}

Node::WeakNode Node::getChild(const TString & childName) {
  for(auto& child : m_childs) {
    if(childName == child->getName()) {
      return child;
    }
  }
}

const Matrix4x4& Node::getWorldTransform() const{
  return m_finalTransform;
}

}