#pragma once
#include <dr_application.h>
#include <dr_renderman.h>
#include <dr_light.h>
#include <dr_material.h>
//#include "LinesTechnique.h"
//#include "FrustumDebug.h"

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
  MoveModel(Vector3D direction);

  void 
  loadResources();

  // Inherited via Application

  //LinesTechnique tecnico;

cc
};

}

