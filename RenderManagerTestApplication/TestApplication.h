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

  virtual void
  onResize() override;

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
  Int32 m_SzTGosIndex;
  bool m_bRotate;
  std::vector<SceneGraph::SharedGameObject> m_vecGos;
  std::shared_ptr<Material> m_floorMat;
  std::shared_ptr<Material> m_hkBodyMat;
  std::shared_ptr<Material> m_hkBodySMat;
  std::shared_ptr<Material> m_hkEyeMat;
  std::shared_ptr<Material> m_modelMat;

  Vector3D modelMovement;
  std::array<Light, 128> Lights;
};

}

