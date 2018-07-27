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

  virtual GameComponent*
  cloneIn(GameObject& _go) override;

    // Inherited via DrawableComponent
  virtual UInt32 
  getClassID() const override;

  void 
  setCamera(Camera* _camera);
 private:
  void
  create();

  Camera* m_camera;
};

}
