#pragma once
#include <dr_application.h>
#include <dr_renderman.h>
#include <dr_camera_manager.h>
#include <dr_light.h>
#include <dr_material.h>

namespace driderSDK {

namespace TransformMode {
enum E {
  kPosition = 0,
  kScale,
  kRotation,
  kSize
};
}
namespace TransformAxis {
enum E {
  kX = 0,
  kY,
  kZ,
  kXZ,
  kXY,
  kZY,
  kNone
};
}

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
  loadResources();
  
  bool
  selectMoveAxe();

  void
  MoveOnAxe(TransformAxis::E axisToMoveOn);

  void
  selectModel();

  Vector3D
  GetCameraMouseRayDirection(CameraManager::SharedCamera Cam);

  // Inherited via Application
  std::thread m_RenderManagerThread;
  bool render;

  bool m_bSelected;
  SceneGraph::SharedGameObject m_selectedGO;

  TransformMode::E m_TransformMode;
  TransformAxis::E m_SelectedMoveAxis;
  float cubeLarge;
  float cubeDefault;

  std::vector<SceneGraph::SharedGameObject> m_vecGos;
  std::shared_ptr<Material> m_BushMat;
  std::shared_ptr<Material> m_StormtrooperMat;
  std::shared_ptr<Material> m_floorMat;
  std::shared_ptr<Material> m_hkBodyMat;
  std::shared_ptr<Material> m_hkBodySMat;
  std::shared_ptr<Material> m_hkEyeMat;
  std::shared_ptr<Material> m_modelMat;
};

}