#pragma once

#include <dr_application.h>
#include <dr_depth_stencil.h>
#include <dr_gfx_memory.h>
#include <dr_render_target.h>
#include <dr_graph.h>
#include <dr_viewport.h>

namespace driderSDK {

class Editor : public Application
{
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
  void loadMainMenu();
  void initImguiMenus(float mainMenuBarheight);
  void loadHierarchy();
  void loadMenuHierarchy();
  void materialEditor();
  void loadScene();
  void saveScene();
  void loadInspector();
  void loadMenuAddComponent();
  void loadFileManager();
  void loadRenderWindow();
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

  bool showFileDilog;
  bool showSaveFileDialog;
  SceneGraph::SharedGameObject m_selectedGameObject;
  std::shared_ptr<Material> m_selectedMaterial;
};

}
