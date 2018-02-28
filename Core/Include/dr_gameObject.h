#pragma once

#include <vector>
#include <dr_memory.h>
#include "dr_core_prerequisites.h"
#include "dr_node.h"

namespace driderSDK {

class GameComponent;

class DR_CORE_EXPORT GameObject : public Node
{ 
 public:
  using ComponentPtr = std::unique_ptr<GameComponent>;
  using ComponentsList = std::vector<ComponentPtr>;

  GameObject(const TString& name = _T(""), WeakNode parent = WeakNode());

  ~GameObject();
  
  GameObject(const GameObject&) = delete;

  GameObject& operator=(const GameObject&) = delete;

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
 protected:
   void
   updateImpl();
   /***************/
   /*     TEMP    */ 
   void
   draw();
   /***************/
 private:
   ComponentsList m_components;
};

}