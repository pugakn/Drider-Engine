#pragma once
#include "dr_ui_prerequisites.h"

#include <dr_viewport.h>
#include <dr_memory.h>
#include <dr_graph.h>
#include <dr_gfx_memory.h>
#include <dr_web_renderer.h>
#include <dr_quad.h>
#include <dr_renderman.h>
namespace driderSDK {
class DR_UI_EXPORT SceneViewer {
  const float W_SCROLL_VEL = 0.2f;
  const float W_MOVE_VEL = 0.38f;
  const float W_ROT_VEL = 1.f;
public:

  /**
  * Init scene viewer
  *
  * @param _v
  *  Render area viewport
  *
  */
  void 
  init(Viewport v);
  /**
  * 1.- Draw render manager to RT
  * 2.- Draw RT texture on a quad with m_viewport
  *
  */
  void 
  draw();
  /**
  * Resize the render area
  *
  */
  void 
  destroy();
  /**
  * Resize the render area
  *
  * @param _v
  *  New viewport
  *
  */
  void 
  resize(const Viewport& _v);

  RenderManager& getRenderManager();
private:
  void initInputs();
  SceneGraph::SharedGameObject GetGMOMouseCollition();

  RenderManager m_renderMan;
  Viewport m_sceneViewport;
  GFXShared<RenderTarget> m_RT;
  GFXShared<DepthStencil> m_RTDPTH;
  Quad m_editorQuad;

  bool m_sceneOnFocus = true;
  bool m_rotWorldActive = false;
  bool m_movWorldActive = false;
  SceneGraph::SharedGameObject m_GMOOnFocus;
};
}