#pragma once

#include <vector>
#include <memory>
#include <dr_prerequisites.h>
#include <dr_matrix4x4.h>
#include "dr_transform.h"

namespace driderSDK {


class DR_API_EXPORT Node  
{
 public:
  using SharedNode = std::shared_ptr<Node>;
  using WeakNode = std::weak_ptr<Node>;

  /**
  Constructor with a name
  */
  Node(const TString& _name, WeakNode _parent = WeakNode());

  virtual ~Node(){}

  virtual void update(const Matrix4x4& accumulatedTransform);

  void addChild(SharedNode child);

  void setParent(SharedNode parent);

  void setName(const TString& name);

  void removeChild(const TString& childName);

  TString getName();

  WeakNode getParent();

  WeakNode getChild(const TString& childName);

  const Matrix4x4& getWorldTransform() const;

  Transform transform;
 protected:
  Matrix4x4 m_finalTransform;  
  std::vector<SharedNode> m_childs;
  WeakNode m_parent;
  TString m_name;
 private:
};
}

