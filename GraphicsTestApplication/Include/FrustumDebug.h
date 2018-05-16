#pragma once

#include "DrawableComponent.h"

namespace driderSDK {

class GameObject;
class Camera;

class FrustumDebug : public DrawableComponent 
{
 public:
  
  FrustumDebug(GameObject& _go, Camera* _camera);

  // Inherited via DrawableComponent
  virtual void 
  onCreate() override;
  
  virtual void 
  onUpdate() override;

  virtual void 
  cloneIn(GameObject& _go) override;

  void 
  setCamera(Camera* _camera);
 private:
  void
  create();

  Camera* m_camera;
};

}
