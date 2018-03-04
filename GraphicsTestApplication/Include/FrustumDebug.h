#pragma once

#include "DrawableComponent.h"

namespace driderSDK {

class GameObject;

class FrustumDebug : public DrawableComponent 
{
public:
  
  FrustumDebug(GameObject& _go);

  // Inherited via DrawableComponent
  virtual void 
  onCreate() override;
  
  virtual void 
  onUpdate() override;

  virtual void 
  cloneIn(GameObject& _go) override;
private:
  void
  create();
};

}
