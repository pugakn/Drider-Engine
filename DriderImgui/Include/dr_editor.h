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
#include "ImGuizmo.h"
#include "dr_client.h"

namespace driderSDK {

class ScriptComponent;

struct CameraProperties
{
  float panVelocity = 500.f;
  float rotationVelocity = 60.f;
  float boostMultiplier = 1.5f;
};

class Editor : public Application,
               public Client
{
  const float W_SCROLL_VEL = 0.2f;
  const float W_MOVE_VEL = 0.38f;
  const float W_ROT_VEL = 1.f;
protected:
  virtual void
  onJoinAccepted() override;

  virtual void
  onJoinDenied() override;

  virtual void
  onConnectionLoss() override;

  virtual void
  onLobbiesListReceived(LobbiesList&& lobbies) override;

  virtual void
  onGameStatusReceived(UInt8 num_players,
                       std::vector<Vector3D> positions) override;

  virtual void
  onInstantiatePlayer(bool isLocalPlayer,
                      const TString& name,
                      const Vector3D& pos,
                      const Vector3D& dir) override;
  // Inherited via
  private:
  void initCallbacks();
  void initRT();
  virtual void postInit() override;
  virtual void postUpdate() override;
  virtual void postRender() override;
  virtual void postDestroy() override;
  virtual void onResize() override;

  void initImGuiStyle();
  void initSceneGraph();
  void initScriptEngine();
  void initInputs();
  void loadMainMenu();
  void loadEditorCameraWindow();
  void loadHierarchy();
  void loadMenuHierarchy();
  void materialEditor();
  void loadSavedLayouts();
  //void loadScene();
  //void saveScene();
  void saveCurrentLayout();
  void showHierarchy(const std::shared_ptr<GameObject>& object, Int32 index);
  void showHierarchySeparator(const std::shared_ptr<GameObject>& object, 
                              const std::shared_ptr<GameObject>& parent,
                              Int32 index);
  //void createMat();
  void dockerTest();
  void loadInspector();
  void loadMenuAddComponent();
  void loadFileManager();
  void loadRenderWindow();
  void drawDebugStuff();
  bool getMouseInScene(Vector2D* mousePosition = nullptr);

  void
  selectModel();

  Vector3D
  GetCameraMouseRayDirection(CameraManager::SharedCamera Cam);

  GFXShared<RenderTarget> m_RT;
  GFXShared<DepthStencil> m_RTDPTH;
  Viewport m_sceneViewport;
  float m_luminanceDelta;
  bool m_renderConfigWindow = true;
  bool m_hierarchyWindow = true;
  bool m_sceneWindow = true;
  bool m_inpectorWindow = true;
  bool m_fileManagerWindow = true;
  bool m_materialEditorWindow = true;
  bool m_editorCamConfigWindow = true;
  GameObject* m_root;

  UInt32 m_selectedItem;

  bool m_bSelected;
  bool m_bShowGizmos;

  ImGuizmo::OPERATION m_TransformMode;
  ImGuizmo::MODE m_space;

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
  //std::vector<UInt32> m_selectedObjects;
  SceneGraph::SharedGameObject m_makeParent;
  SceneGraph::SharedGameObject m_makeChild;
  Int32 m_insertIndex;

  Vector3D lastMousePos;
  Vector3D currentMousePos;
  //Vector3D deltaMouse;

  //Connection values
  bool m_err;
  LobbiesList m_lobbies;
  String m_nameStr;

  bool m_connected;
  bool m_valueRegistered;
  bool m_rotatingCamera = false;

  std::vector<std::shared_ptr<GameObject>> m_players;
  CameraProperties m_editorCamProperties;

};

}
