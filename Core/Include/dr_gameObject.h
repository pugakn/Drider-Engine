#pragma once

#include <vector>
#include "dr_core_prerequisites.h"
#include "dr_gameComponent.h"
#include "dr_node.h"

namespace driderSDK {

class GameObject : public Node
{ 


  template<class T, class...Args>
  void 
  createComponent(Args&&...args);

  template<class T> 
  T* getComponent();

  
 private:
  using ComponentPtr = std::unique_ptr<GameComponent>;
  using ComponentsList = std::vector<ComponentPtr>;
   ComponentsList m_components;
};

}