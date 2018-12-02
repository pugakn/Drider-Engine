#pragma once

#include <dr_memory.h>

#include "dr_core_prerequisites.h"
#include "dr_gameComponent.h"

namespace driderSDK {

class Camera;

/**
* Camera class component.
* Holds the information of this camera.
*
* Sample usage:
* gameObject.createComponent<CameraComponent>();
*/
class DR_CORE_EXPORT CameraComponent : public GameComponent
{
public:
  using SharedCamera = std::shared_ptr<Camera>;
  using WeakCamera = std::weak_ptr<Camera>;

  CameraComponent(GameObject& _gameObject, 
                  SharedCamera _camera = SharedCamera());

  /**
  * Makes this component the main camera.
  */
  void
  setActive();

  /**
  * Returns the viewport width.
  *
  * @return
  *  Returns the width of the camera viewport.
  *
  */
  float
  getWidth();
  
  /**
  * Sets the camera viewport width.
  *
  * @param width
  *  The new width of the camera viewport.
  *
  */
  void
  setWidth(float width);

  /**
  * Returns the viewport height.
  *
  * @return
  *  Returns the height of the camera viewport.
  *
  */
  float
  getHeight();

  /**
  * Sets the camera viewport height.
  *
  * @param height
  *  The new height of the camera viewport.
  *
  */
  void
  setHeight(float height);

  /**
  * Returns the near plane.
  *
  * @return
  *  Returns the near plane of the camera.
  *
  */
  float
  getNearPlane();

  /**
  * Sets the camera near plane.
  *
  * @param nearPlane
  *  The new near plane of the camera.
  *
  */
  void
  setNearPlane(float nearPlane);

  /**
  * Returns the far plane.
  *
  * @return
  *  Returns the far plane of the camera.
  *
  */
  float
  getFarPlane();

  /**
  * Sets the camera far plane.
  *
  * @param farPlane
  *  The new far plane of the camera.
  *
  */
  void
  setFarPlane(float farPlane);
  
  /**
  * Returns the fov.
  *
  * @return
  *  Returns the fov of the camera.
  *
  */
  float
  getFov();
  
  /**
  * Sets the camera fov.
  *
  * @param fov
  *  The new fov of the camera.
  *
  */
  void
  setFov(float fov);

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
  getClassID() override;

  virtual GameComponent* 
  cloneIn(GameObject& _go) override;

  WeakCamera m_camera;
};

}