#pragma once

#include "DrawableComponent.h"

namespace driderSDK {

class GameObject;
class Camera;

class FrustumDebug : public DrawableComponent
{
 public:
  
   FrustumDebug(GameObject& _go, Camera* cam);

   FrustumDebug(const FrustumDebug& copy) = delete;

   FrustumDebug(FrustumDebug&& copy) = delete;


   FrustumDebug& operator=(const FrustumDebug& copy) = delete;

   FrustumDebug& operator=(FrustumDebug&& copy) = delete;

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
  Camera* debugCam;
};

}
