#pragma once

#include <vector>
#include <memory>
#include <dr_matrix4x4.h>
#include "dr_core_prerequisites.h"
#include "dr_enableObject.h"
#include "dr_transform.h"

namespace driderSDK {


class DR_CORE_EXPORT Node : public std::enable_shared_from_this<Node>, 
                            public EnableObject
{
 public:
  using SharedNode = std::shared_ptr<Node>;
  using WeakNode = std::weak_ptr<Node>;

  /**
  * Constructor with a name and a parent node.
  */
  Node(const TString& _name = _T(""), WeakNode _parent = WeakNode());

  virtual ~Node(){}

  void 
  update();
  
  void 
  addChild(SharedNode child);

  /**
  * Sets the parent of the node.
  *
  * @param parent
  * The new parent. If the new parent doesn't has this node as a child, it will be added
  * automatically.
  */
  void 
  setParent(SharedNode parent);

  /**
  * Sets the name of the node
  *
  * @param name
  *  The new name.
  */
  void 
  setName(const TString& name);

  /**
  * Removes a child from its childredn.
  * 
  * @param child
  *   The specified child to remove.
  */
  void
  removeChild(SharedNode child);

  /**
  * Removes all the children from the node.
  */
  void 
  removeChildren();

  const TString& 
  getName();

  SharedNode 
  getParent();

  /**
  * Gets a list of children.
  * 
  * @param childrenNames
  *  The name of the children.
  * 
  * @return
  *   A list of all the children whose name matches the specified one.
  */
  std::vector<SharedNode>
  getChildrenWithName(const TString& childrenNames);

  /**
  * Gets a child of the node.
  * 
  * @param childName
  *  The name of the child.
  * K
  * @return 
  *   The first child (if theres is more than one with the same name) whose 
  *   name matches the specified one. If no child matches the name nullptr.
  */
  SharedNode 
  getChild(const TString& childName);

  /**
  * Gets a child of the node.
  * 
  * @param index
  *  The index of the child in the children list.
  * 
  * @return
  *   The child at the specified index. If the index is greater or equal to 
  *   the number of childs nullptr.
  */
  SharedNode
  getChild(SizeT index);
  
  /**
  * Gets the number of children.
  * 
  * @return
  *   The number of children
  */
  SizeT
  getChildrenCount();

  const Transform& 
  getWorldTransform() const;

  /***************/
  /*     TEMP    */
  virtual void
  draw();
  /***************/

  template<class T>
  std::shared_ptr<T>  
  getAs()
  {
    return std::dynamic_pointer_cast<T>(shared_from_this());
  }  

  Transform transform;
 protected:
  virtual void 
  updateImpl();

 protected:
  std::vector<SharedNode> m_children;
  Transform m_finalTransform;
  WeakNode m_parent;
  TString m_name;
 private:
};
}

