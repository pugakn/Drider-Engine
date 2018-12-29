#pragma once

#include <dr_application.h>
#include <dr_camera_manager.h>
#include <dr_depth_stencil.h>
#include <dr_gfx_memory.h>
#include <dr_render_target.h>
#include <dr_graph.h>
#include <dr_vector2d.h>
#include <dr_viewport.h>

#include "imguihelper.h"
#include "imguidock.h"

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

class ScriptComponent;

class Editor : public Application
{
  const float W_SCROLL_VEL = 0.2f;
  const float W_MOVE_VEL = 0.38f;
  const float W_ROT_VEL = 1.f;
  // Inherited via
  private:
  void initCallbacks();
  void initRT();
  virtual void postInit() override;
  virtual void postUpdate() override;
  virtual void postRender() override;
  virtual void postDestroy() override;
  virtual void onResize() override;

  void initSceneGraph();
  void initScriptEngine();
  void initInputs();
  void loadMainMenu();
  void initImguiMenus(float mainMenuBarheight);
  void loadHierarchy();
  void loadMenuHierarchy();
  void materialEditor();
  void loadSavedLayouts();
  //void loadScene();
  //void saveScene();
  void saveCurrentLayout();
  //void createMat();
  void dockerTest();
  void loadInspector();
  void loadMenuAddComponent();
  void loadFileManager();
  void loadRenderWindow();
  void drawDebugStuff();
  bool getMouseInScene(Vector2D* mousePosition = nullptr);


  bool
  selectMoveAxe();

  void
  MoveOnAxe(TransformAxis::E axisToMoveOn);

  void
  selectModel();

  Vector3D
  GetCameraMouseRayDirection(CameraManager::SharedCamera Cam);

  GFXShared<RenderTarget> m_RT;
  GFXShared<DepthStencil> m_RTDPTH;
  Viewport m_sceneViewport;
  float m_luminanceDelta;
  float m_mainMenuBarheight;
  bool  m_initFlag;
  bool m_renderConfigWindow;
  bool m_hierarchyWindow;
  bool m_sceneWindow;
  bool m_inpectorWindow;
  bool m_fileManagerWindow;
  bool m_materialEditorWindow;
  GameObject* m_root;

  UInt32 m_selectedItem;

  bool m_bSelected;

  TransformMode::E m_TransformMode;
  TransformAxis::E m_SelectedMoveAxis;
  float cubeLarge;
  float cubeDefault;
  bool m_bOffseted;
  float m_fOffset;

  //bool showSaveFileDialog;
  //bool createMaterialFileDialog;
  bool m_rotWorldActive = false;
  bool m_movWorldActive = false;
  float m_posMouseSceneWindow[2];
  SceneGraph::SharedGameObject m_selectedGameObject;
  std::shared_ptr<Material> m_selectedMaterial;
  SceneGraph::SharedGameObject m_GMOOnFocus;
  std::vector<TString> semantics;
  //std::vector<ScriptComponent*> m_scripts;
  ImGui::DockContext* m_dockContext;
  std::vector<String> m_savedLayouts;
  Int32 m_currentLayout = -1;
  const String m_layoutsPath = "Resources\\Layouts";
};

}
