#pragma once

#include <dr_gameComponent.h>

#include "dr_util_prerequisites.h"

namespace driderSDK {

class SpiderBehavior : public GameComponent
{
 public:
      
   enum State 
   {

   };

   SpiderBehavior(GameObject& _gameObject);

   virtual void onCreate() override;
   virtual void onUpdate() override;
   virtual void onRender() override;
   virtual void onDestroy() override;

   virtual GameComponent*
   cloneIn(GameObject & _go) override;

 private:
   State m_state;
};

}
