#pragma once

#include <vector>
#include <dr_memory.h>
#include "dr_core_prerequisites.h"
#include "dr_node.h"

namespace driderSDK {

class GameComponent;

class GameObject : public Node
{ 
 public:
  using ComponentPtr = std::unique_ptr<GameComponent>;
  using ComponentsList = std::vector<ComponentPtr>;

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
  GameComponent* 
  createComponent(Args&&...args)
  {
    ComponentPtr component = dr_make_unique<T>(*this, 
                                               std::forward<Args>(args)...);
    
    component->onCreate();

    m_components.push_back(std::move(component));

    return m_components.back().get();
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
 private:
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