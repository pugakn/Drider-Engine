#pragma once

#include <dr_memory.h>

#include "dr_core_prerequisites.h"
#include "dr_gameComponent.h"

namespace driderSDK {

class Camera;

class DR_CORE_EXPORT CameraComponent : public GameComponent
{
public:
  using SharedCamera = std::shared_ptr<Camera>;
  using WeakCamera = std::weak_ptr<Camera>;

  CameraComponent(GameObject& _gameObject, 
                  SharedCamera _camera = SharedCamera());

  void
  setCamera(SharedCamera camera);

  void
  setActive();

private:  

  // Inherited via GameComponent
  virtual void 
  onCreate() override;

  virtual void 
  onUpdate() override;

  virtual void 
  onRender() override;

  virtual void 
  onDestroy() override;

  virtual UInt32
  getClassID() const override;

  virtual GameComponent* 
  cloneIn(GameObject& _go) override;

  WeakCamera m_camera;
};

}
