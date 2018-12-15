#pragma once

#include <dr_application.h>
#include <dr_depth_stencil.h>
#include <dr_gfx_memory.h>
#include <dr_render_target.h>
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
  void initImguiMenus(float mainMenuBarheight);
  void loadHierarchy();
  void loadMenuHierarchy();
  void loadScene();
  void saveScene();
  GFXShared<RenderTarget> m_RT;
  GFXShared<DepthStencil> m_RTDPTH;
  Viewport m_sceneViewport;
  float m_luminanceDelta;
  bool  m_initFlag;
  UInt32 m_selectedItem;

  bool showFileDilog;
  bool showSaveFileDialog;
};

}