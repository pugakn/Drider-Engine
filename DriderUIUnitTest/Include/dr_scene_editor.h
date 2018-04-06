#pragma once
#include <dr_camera.h>
#include <dr_viewport.h>
//#include <dr_resource_manager.h>
#include <dr_memory.h>
#include <dr_shader.h>

#include <dr_graph.h>

#include <dr_gfx_memory.h>
#include <dr_render_target.h>


#include <dr_web_renderer.h>
#include <dr_quad.h>
namespace driderSDK {
class SceneEditor {
public:
  void init(Viewport v);
  void update();
  void draw();
  void destroy();

  void addGameObject();
  //void removeGameObject();


private:
  void initInputs();
  void initCameras();
  void initAssets();
  void initUI();

  //QUERY_ORDER::E m_queryOrder;

  Viewport m_viewport;
  std::shared_ptr<Camera> m_camera;
  std::shared_ptr<Camera> m_leftCam;
  std::shared_ptr<Camera> m_upCam;
  std::shared_ptr<Camera> m_activeCam;

  std::shared_ptr<GameObject> m_joker;


  GFXUnique<RenderTarget> m_RT;
  WebRenderer webRenderer;
  Quad quad;
};
}