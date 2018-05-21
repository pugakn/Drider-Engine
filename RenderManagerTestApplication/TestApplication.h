#pragma once

#include <dr_application.h>
#include <dr_renderman.h>

namespace driderSDK {

class RenderManApp : public Application
{
 public:


 private:

  virtual void 
  postInit() override;

  virtual void 
  postUpdate() override;

  virtual void 
  postRender() override;

  virtual void 
  postDestroy() override;

  void
  initInputCallbacks();

  void
  RotateModel();

  void
  MoveModel(const Vector3D& direction);

  void 
  loadResources();

  // Inherited via Application

  RenderMan m_renderMan;
  SceneGraph::SharedGameObject model;

  Vector3D modelMovement;
};

}

