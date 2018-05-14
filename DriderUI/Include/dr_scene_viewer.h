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
    const float W_SCROLL_VEL = 0.2;
    const float W_MOVE_VEL = 0.38;
    const float W_ROT_VEL = 1;
  public:
    void init(Viewport v);
    void draw();
    void destroy();
    void resize(const Viewport& _v);

    RenderMan& getRenderManager();
  private:
    void initInputs();
    void loadResources();
    SceneGraph::SharedGameObject GetGMOMouseCollition();
    //QUERY_ORDER::E m_queryOrder;

    RenderMan m_renderMan;
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