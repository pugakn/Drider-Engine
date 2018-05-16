#pragma once
#include <dr_camera.h>
#include <dr_viewport.h>
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

#include <dr_scene_viewer.h>
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

  //void createComponentOnGMO(const TString& gmoID, const Int32 type);
  //void removeComponentOnGMO(const TString& gmoID, const TString& componentID);


  void resize(Viewport _viewport);
private:
  void initInputs();
  void initUI();
  void initSceneGraph();
  void loadResources();

  void UI_UpdateSceneGraph();
  void UI_UpdatePropertySheet(const GameObject& obj);


  Viewport m_viewport;



  WebRenderer webRenderer;
  WebRenderer m_netLobby;
  Quad quad;
  TString m_onFocusGMO;

  SceneViewer m_sceneViewer;

  SceneGraph::SharedGameObject model;
  std::shared_ptr<Material> modelMat;
  SceneGraph::SharedGameObject floor;
  std::shared_ptr<Material> floorMat;

  Vector3D modelMovement;
  std::array<Light, 128> Lights;
};
}