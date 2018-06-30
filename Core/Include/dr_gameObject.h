#pragma once

#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <dr_id_object.h>
#include <dr_logger.h>
#include <dr_memory.h>
#include <dr_transform.h>

#include "dr_core_prerequisites.h"
#include "dr_enableObject.h"
#include "dr_name_object.h"

#include "dr_gameComponent.h"

#include <dr_export_script.h>
#include <..\..\Script\Include\dr_script_engine.h>

namespace driderSDK {

class GameComponent;

class DR_CORE_EXPORT ComponentPartition
{
 public:
  bool operator()(const std::unique_ptr<GameComponent>& l) const;
};

class GameObject;

GameObject* Ref_GameObject();

class DR_CORE_EXPORT GameObject : public std::enable_shared_from_this<GameObject>,  
                                  public EnableObject,
                                  public NameObject,
                                  public IDObject
{ 
 public:
  using SharedGameObj = std::shared_ptr<GameObject>;
  using WeakGameObj = std::weak_ptr<GameObject>;
  using ChildrenList = std::vector<SharedGameObj>;
  using ComponentPtr = std::unique_ptr<GameComponent>;
  using ComponentsList = std::vector<ComponentPtr>;

  //Int32 refCount;

  GameObject(const TString& name = _T(""));

  GameObject(const GameObject& other);

  virtual ~GameObject();

  bool
  operator==(GameObject&);
  
  /*void
  init();*/
  void 
  start();

  virtual void 
  update();
  
  void
  render();

  void
  destroy();
  
  virtual SharedGameObj
  createInstance();
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
    
    addComponent(std::move(component));

    auto&t = m_components.back();

    t->onCreate();

    if (m_isStarted) {
      t->onStart();
    }

    return rawPtr;
  }

  template<class T>
  std::vector<T*>
  getComponents()
  {
    std::vector<T*> componentCastedList;
    for (auto& componet : m_components) {
      if (auto casted = dynamic_cast<T*>(componet.get())) {
        componentCastedList.push_back(casted);
        continue;
      }
    }
    return componentCastedList;
  }


  /**
  * Gets the frist component of the specified type in the template parameter.
  *
  * @return
  *   If any of the components matches the specified type it'll return 
  *   a pointer to the component, nullptr otherwise.
  */
  template<class T> 
  T* 
  getComponent()
  {
    T* componentCasted = nullptr;
    for (auto& component : m_components) {
      /*if (auto casted = dynamic_cast<T*>(componet.get())) {
        componentCasted = casted;
        break;
      }*/
      
      if (IDClass<T>::ID() == component->getClassID()) {
        componentCasted = static_cast<T*>(component.get());
        break;
      }
    }

    return componentCasted;
  }

  /*template<class T> 
  T* 
  getComponent(UInt32 classID)
  {
    T* componentCasted = nullptr;
    for (auto& component : m_components) {
      if (classID == component->getClassID()) {
        componentCasted = static_cast<T*>(component.get());
        break;
      }
    }

    return componentCasted;
  }*/

  template<class T = GameComponent>
  T*
  getComponent(const TString& componentName)
  {
    T* comp = nullptr;

    for (auto& cmp : m_components) {
      if (cmp->getName() == componentName) {
        comp = cmp.get();
      }
    }

    if (!std::is_same<T, GameComponent>::value) {
      return dynamic_cast<T*>(comp);
    }

    return comp;
  }

  template<class T>
  void
  removeComponent()
  {
    for (auto it = m_components.begin(); it != m_components.end(); ++it) {
      if (T* comp = dynamic_cast<T*>(it->get())) {
        (*it)->onDestroy();
        m_components.erase(it);
        return;
      }
    }

    DR_DEBUG_ONLY(Logger::addLog(_T("Trying to remove unexisting component")));
  }
  
  void
  removeComponent(const TString& compName);

  void
  addComponent(ComponentPtr component);
  
  void 
  addChild(SharedGameObj child);

  /**
  * Sets 
  the parent of the node.
  *
  * @param parent
  * The new parent. If the new parent doesn't has this node as a child, it will be added
  * automatically.
  */
  void 
  setParent(SharedGameObj parent);

  SharedGameObj 
  getParent() const;

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

  const Transform& 
  getWorldTransform() const;

  Transform&
  getTransform();

  const TString&
  getTag() const;

  void 
  setTag(const TString& _tag);

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
  getChild(UInt32 index);
  
  /**
  * Gets a node in the tree hierarchy. If the node isn't on the
  * children list, it'll search recursively on the children of its
  * children. 
  */
  SharedGameObj
  findNode(const TString& nodeName);

  SharedGameObj
  findNode(const UInt32 idNode);

  /**
  * Gets GameObject*
  */
  GameObject* 
  findObject(const TString& nodeName);

  /**
  * Gets the number of children.
  * 
  * @return
  *   The number of children
  */
  SizeT
  getChildrenCount() const;

  void 
  setStatic(bool _static);

  bool 
  isStatic() const;

  bool
  changed() const;

  void kill() const;

  bool 
  isKilled() const;

  GameObject&
  operator=(const GameObject& ref);

  static BEGINING_REGISTER(GameObject, 0, asOBJ_REF | asOBJ_NOCOUNT)

  result = REGISTER_REF_NOCOUNT(GameObject)                                                           

  //Register functions
  result = REGISTER_FOO(GameObject,
                        "GameObject@ findObject(const TString& in)",
                        asMETHODPR(GameObject, findObject, (const TString&), GameObject*))

  result = REGISTER_FOO(GameObject, 
                        "Transform& getTransform()", 
                        asMETHODPR(GameObject, getTransform, (), Transform&))

  result = REGISTER_FOO(GameObject,
                        "const TString& getTag()",
                        asMETHODPR(GameObject, getTag, () const, const TString&))

  result = REGISTER_FOO(GameObject,
                        "void setTag(const TString& in)",
                        asMETHODPR(GameObject, setTag, (const TString&), void))

  result = REGISTER_FOO(GameObject,
                        "GameComponent@ getComponent(const TString& in)",
                        asMETHODPR(GameObject, getComponent, (const TString&), GameComponent*))

//Register operators
  result = REGISTER_OP(GameObject, operator=, opAssign, const GameObject&, GameObject&, "GameObject@", in)

  END_REGISTER 

 // Private functions only use to scripting
 private:
  GameObject*
  getChildByIndex(Int32 index);

 private:

  friend GameComponent;
  
  using NamesMap = std::unordered_map<TString, Int32>;
  
  using NamesSet = std::unordered_set<TString>;
  /**
  * Gets a validated name (not repeaded) for the proposed name.
  */
  TString
  getValidName(TString name);
  
 protected:
  /*void
  removeComponentP(const TString& compName);*/

  virtual void
  copyData(SharedGameObj other) const {}

  virtual void
  updateImpl(){}

  mutable bool m_isKilled;
  bool m_change;
  bool m_isStarted;
  bool m_isStatic;
  ChildrenList m_children;
  ComponentsList m_components;
  Transform m_finalTransform;
  Transform m_localTransform;
  WeakGameObj m_parent;
  NameObject m_tag;
  NamesMap m_componentNames;
  //NamesSet m_componentsToRemove;
  DR_DEBUG_ONLY(bool m_destroyed);
};

}