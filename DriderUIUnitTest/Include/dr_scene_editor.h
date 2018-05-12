#pragma once
#include <dr_camera.h>
#include <dr_viewport.h>
//#include <dr_resource_manager.h>
#include <memory>
#include <dr_memory.h>
#include <dr_shader.h>

#include <dr_graph.h>

#include <dr_gfx_memory.h>
#include <dr_render_target.h>
#include <dr_gameObject.h>

#include <dr_web_renderer.h>
#include <dr_quad.h>

#include <dr_renderman.h>
#include <dr_depth_stencil.h>
namespace driderSDK {
class SceneEditor {
public:
  void init(Viewport v);
  void update();
  void draw();
  void destroy();

  std::shared_ptr<GameObject> 
    addGameObject(
    std::shared_ptr<GameObject> parent,
    const TString& name,
    const Vector3D& pos);
  //void removeGameObject();

  void createComponentOnGMO(const TString& gmoID, const Int32 type);// Uno por uno asd gdsagfdsf dsf< ?
  void removeComponentOnGMO(const TString& gmoID, const TString& componentID);


private:
  Int32 m_sceneWidth;
  Int32 m_sceneHeight;
  void initInputs();
  void initCameras();
  void initUI();
  void initSceneGraph();
  void loadResources();
  void sceneResized();

  void UI_UpdateSceneGraph();
  void UI_UpdatePropertySheet(const GameObject& obj);

  SceneGraph::SharedGameObject GetGMOMouseCollition();

  //QUERY_ORDER::E m_queryOrder;

  Viewport m_viewport;
  Viewport m_sceneViewportNormalized;
  std::shared_ptr<Camera> m_camera;
  std::shared_ptr<Camera> m_leftCam;
  std::shared_ptr<Camera> m_upCam;
  std::shared_ptr<Camera> m_activeCam;

  std::shared_ptr<GameObject> m_joker;


  GFXShared<RenderTarget> m_RT;
  GFXShared<DepthStencil> m_RTDPTH;

  WebRenderer webRenderer;
  Quad quad;
  Quad m_editorQuad;


  RenderMan m_renderMan;
  SceneGraph::SharedGameObject model;
  std::shared_ptr<Material> modelMat;
  SceneGraph::SharedGameObject floor;
  std::shared_ptr<Material> floorMat;

  Vector3D modelMovement;
  std::array<Light, 128> Lights;


  TString m_onFocusGMO;
  bool m_sceneOnFocus = true;
  bool m_rotWorldActive = false;
  SceneGraph::SharedGameObject m_GMOOnFocus;
};
}