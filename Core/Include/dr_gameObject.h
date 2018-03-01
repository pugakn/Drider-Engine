#pragma once

#include <vector>
#include <dr_memory.h>
#include <dr_transform.h>
#include "dr_core_prerequisites.h"
#include "dr_enableObject.h"

namespace driderSDK {

class GameComponent;

class DR_CORE_EXPORT GameObject : public std::enable_shared_from_this<GameObject>,  
                                  public EnableObject                  
{ 
 public:
  using SharedGameObj = std::shared_ptr<GameObject>;
  using WeakGameObj = std::weak_ptr<GameObject>;
  using ChildrenList = std::vector<SharedGameObj>;
  using ComponentPtr = std::unique_ptr<GameComponent>;
  using ComponentsList = std::vector<ComponentPtr>;

  GameObject(const TString& name = _T(""));

  virtual ~GameObject();
  
  GameObject(const GameObject&) = delete;

  GameObject& operator=(const GameObject&) = delete;

  void
  init();

  void 
  update();
  
  void
  render();

  /*virtual void
  destroy();*/

  /**
  * Creates a component of the specified type and adds it to the
  * list of components.
  *
  * @param args
  *  Parameters for the constructor of the component.
  * 
  * @return
  *   A pointer to the created component.
  */
  template<class T, class...Args>
  T* 
  createComponent(Args&&...args)
  {
    auto component = dr_make_unique<T>(*this, 
                                       std::forward<Args>(args)...);
    
    T* rawPtr = component.get();
    
    m_components.push_back(std::move(component));

    m_components.back()->onCreate();

    return rawPtr;
  }

  /**
  * Gets the component of the specified type in the template parameter.
  *
  * @return
  *  If any of the components matches the specified type it'll return 
  *  a pointer to the component, nullptr otherwise.
  */
  template<class T> 
  T* getComponent()
  {
    T* componentCasted = nullptr;
    for (auto& componet : m_components) {
      if(auto casted = dynamic_cast<T*>(componet.get())){
        componentCasted = casted;
        break;
      }
    }

    return componentCasted;
  }
  
  void
  addComponent(ComponentPtr component);

  
  void 
  addChild(SharedGameObj child);

  /**
  * Sets the parent of the node.
  *
  * @param parent
  * The new parent. If the new parent doesn't has this node as a child, it will be added
  * automatically.
  */
  void 
  setParent(SharedGameObj parent);

  SharedGameObj 
  getParent();

  /**
  * Removes a child from its childredn.
  * 
  * @param child
  *   The specified child to remove.
  */
  void
  removeChild(SharedGameObj child);

  /**
  * Removes all the children from the node.
  */
  void 
  removeChildren();

  /**
  * Sets the name of the node
  *
  * @param name
  *  The new name.
  */
  void 
  setName(const TString& name);

  const TString& 
  getName();

  const Transform& 
  getWorldTransform() const;

  Transform&
  getTransform();

  /**
  * Gets a list of children.
  * 
  * @param childrenNames
  *  The name of the children.
  * 
  * @return
  *   A list of all the children whose name matches the specified one.
  */
  ChildrenList
  getChildrenWithName(const TString& childrenNames);

  const ChildrenList&
  getChildren();

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
  SharedGameObj 
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
  SharedGameObj
  getChild(SizeT index);
  
  /**
  * Gets a node in the tree hierarchy. If the node isn't on the
  * children list, it'll search recursively on the children of its
  * children. 
  */
  SharedGameObj
  findNode(const TString& nodeName);

  /**
  * Gets the number of children.
  * 
  * @return
  *   The number of children
  */
  SizeT
  getChildrenCount();

  void 
  setStatic(bool _static);

  bool 
  isStatic() const;

 private:

  void
  propagateChange();

 protected:

  virtual void
  updateImpl();


  bool m_change;
  bool m_isStatic;
  ChildrenList m_children;
  ComponentsList m_components;
  Transform m_finalTransform;
  Transform m_localTransform;
  TString m_name;
  WeakGameObj m_parent;
};

}