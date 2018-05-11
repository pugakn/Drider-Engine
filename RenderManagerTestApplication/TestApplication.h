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

  RenderMan m_renderMan;

  SceneGraph::SharedGameObject m_selectedGO;
  SizeT m_SzTGosIndex;
  std::vector<SceneGraph::SharedGameObject> m_vecGos;
  std::shared_ptr<Material> m_floorMat;
  std::shared_ptr<Material> m_modelMat;

  Vector3D modelMovement;
  std::array<Light, 128> Lights;
};

}

